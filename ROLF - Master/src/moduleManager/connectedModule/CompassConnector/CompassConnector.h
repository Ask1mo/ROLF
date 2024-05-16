#ifndef COMPASSCONNECTOR_H
#define COMPASSCONNECTOR_H

#include <vector>
#include "setup.h"
#include "SoftwareSerial.h"
#include "pipePieces.h"

#define NEIGH_CONNECTSTATE_UNKNOWN              0
#define NEIGH_CONNECTSTATE_DISCONNECTED         1
#define NEIGH_CONNECTSTATE_CONNECTED            2
#define NEIGH_CONNECTSTATE_BLOCKED              3


#define ADRESS_UNKNOWN 0

#define DIRECTIONS 6
#define DIRECTION_NONE  0
#define DIRECTION_NORTH 1
#define DIRECTION_EAST  2
#define DIRECTION_SOUTH 3
#define DIRECTION_WEST  4
#define DIRECTION_UP    5
#define DIRECTION_DOWN  6


class CompassConnector
{
    private:
    uint8_t compassDirection;
    uint8_t basePipe; 

    uint8_t connectionState;
    uint8_t neighborAdress;
    uint8_t neighborDirection;
    
    public:
    CompassConnector(uint8_t compassDirection, uint8_t basePipe);
    void updateConnection(uint8_t neighborAdress, uint8_t neighborDirection);
};



#endif