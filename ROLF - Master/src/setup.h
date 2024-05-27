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


#define PRESET_1_DEBUGCROSS 1
#define PRESET_2_AllCross1 2





#define BAUDRATE_MONITOR 115200
#define SSID "GLOWII"
#define PASSWORD "AskimoGlow"
#define SERVER_UDPPORT 4210  // local port to listen on

#endif