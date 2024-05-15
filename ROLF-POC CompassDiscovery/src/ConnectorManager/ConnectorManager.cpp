#include "ConnectorManager.h"

ConnectorManager::ConnectorManager(uint8_t *moduleAdress)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating ConnectorManager at adress "));
        Serial.println((int)this, DEC);
    }

    this->compassConnectors = new CompassConnector*[DIRECTIONS];
    this->compassConnectors[0] = new CompassConnector(PIN_IDENT_NORTH,  PIN_SYNC_NORTH,   DIRECTION_NORTH,    moduleAdress);
    this->compassConnectors[1] = new CompassConnector(PIN_IDENT_EAST,   PIN_SYNC_EAST,    DIRECTION_EAST,     moduleAdress);
    this->compassConnectors[2] = new CompassConnector(PIN_IDENT_SOUTH,  PIN_SYNC_SOUTH,   DIRECTION_SOUTH,    moduleAdress);
    this->compassConnectors[3] = new CompassConnector(PIN_IDENT_WEST,   PIN_SYNC_WEST,    DIRECTION_WEST,     moduleAdress);
    this->compassConnectors[4] = new CompassConnector(PIN_IDENT_UP,     PIN_SYNC_UP,      DIRECTION_UP,       moduleAdress);
    this->compassConnectors[5] = new CompassConnector(PIN_IDENT_DOWN,   PIN_SYNC_DOWN,    DIRECTION_DOWN,     moduleAdress);
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
        if (directionsTurn >= DIRECTIONS)directionsTurn = 0;
    }


    for (int i = 0; i < DIRECTIONS;  i++)
    {
        compassConnectors[i]->tick();

        //Make sure no duplicates connections have been made
        for (int j = 0; j < DIRECTIONS; j++)
        {
            //If two connectors have the same neighbor, delete the oldest one. If any of the connectors have an unknown neighbor, ignore them
            if(compassConnectors[i]->getNeighborAdress() == compassConnectors[j]->getNeighborAdress() && i != j && compassConnectors[i]->getNeighborAdress() != ADRESS_UNKNOWN && compassConnectors[j]->getNeighborAdress() != ADRESS_UNKNOWN)
            {
                //If i is the newest, delete j. Else delete i
                if(compassConnectors[i]->getLastPulseTime() > compassConnectors[j]->getLastPulseTime()) compassConnectors[j]->forgetNeighbor();
                else compassConnectors[i]->forgetNeighbor();
                
            }
        }
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
        updateCode = compassConnectors[i]->getUpdateCode();
        if (updateCode != "")
        {
            printConnectors();
            return updateCode;
        }
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