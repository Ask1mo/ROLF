#ifndef ASKBUTTON_H
#define AKSBUTTON_H

#include <Arduino.h>
#include "Trinity/setup.h"

#define BUTTON_RELEASED             0
#define BUTTON_TAPPED               1
#define BUTTON_HELD                 2
#define BUTTON_ERROR                3 
#define BUTTON_PRESSSTATE_HIGH      1 //This one is only for getPressState()

class AskButton
{
    private:
    byte pin;
    bool state;
    bool prevState;
    bool holdStateIsKnown;
    unsigned long myMillis;
    unsigned long pressStartTime;
    unsigned long lastActionTime;
    uint32_t longPressDuration;

    uint8_t lastCommand;

    public:
    AskButton(byte pin, uint32_t longPressDuration);
    byte getCommand();
    uint8_t getLastCommand(bool removeAfterRead);
    bool getPressState();
    unsigned long getLastTimeDelta();
};



#endif