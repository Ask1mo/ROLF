#ifndef DATATYPES_H
#define DATATYPES_H

#include <Arduino.h>

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

struct ModuleLedInfo_Output
{
    uint8_t moduleID;
    uint8_t inputDirection;
    uint8_t outputDirection;
    uint8_t color;
    uint16_t delayOffset;
    uint16_t delayMine;
    
};

struct ModuleChangeInfo
{
    uint8_t moduleID;
    uint8_t direction;
    uint8_t neighborID;
    uint8_t neighborDirection;
};

struct NewClientInfo
{
    String macAdress;
    String ipAdress;
    uint8_t templateID;
};

#endif