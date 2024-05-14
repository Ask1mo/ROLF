#include "connectedModule.h"


//Constructor
ConnectedModule::ConnectedModule(String macAdress, String ipAdress, uint8_t moduleID, uint8_t shape)
{
    this->macAdress = macAdress;
    this->ipAdress = ipAdress;
    this->moduleID = moduleID;
    this->shape = shape;
    this->neighborModules = new NeighborModule*[DIRECTIONS];
    for (int i = 0; i < DIRECTIONS; i++)
    {
        this->neighborModules[i] = NULL;
    }
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


