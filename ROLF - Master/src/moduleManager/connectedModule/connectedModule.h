#ifndef CONNECTEDMODULE_H
#define CONNECTEDMODULE_H

#include "setup.h"
#include "pipePieces.h"

#define DIRECTIONS 6
#define DIRECTION_NONE  0
#define DIRECTION_NORTH 1
#define DIRECTION_EAST  2
#define DIRECTION_SOUTH 3
#define DIRECTION_WEST  4
#define DIRECTION_UP    5
#define DIRECTION_DOWN  6

#define ADDR_NONE 0

#define ROTATION_0Deg 0
#define ROTATION_90Deg 1
#define ROTATION_180Deg 2
#define ROTATION_270Deg 3

String directionToString(uint8_t direction);

struct CompassConnector
{
  uint8_t compassDirection; //NEWSUD
  uint8_t basePipe; //The type of pipe that is connected to the base of the module. (Pipe length, female, end cap.)
  uint16_t pipeDelay; //The delay of the pipe

  uint8_t rotationCompensatedDirection;

  uint8_t neighborAdress; //Adress of a connected neigbor (0 if no neighbor is disconnected)
  uint8_t neighborDirection; //Connected NEWSUD of a neigbor
};

struct BaseInfo
{
  uint8_t id;
  uint8_t heartPiece;
  
  uint8_t northPipeLength;
  uint16_t northPipeDelay;
  
  uint8_t eastPipeLength;
  uint16_t eastPipeDelay;
  
  uint8_t southPipeLength;
  uint16_t southPipeDelay;
  
  uint8_t westPipeLength;
  uint16_t westPipeDelay;
  
  uint8_t upPipeLength;
  uint16_t upPipeDelay;
  
  uint8_t downPipeLength;
  uint16_t downPipeDelay;
};

class ConnectedModule
{
    private:
    String macAdress;
    String ipAdress;
    uint8_t moduleID;
    BaseInfo baseInfo;
    CompassConnector compassConnectors[DIRECTIONS];
    bool puzzlePlaced;

    public:
    ConnectedModule(String macAdress, String ipAdress , uint8_t moduleID, BaseInfo baseInfo);
    void updateIpAdress(String ipAdress);
    void setIpAdress(String ipAdress);
    uint8_t getModuleID();
    String getMacAdress();
    String getIpAdress();
    void updateConnection(uint8_t direction, uint8_t neighborID, uint8_t neighborDirection);

    bool getPuzzlePlaced();
    void setPuzzlePlaced(bool puzzlePlaced);
    BaseInfo getBaseInfo();
    bool checkHasNeighbor(uint8_t neighborID);
    //CompassConnector getConnectorInfo(uint8_t direction);
    CompassConnector getConnectorFromCompensatedDirection(uint8_t compensatedDirection);
    CompassConnector getConnectorFromID(uint8_t neighborID);
    uint16_t getPipeDelayFromCompensatedDirection(uint8_t compensatedDirection);
    void rotate(uint8_t rotation);
    void printConnectors();

};



#endif