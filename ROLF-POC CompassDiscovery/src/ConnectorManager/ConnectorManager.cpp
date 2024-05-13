#include "ConnectorManager.h"

ConnectorManager::ConnectorManager(uint8_t *moduleAdress)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating ConnectorManager at adress "));
        Serial.println((int)this, DEC);
    }

    this->compassConnectors = new CompassConnector*[DIRECTIONS];
    this->compassConnectors[0] = new CompassConnector(PIN_COMPASS_NORTH, DIRECTION_NORTH,    "North",    moduleAdress);
    this->compassConnectors[1] = new CompassConnector(PIN_COMPASS_EAST,  DIRECTION_EAST,     "East",     moduleAdress);
    this->compassConnectors[2] = new CompassConnector(PIN_COMPASS_SOUTH, DIRECTION_SOUTH,    "South",    moduleAdress);
    this->compassConnectors[3] = new CompassConnector(PIN_COMPASS_WEST,  DIRECTION_WEST,     "West",     moduleAdress);
    this->compassConnectors[4] = new CompassConnector(PIN_COMPASS_UP,    DIRECTION_UP,       "Up",       moduleAdress);
    this->compassConnectors[5] = new CompassConnector(PIN_COMPASS_DOWN,  DIRECTION_DOWN,     "Down",     moduleAdress);
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
        Serial.print(F("Connecting CompassConnector "));
        Serial.println(i);
        compassConnectors[i]->connect();
    }
    Serial.println(F("Connection Discovery finished"));
}

void ConnectorManager::sendSyncSignal() //Sends a sync signal to all connectors
{
    for (int i = 0; i < DIRECTIONS; i++)
    {
        compassConnectors[i]->transmit();
    }
}