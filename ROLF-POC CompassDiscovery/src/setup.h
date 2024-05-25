#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include "pipePieces.h"

//Debug settings
#define DEBUGLEVEL DEBUG_DISABLED   //This is the level of debug messages you'll receive. Not all code supports this type of messaging.
//Debug presets, do not touch
#define DEBUG_DISABLED              0
#define DEBUG_ERRORS                1
#define DEBUG_WARNINGS              2
#define DEBUG_OPERATIONS            3
#define DEBUG_DAYISRUINED           5
//Debug settings

//Wifi & UDP
#define SSID "GLOWII"               //Wifi name
#define PASSWORD "AskimoGlow"       //Password,                                                                                                 //TODO: needs changing
#define SERVER_IP "192.168.137.246" //Should be Static IP of the master
//Wifi & UDP

//UART
#define BAUDRATE_MONITOR 115200    //Serial monitor baudrate
#define BAUDRATE_SYSTEM 9600       //Module to module communcation speed for the system                                                         //TODO: update to 115200
//UART

//Pinout
#define PIN_IDENT_NORTH   32        //These 6 pins communicate the orientation of the module to their neighbouring modules
#define PIN_IDENT_EAST    33
#define PIN_IDENT_SOUTH   25
#define PIN_IDENT_WEST    26
#define PIN_IDENT_UP      27
#define PIN_IDENT_DOWN    14

#define PIN_SYNC_NORTH   19         //These 6 pins communicate when to sync their LEDS
#define PIN_SYNC_EAST    23
#define PIN_SYNC_SOUTH   18
#define PIN_SYNC_WEST    5
#define PIN_SYNC_UP      17
#define PIN_SYNC_DOWN    16

#define PIN_DEAD            12      //A pin needed to keep the serial communication half-duplex. The unused pins are assigned to this pin.
#define PIN_BUTTON          13      //A debug button                                                                                            //TODO: Remove
#define PIN_LEDS            22      //The pin that controls the LED strip
//Pinout


//Pipe piecePresets
#define PRESET_NORMALX              //This is where you select the type of pipe you're using

//Preset storage                    //Presets are stored here.
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
#define PANELSETUP_VOICETUBE_PSV    //This is where you select the type of panel you're using. Old feature from Trinity
#define PLATFORM_ESP32_WROOMDEVKIT  //This is where you select the type of platform you're using. Old feature from Trinity
#define LEDTYPE_STANDARD            //This is where you select the type of LED strip you're using. Old feature from Trinity
#define ENABLE_DIODECONTROL true    //This is where you select if you want to use individual diode control. Old feature from Trinity
#define CUSTOMPALETTEAMOUNT 5       //This is where you select the amount of custom palette slots you want to use. Old feature from Trinity

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