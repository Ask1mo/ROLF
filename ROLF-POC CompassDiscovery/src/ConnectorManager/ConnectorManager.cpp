#include "ConnectorManager.h"

ConnectorManager::ConnectorManager(uint8_t *moduleAdress)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating ConnectorManager at adress "));
        Serial.println((int)this, DEC);
    }

    this->compassConnectors = new CompassConnector*[DIRECTIONS];
    this->compassConnectors[0] = new CompassConnector(PIN_IDENT_NORTH,  PIN_SYNC_NORTH,   DIRECTION_NORTH,    "North",    moduleAdress);
    this->compassConnectors[1] = new CompassConnector(PIN_IDENT_EAST,   PIN_SYNC_EAST,    DIRECTION_EAST,     "East",     moduleAdress);
    this->compassConnectors[2] = new CompassConnector(PIN_IDENT_SOUTH,  PIN_SYNC_SOUTH,   DIRECTION_SOUTH,    "South",    moduleAdress);
    this->compassConnectors[3] = new CompassConnector(PIN_IDENT_WEST,   PIN_SYNC_WEST,    DIRECTION_WEST,     "West",     moduleAdress);
    this->compassConnectors[4] = new CompassConnector(PIN_IDENT_UP,     PIN_SYNC_UP,      DIRECTION_UP,       "Up",       moduleAdress);
    this->compassConnectors[5] = new CompassConnector(PIN_IDENT_DOWN,   PIN_SYNC_DOWN,    DIRECTION_DOWN,     "Down",     moduleAdress);
    directionsTurn = 0;
}

void ConnectorManager::tick()
{
    uint64_t currentMillis = millis();

    if (currentMillis - lastIdentPulseMillis > INTERVAL_SYNCPULSE)
    {
        lastIdentPulseMillis = currentMillis;
        compassConnectors[directionsTurn]->sendPulse_Ident();

        directionsTurn++;
        if (directionsTurn >= DIRECTIONS)
        {
            directionsTurn = 0;
            printConnectors();
        }
    }


    for (int i = 0; i < DIRECTIONS;  i++)
    {
        compassConnectors[i]->tick();
    }
}



void ConnectorManager::sendSyncSignal() //Sends a sync signal to all connectors
{
    for (int i = 0; i < DIRECTIONS; i++)
    {
        compassConnectors[i]->sendPulse_Sync();
    }
}

String ConnectorManager::getUpdateCode()
{
    String updateCode = "";
    for (int i = 0; i < DIRECTIONS; i++)
    {
        updateCode += compassConnectors[i]->getUpdateCode();
    }
    return updateCode;
}

void ConnectorManager::printConnectors()
{
    Serial.println(F("Printing connectors:"));
    for (int i = 0; i < DIRECTIONS; i++)
    {
        compassConnectors[i]->printConnector();
    }
    Serial.println();
}