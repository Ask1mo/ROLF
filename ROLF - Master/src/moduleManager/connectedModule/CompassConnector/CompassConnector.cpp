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
