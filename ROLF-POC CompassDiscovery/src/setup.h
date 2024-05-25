#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include "pipePieces.h"

//Debug settings
#define DEBUG_DISABLED              0
#define DEBUG_ERRORS                1
#define DEBUG_WARNINGS              2
#define DEBUG_OPERATIONS            3
#define DEBUG_DAYISRUINED           5
/*---=== Setting examples ===---*/
#define DEBUGLEVEL DEBUG_DISABLED
//Debug settings

//Wifi & UDP
#define SSID "GLOWII"
#define PASSWORD "AskimoGlow"
#define SERVER_IP "192.168.137.246"
#define SERVER_UDPPORT 4210  // local port to listen on
//Wifi & UDP

//UART
#define BAUDRATE_MONITOR 115200
#define BAUDRATE_SYSTEM 9600
//UART

//Pinout
#define PIN_IDENT_NORTH   32
#define PIN_IDENT_EAST    33
#define PIN_IDENT_SOUTH   25
#define PIN_IDENT_WEST    26
#define PIN_IDENT_UP      27
#define PIN_IDENT_DOWN    14

#define PIN_SYNC_NORTH   19
#define PIN_SYNC_EAST    23
#define PIN_SYNC_SOUTH   18
#define PIN_SYNC_WEST    5
#define PIN_SYNC_UP      17
#define PIN_SYNC_DOWN    16

#define PIN_DEAD            12
#define PIN_BUTTON          13
#define PIN_LEDS            22
//Pinout


//Pipe piecePresets
#define PRESET_NORMALX

#ifdef PRESET_NORMALX
    #define BASE_HEARTPIECE BASE_HEART_X 
    #define BASE_NORTHPIPE    2
    #define BASE_EASTPIPE     1
    #define BASE_SOUTHPIPE    1
    #define BASE_WESTPIPE     1
    #define BASE_UPPIPE       0
    #define BASE_DOWNPIPE     0
#endif  
//Pipe piecePresets

/*---=== Trinity setup ===---*/
#define PANELSETUP_VOICETUBE_PSV
#define PLATFORM_ESP32_WROOMDEVKIT
#define LEDTYPE_STANDARD
#define LINKPROGRAM_MK2
#define ENABLE_DIODECONTROL true
#define CUSTOMPALETTEAMOUNT 5

//Strip Setup
#ifdef LEDTYPE_STANDARD
    #define LEDCOLORDER GRB
#endif
#ifdef LEDTYPE_CHRISTMASSTRING
    #define LEDCOLORDER RGB
#endif
//Strip Setup
/*---=== Trinity setup ===---*/

#endif