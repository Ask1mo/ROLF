#ifndef CONNECTEDMODULE_H
#define CONNECTEDMODULE_H

#include "setup.h"


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

  String neighborMac; //Adress of a connected neigbor (0 if no neighbor is disconnected)
  uint8_t neighborDirection; //Connected NEWSUD of a neigbor
};



class ConnectedModule
{
    private:
    String macAdress;
    BaseInfo baseInfo;
    CompassConnector compassConnectors[DIRECTIONS];
    bool puzzlePlaced;

    public:
    ConnectedModule(String macAdress, uint8_t temmplateID);
    String getMacAdress();
    void updateConnection(uint8_t direction, String neighborMac, uint8_t neighborDirection);

    bool getPuzzlePlaced();
    void setPuzzlePlaced(bool puzzlePlaced);
    BaseInfo getBaseInfo();
    bool checkHasNeighbor(String neighborMac);

    CompassConnector getConnectorFromCompensatedDirection(uint8_t compensatedDirection);
    CompassConnector getConnectorFromID(uint8_t neighborID);
    uint16_t getPipeDelayFromCompensatedDirection(uint8_t compensatedDirection);
    void rotate(uint8_t rotation);
    void printConnectors();

};



#endif