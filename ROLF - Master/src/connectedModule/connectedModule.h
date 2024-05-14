#ifndef CONNECTEDMODULE_H
#define CONNECTEDMODULE_H

#include "setup.h"

#define DIRECTIONS 6
#define DIRECTION_NONE  0
#define DIRECTION_NORTH 1
#define DIRECTION_EAST  2
#define DIRECTION_SOUTH 3
#define DIRECTION_WEST  4
#define DIRECTION_UP    5
#define DIRECTION_DOWN  6

struct NeighborModule
{
    uint8_t moduleID;
    uint8_t connectedSide_Base;
    uint8_t connectedSide_Neighbor;
};

class ConnectedModule
{
    private:
    String macAdress;
    uint8_t moduleID;
    uint8_t shape;
    NeighborModule **neighborModules;

    public:
    ConnectedModule(String macAdress, uint8_t moduleID, uint8_t shape);
};



#endif