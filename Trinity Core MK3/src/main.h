#ifndef MAIN_H
#define MAIN_H

#include "Trinity/Trinity.h"

#define NEXTEFFECTTIME 20000
#define ENABLECYCLING false

//#define ANIMATIONSET_STOCK
#define ANIMATIONSET_WESTPOINT

#define PIN_RELAY 33
#define PIN_BUTTON 25

Trinity *trinity;
AskButton *button;

uint64_t prevMillis;
uint8_t currentShowingEffect;
bool dayNight;

uint8_t colour;

#define LEDAMOUNT
float powerPercentage;

uint64_t lastPowerUpdate;
#define POWERUPDATEINTERVAL 10
#define POWERUPDATEVARIABLE 0.06 //0.12 0.06

uint32_t sensorHits;
uint32_t sensorPolls;
uint64_t prevSensorMillis;
#define SENSORPOLLTIME 250
#define SENSORTRIGGERPERCENTAGE 10



#endif