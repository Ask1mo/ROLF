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

struct BaseInfo
{
  uint8_t heartPiece;
  uint8_t northPipe;
  uint8_t eastPipe;
  uint8_t southPipe;
  uint8_t westPipe;
  uint8_t upPipe;
  uint8_t downPipe;
};

class ConnectedModule
{
    private:
    String macAdress;
    String ipAdress;
    uint8_t moduleID;
    BaseInfo baseInfo;
    NeighborModule **neighborModules;

    public:
    ConnectedModule(String macAdress, String ipAdress , uint8_t moduleID, BaseInfo baseInfo);
    void updateIpAdress(String ipAdress);
    void setIpAdress(String ipAdress);
    uint8_t getModuleID();
    String getMacAdress();
    String getIpAdress();
};



#endif