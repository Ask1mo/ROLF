#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "Data/pipePieces.h"

//Global Defines
#define PRESET_1_DEBUGCROSS         1
#define PRESET_2_AllCross1          2
#define PRESET_3_STRIJP_SINGLEPIPE  3
#define PRESET_253_STRIJP_HORNWEIRD 253
#define PRESET_254_STRIJP_HORNLONG  254
#define PRESET_255_Horn             255

#define DIRECTIONS 6
#define DIRECTION_NONE  0
#define DIRECTION_NORTH 1
#define DIRECTION_EAST  2
#define DIRECTION_SOUTH 3
#define DIRECTION_WEST  4
#define DIRECTION_UP    5
#define DIRECTION_DOWN  6

#define XFACTOR 5 //Obsolete?

//Global Structs
struct predefinedModulePlacement
{
  uint8_t moduleID; //Still required?
  String macAdress;
  uint8_t rotation;
  uint8_t x;
  uint8_t y;
  uint8_t z;
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
struct TransmissionData_LedInstruction
{
    String  macAdress; //Do not use?
    uint8_t inputDirection;
    uint8_t outputDirection;
    uint8_t color;
    uint16_t delayOffset;
    uint16_t delayMine;
    
};
struct TransmissionData_NeighborConnection
{
    String  macAdress;
    uint8_t direction;
    String  neighborMac;
    uint8_t neighborDirection;
};
struct Transmission_NewClientInfo
{
    String macAdress;
    String ipAdress;
    uint8_t templateID;
};

//Global Functions
BaseInfo getBaseInfo(uint8_t presetID);
String directionToString(uint8_t direction);


#endif