#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>

#define DEBUG_DISABLED              0
#define DEBUG_ERRORS                1
#define DEBUG_WARNINGS              2
#define DEBUG_OPERATIONS            3
#define DEBUG_DAYISRUINED           5
/*---=== Setting examples ===---*/
#define DEBUGLEVEL DEBUG_OPERATIONS

#define PULSELENGTH_ID 5
#define PULSELENGTH_SYNC 5
#define INTERVALLENGTH 1000
#define INTERVAL_MODULECHANGESCAN 10000

#define BAUDRATE_MONITOR 115200
#define BAUDRATE_SYSTEM 9600

#define SSID "GLOWII"
#define PASSWORD "AskimoGlow"
#define SERVER_UDPPORT 4210  // local port to listen on

/*
#define PIN_COMPASS_NORTH   2
#define PIN_COMPASS_EAST    3
#define PIN_COMPASS_SOUTH   4
#define PIN_COMPASS_WEST    5
#define PIN_COMPASS_UP      6
#define PIN_COMPASS_DOWN    7
#define PIN_DEAD            11
#define PIN_BUTTON          12
#define PIN_LED             13
*/

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
#define PIN_LED             22



#define MESSAGE_CLCO_NEWCLIENT          "NewCl"
#define MESSAGE_COCL_IDASSIGNMENT       "IDAss"

#define MESSAGE_CLCO_CONNECTIONCHANGED  "ConCh"
#define MESSAGE_COCL_UPDATEREQUEST      "UpReq"

#define MESSAGE_COCL_NEWEFFECT          "NewFX"

#define MESSAGE_DUPL_SESSIONCHECK       "SeChk"


#endif