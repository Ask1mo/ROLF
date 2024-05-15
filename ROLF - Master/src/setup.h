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






#define INTERVAL_MODULECHANGESCAN 10000
#define BAUDRATE_MONITOR 115200
#define SSID "GLOWII"
#define PASSWORD "AskimoGlow"
#define SERVER_UDPPORT 4210  // local port to listen on



#define MESSAGE_CLCO_NEWCLIENT          "NewCl"
#define MESSAGE_COCL_IDASSIGNMENT       "IDAss"

#define MESSAGE_CLCO_CONNECTIONCHANGED  "ConCh"
#define MESSAGE_COCL_UPDATEREQUEST      "UpReq"

#define MESSAGE_COCL_NEWEFFECT          "NewFX"

#define MESSAGE_DUPL_SESSIONCHECK       "SeChk"


#endif