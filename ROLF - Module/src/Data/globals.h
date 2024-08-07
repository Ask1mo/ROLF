#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "pipePieces.h"

//Global Defines
#define DIRECTIONS 6
#define DIRECTION_NONE  0
#define DIRECTION_NORTH 1
#define DIRECTION_EAST  2
#define DIRECTION_SOUTH 3
#define DIRECTION_WEST  4
#define DIRECTION_UP    5
#define DIRECTION_DOWN  6

#define PRESET_1_DEBUGCROSS         1
#define PRESET_2_AllCross1          2
#define PRESET_3_STRIJP_SINGLEPIPE  3
#define PRESET_253_STRIJP_HORNWEIRD 253
#define PRESET_254_STRIJP_HORNLONG  254
#define PRESET_255_Horn             255

#define XFACTOR 20 //Standardised delay factor for standard length pipe pieces //Obsolete?

//Global Structs
struct Transmission
{
    //Message identificaiton.
    String goalMac;         //Mac adress of the module that needs to receive this message. "MASTER" for master
    uint8_t messageID;      //Unique ID for this message, so that no messages are retransmitted twice.
    String messageType;     //Type of message that is contained in "message"
    //Message content
    String message;
    //Local
    uint8_t connectorID;    //Directional Pin/port on which this message was received
};
struct BaseInfo //Information about what setup this module is using
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
struct LedUpdate
{
  uint8_t inputPanel;
  uint8_t outputPanel;
  uint8_t colour;
  uint16_t offset;
};

//Global Functions

BaseInfo getBaseInfo(uint8_t presetID);
String directionToString(uint8_t compassDirection);
String colonMacToSixCharMac(String colonMac);
String sixCharMacToColonMac(String sixCharMac);
void printStringAsHex(String data);

#endif