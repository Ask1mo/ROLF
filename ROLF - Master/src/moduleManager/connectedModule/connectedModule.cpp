#include "connectedModule.h"


//Constructor
ConnectedModule::ConnectedModule(String macAdress, String ipAdress, uint8_t moduleID, BaseInfo baseInfo)
{
    this->macAdress = macAdress;
    this->ipAdress = ipAdress;
    this->moduleID = moduleID;
    this->baseInfo = baseInfo;
    puzzlePlaced = false;
    //Todo: make this prettier.
    compassConnectors[0] = {DIRECTION_NORTH, baseInfo.northPipe,    DIRECTION_NORTH,0, 0};
    compassConnectors[1] = {DIRECTION_EAST,  baseInfo.eastPipe,     DIRECTION_EAST, 0, 0};
    compassConnectors[2] = {DIRECTION_SOUTH, baseInfo.southPipe,    DIRECTION_SOUTH,0, 0};
    compassConnectors[3] = {DIRECTION_WEST,  baseInfo.westPipe,     DIRECTION_WEST, 0, 0};
    compassConnectors[4] = {DIRECTION_UP,    baseInfo.upPipe,       DIRECTION_UP,   0, 0};
    compassConnectors[5] = {DIRECTION_DOWN,  baseInfo.downPipe,     DIRECTION_DOWN, 0, 0};

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
    if (direction >= DIRECTIONS || neighborDirection >= DIRECTIONS)
    {
        Serial.println("ConnectedModule::updateConnection: Invalid direction");
        return;
    }
    compassConnectors[direction].neighborAdress = neighborID;
    compassConnectors[direction].neighborDirection = neighborDirection;
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

uint8_t ConnectedModule::checkHasNeighbor(uint8_t neighborID)
{
    for (uint8_t i = 0; i < DIRECTIONS; i++)
    {
        if (compassConnectors[i].neighborAdress == neighborID)
        {
            return i; //Return the direction of the neighbor.
        }
    }
    return false;
}

CompassConnector ConnectedModule::getConnectorInfo(uint8_t direction)
{
    if (direction >= DIRECTIONS)
    {
        Serial.println("ConnectedModule::getConnectorInfo: Invalid direction");
        return {0, 0, 0, 0};
    }
    return compassConnectors[direction];
}
CompassConnector ConnectedModule::getConnectorInfo_RotationAdjusted(uint8_t rotationAdjustedDirection)
{
    if (rotationAdjustedDirection >= DIRECTIONS)
    {
        Serial.println("ConnectedModule::getConnectorInfo_RotationAdjusted: Invalid direction");
        return {0, 0, 0, 0};
    }
    for (uint8_t i = 0; i < DIRECTIONS; i++)
    {
        if (compassConnectors[i].rotationCompensatedDirection == rotationAdjustedDirection)
        {
            return compassConnectors[i];
        }
    }
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
        Serial.print(compassConnectors[i].compassDirection);
        Serial.print(" to ");
        Serial.println(compassConnectors[i].rotationCompensatedDirection);
    }
    
}