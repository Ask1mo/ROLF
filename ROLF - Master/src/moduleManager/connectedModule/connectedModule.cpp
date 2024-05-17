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
    compassConnectors = new CompassConnector*[DIRECTIONS];
    compassConnectors[0] = new CompassConnector(DIRECTION_NORTH,  baseInfo.northPipe);
    compassConnectors[1] = new CompassConnector(DIRECTION_EAST,   baseInfo.eastPipe);
    compassConnectors[2] = new CompassConnector(DIRECTION_SOUTH,  baseInfo.southPipe);
    compassConnectors[3] = new CompassConnector(DIRECTION_WEST,   baseInfo.westPipe);
    compassConnectors[4] = new CompassConnector(DIRECTION_UP,     baseInfo.upPipe);
    compassConnectors[5] = new CompassConnector(DIRECTION_DOWN,   baseInfo.downPipe);

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
    if (neighborID == ADDR_NONE)
    {
        if (this->compassConnectors[direction] != NULL)
        {
            delete this->compassConnectors[direction];
            this->compassConnectors[direction] = NULL;
        }
    }
    else
    {
        if (this->compassConnectors[direction] == NULL)
        {
            this->compassConnectors[direction] = new CompassConnector(neighborID, neighborDirection);
        }
        else
        {
            this->compassConnectors[direction]->updateConnection(neighborID, neighborDirection);
        }
    }
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