#include "CompassConnector.h"


//Constructor
CompassConnector::CompassConnector(uint8_t compassDirection, uint8_t basePipe)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating CompassConnector at adress "));
        Serial.println((int)this, DEC);
    }

    this->compassDirection = compassDirection;
    this->basePipe = basePipe;
    this->connectionState = NEIGH_CONNECTSTATE_UNKNOWN;
    this->neighborAdress = ADRESS_UNKNOWN;
    this->neighborDirection = DIRECTION_NONE;
}

//Private

//Public
void CompassConnector::updateConnection(uint8_t neighborAdress, uint8_t neighborDirection)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Updated connection to adress "));
        Serial.print(neighborAdress);
        Serial.print(F(" in direction "));
        Serial.println(neighborDirection);
    }

    this->neighborAdress = neighborAdress;
    this->neighborDirection = neighborDirection;

    if (neighborAdress == ADRESS_UNKNOWN && neighborDirection == DIRECTION_NONE)
    {
        this->connectionState = NEIGH_CONNECTSTATE_DISCONNECTED;
    }
    else if (neighborAdress == ADRESS_UNKNOWN || neighborDirection == DIRECTION_NONE)
    {   
        this->connectionState = NEIGH_CONNECTSTATE_UNKNOWN;
    }
    else
    {
        this->connectionState = NEIGH_CONNECTSTATE_CONNECTED;
    }
}