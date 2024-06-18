#include "connectedModule.h"

String directionToString(uint8_t direction)
{
    switch (direction)
    {
        case DIRECTION_NONE:
        return "None0 ";
        case DIRECTION_NORTH:
        return "North1";
        case DIRECTION_EAST:
        return "East2 ";
        case DIRECTION_SOUTH:
        return "South3";
        case DIRECTION_WEST:
        return "West4 ";
        case DIRECTION_UP:
        return "Up5   ";
        case DIRECTION_DOWN:
        return "Down6 ";
        default:
        return "DIRERR";
    }
}

//Constructor
ConnectedModule::ConnectedModule(String macAdress, String ipAdress, uint8_t moduleID, BaseInfo baseInfo)
{
    this->macAdress = macAdress;
    this->ipAdress = ipAdress;
    this->moduleID = moduleID;
    this->baseInfo = baseInfo;
    puzzlePlaced = false;
    //Todo: make this prettier.
    compassConnectors[0] = {DIRECTION_NORTH, baseInfo.northPipeLength, baseInfo.northPipeDelay,    DIRECTION_NORTH,0, 0};
    compassConnectors[1] = {DIRECTION_EAST,  baseInfo.eastPipeLength,  baseInfo.eastPipeDelay,    DIRECTION_EAST, 0, 0};
    compassConnectors[2] = {DIRECTION_SOUTH, baseInfo.southPipeLength, baseInfo.southPipeDelay,    DIRECTION_SOUTH,0, 0};
    compassConnectors[3] = {DIRECTION_WEST,  baseInfo.westPipeLength,  baseInfo.westPipeDelay,    DIRECTION_WEST, 0, 0};
    compassConnectors[4] = {DIRECTION_UP,    baseInfo.upPipeLength,    baseInfo.upPipeDelay,    DIRECTION_UP,   0, 0};
    compassConnectors[5] = {DIRECTION_DOWN,  baseInfo.downPipeLength,  baseInfo.downPipeDelay,    DIRECTION_DOWN, 0, 0};

}
void ConnectedModule::setIpAdress(String ipAdress)
{
    this->ipAdress = ipAdress;
}

void ConnectedModule::updateIpAdress(String ipAdress)
{
    this->ipAdress = ipAdress;
}

uint8_t ConnectedModule::getModuleID()
{
    return this->moduleID;
}

String ConnectedModule::getMacAdress()
{
    return this->macAdress;
}

String ConnectedModule::getIpAdress()
{
    return this->ipAdress;
}

void ConnectedModule::updateConnection(uint8_t direction, uint8_t neighborID, uint8_t neighborDirection)
{
    if (direction >= DIRECTIONS || direction == 0 || neighborDirection >= DIRECTIONS)
    {
        Serial.println("ConnectedModule::updateConnection: Invalid direction");
        return;
    }
    compassConnectors[direction-1].neighborAdress = neighborID;
    compassConnectors[direction-1].neighborDirection = neighborDirection;
}


bool ConnectedModule::getPuzzlePlaced()
{
    return puzzlePlaced;
}
void ConnectedModule::setPuzzlePlaced(bool puzzlePlaced)
{
    this->puzzlePlaced = puzzlePlaced;
}

BaseInfo ConnectedModule::getBaseInfo()
{
    return baseInfo;
}

bool ConnectedModule::checkHasNeighbor(uint8_t neighborID)
{
    for (uint8_t i = 0; i < DIRECTIONS; i++)
    {
        if (compassConnectors[i].neighborAdress == neighborID)
        {
            //Serial.print("ConnectedModule::checkHasNeighbor: Neighbor found with ID ");
          //  Serial.println(neighborID);
            return true; //Return the direction of the neighbor.
        }
    }

  //  Serial.print("ERROR: ConnectedModule::checkHasNeighbor: No neighbor found with ID");
 //   Serial.println(neighborID);
    return false;
}


/*CompassConnector ConnectedModule::getConnectorInfo(uint8_t direction)
{
    if (direction >= DIRECTIONS)
    {
        Serial.println("ConnectedModule::getConnectorInfo: Invalid direction");
        return {0, 0, 0, 0};
    }
    return compassConnectors[direction];
}*/
CompassConnector ConnectedModule::getConnectorFromCompensatedDirection(uint8_t compensatedDirection)
{
    if (compensatedDirection > DIRECTIONS)
    {
        Serial.println("ConnectedModule::getConnectorInfo_RotationAdjusted: Invalid direction");
        return {0, 0, 0, 0};
    }
    for (uint8_t i = 0; i < DIRECTIONS; i++)
    {
        if (compassConnectors[i].rotationCompensatedDirection == compensatedDirection)
        {
            return compassConnectors[i];
        }
    }

    return {0, 0, 0, 0};
}


void ConnectedModule::rotate(uint8_t rotation)
{
    Serial.print("Rotating module ");
    Serial.print(rotation);
    Serial.println(" times.");
    //Stupid solution, but it works. It just manually rotates the compassConnectors for "i" amount of times. Maybe its good?
    for (uint8_t i = 0; i < DIRECTIONS-2; i++)// NOTE: DOES NOT DO UP/DOWN
    {
        compassConnectors[i].rotationCompensatedDirection = compassConnectors[i].compassDirection; //Reset the rotationCompensatedDirection to the compassDirection.
        for (uint8_t j = 0; j < rotation; j++)
        {
            compassConnectors[i].rotationCompensatedDirection++;
            if (compassConnectors[i].rotationCompensatedDirection > DIRECTION_WEST) compassConnectors[i].rotationCompensatedDirection = DIRECTION_NORTH;
            
        }
        Serial.print("Rotated direction ");
        Serial.print(directionToString(compassConnectors[i].compassDirection));
        Serial.print(" to ");
        Serial.println(directionToString(compassConnectors[i].rotationCompensatedDirection));
    }
}

CompassConnector ConnectedModule::getConnectorFromID(uint8_t neighborID)
{
    for (uint8_t i = 0; i < DIRECTIONS; i++)
    {
        if (compassConnectors[i].neighborAdress == neighborID)
        {
            return compassConnectors[i];
        }
    }
    Serial.println("ERROR: ConnectedModule::getConnectorFromID: No neighbor found with that ID");
    return {0, 0, 0, 0};
}
void ConnectedModule::printConnectors()
{
    Serial.print("Printing pipes on module ");
    Serial.println(moduleID);
    for (uint8_t i = 0; i < DIRECTIONS; i++)
    {
        Serial.print("Direction: ");
        Serial.print(directionToString(compassConnectors[i].compassDirection));
        Serial.print(" - CompensatedDirection: ");
        Serial.print(directionToString(compassConnectors[i].rotationCompensatedDirection));
        Serial.print(" - Pipe length: ");
        Serial.print(compassConnectors[i].basePipe);
        Serial.print(" - NeighborAdress: ");
        Serial.print(compassConnectors[i].neighborAdress);
        Serial.print(" - Neighbor's Direction: ");
        Serial.println(directionToString(compassConnectors[i].neighborDirection));
    }
}

uint16_t ConnectedModule::getPipeDelayFromCompensatedDirection(uint8_t compensatedDirection)
{
    for (uint8_t i = 0; i < DIRECTIONS; i++)
    {
        if (compassConnectors[i].rotationCompensatedDirection == compensatedDirection)
        {
            return compassConnectors[i].pipeDelay;
        }
    }
    Serial.println("ERROR: ConnectedModule::getPipeDelayFromCompensatedDirection: No pipe found with that direction");
    return 0;
}