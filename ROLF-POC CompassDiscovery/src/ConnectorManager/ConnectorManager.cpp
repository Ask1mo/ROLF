#include "ConnectorManager.h"

ConnectorManager::ConnectorManager(uint8_t *moduleAdress)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating ConnectorManager at adress "));
        Serial.println((int)this, DEC);
    }

    this->compassConnectors = new CompassConnector*[DIRECTIONS];
    this->compassConnectors[DIRECTION_NORTH] =  new CompassConnector(PIN_COMPASS_NORTH, DIRECTION_NORTH,    "North",    moduleAdress);
    this->compassConnectors[DIRECTION_EAST] =   new CompassConnector(PIN_COMPASS_EAST,  DIRECTION_EAST,     "East",     moduleAdress);
    this->compassConnectors[DIRECTION_SOUTH] =  new CompassConnector(PIN_COMPASS_SOUTH, DIRECTION_SOUTH,    "South",    moduleAdress);
    this->compassConnectors[DIRECTION_WEST] =   new CompassConnector(PIN_COMPASS_WEST,  DIRECTION_WEST,     "West",     moduleAdress);
    this->compassConnectors[DIRECTION_UP] =     new CompassConnector(PIN_COMPASS_UP,    DIRECTION_UP,       "Up",       moduleAdress);
    this->compassConnectors[DIRECTION_DOWN] =   new CompassConnector(PIN_COMPASS_DOWN,  DIRECTION_DOWN,     "Down",     moduleAdress);
}

bool ConnectorManager::tick() //Listens for pulses and handles them
{
    for (int i = 0; i < DIRECTIONS;  i++)
    {
        if (compassConnectors[i]->tick())
        {
            return true; //If a pulse was received, return true to start LED sync
        }
    }
    return false;
}


void ConnectorManager::connect() //Goes though all connectors one by one. Makes sure they are connected.
{
    Serial.println(F("Starting Connection Discovery"));
    for (int i = 0; i < DIRECTIONS; i++)
    {
        compassConnectors[i]->connect();
    }
    Serial.println(F("Connection Discovery finished"));
}

void ConnectorManager::sendSyncSignal() //Sends a sync signal to all connectors
{
    for (int i = 0; i < DIRECTIONS; i++)
    {
        compassConnectors[i]->transmit(NEIGH_ADRESS_MASTER);
    }
}