#ifndef SETUP_H
#define SETUP_H

#include "Data/globals.h"

#define DEBUG_DISABLED              0
#define DEBUG_ERRORS                1
#define DEBUG_WARNINGS              2
#define DEBUG_OPERATIONS            3
#define DEBUG_DAYISRUINED           5
/*---=== Setting examples ===---*/
#define DEBUGLEVEL DEBUG_OPERATIONS


#define BAUDRATE_MONITOR 115200
#define BAUDRATE_SYSTEM 9600       //Module to module communcation speed for the system                                                         //TODO: update to 115200

#define SSID "GLOWII"
#define PASSWORD "AskimoGlow"

#define PIN_COMMS 32
#define PIN_LEDSYNC 19

#define PIN_DEAD            12      //A pin needed to keep the serial communication half-duplex. The unused pins are assigned to this pin.

#endif