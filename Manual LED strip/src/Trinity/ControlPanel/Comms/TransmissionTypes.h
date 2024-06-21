
#ifndef TRANSMISSIONTYPES_H
#define TRANSMISSIONTYPES_H

#include <Arduino.h>
#include "Trinity/LedManager/effectProcessor/effectProcessor.h"

#define TRANSMISSION_IN_NONE                   0
#define TRANSMISSION_IN_REQUEST                1
#define TRANSMISSION_IN_IDENT                  2
#define TRANSMISSION_IN_LEDMANAGER             3
#define TRANSMISSION_IN_PANEL                  4
#define TRANSMISSION_IN_DIODE                  5
#define TRANSMISSION_IN_CUSTOMPALETTES         6
#define TRANSMISSION_IN_SLEEPTIMER             7
#define TRANSMISSION_IN_LIGHTSENSOR            8

/* these defines are never used, but it's good to keep track
#define TRANSMISSION_OUT_NONE       0
#define TRANSMISSION_OUT_IDENT      1
#define TRANSMISSION_OUT_LEDMANAGER 2
#define TRANSMISSION_OUT_PANEL      3
#define TRANSMISSION_OUT_DIODE      4
*/


struct Transmission_LedManager
{
    uint8_t brightness;
    uint8_t speed;
};
struct Transmission_Panel
{
    uint8_t panelNumber;
    VFXData vfxData;
};
struct Transmission_Diode
{
    uint8_t panelNumber;
    uint16_t diodeNumber;
    VFXData vfxData;
};
struct Transmission_CustomPalette
{
    uint8_t slot;
    ColourRGB *customRGB[AMOUNTOFCOLOURS];
};
struct Transmission_SleepTimer
{
    bool timerID;
    uint8_t hour;
    uint8_t minute;
    bool enabled;
};
struct Transmission_LightSensor
{
    uint16_t offset;
    bool enabled;
};

#endif