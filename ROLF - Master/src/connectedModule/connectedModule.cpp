#include "connectedModule.h"


//Constructor
ConnectedModule::ConnectedModule(String macAdress, uint8_t moduleID, uint8_t shape)
{
    this->macAdress = macAdress;
    this->moduleID = moduleID;
    this->shape = shape;
    this->neighborModules = new NeighborModule*[DIRECTIONS];
    for (int i = 0; i < DIRECTIONS; i++)
    {
        this->neighborModules[i] = NULL;
    }
}
