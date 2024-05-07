#include "AskButton.h"

AskButton::AskButton(byte pin, uint32_t longPressDuration)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating Button at adress "));
        Serial.println((int)this, DEC);
    }

    this->pin = pin;
    this->longPressDuration = longPressDuration;

    pinMode(this->pin, INPUT_PULLUP);
    lastCommand = BUTTON_RELEASED;
}

byte AskButton::getCommand()
{
    state = digitalRead(pin);
    myMillis = millis();

    if (state != prevState)
    {
        if(state == LOW) //If button is pressed
        {
            pressStartTime = myMillis;
            prevState = state;
            return BUTTON_RELEASED;
        }
        else// If button is released
        {
            if (myMillis - pressStartTime <= longPressDuration)
            {
                prevState = state;
                lastActionTime = myMillis;
                //Serial.println(F("BTN TAP"));
                lastCommand = BUTTON_TAPPED;
                return BUTTON_TAPPED;
            }
            else
            {
                prevState = state;
                holdStateIsKnown = false;
                return BUTTON_RELEASED;
            }
            
        }
        
    }

    if (state == LOW) // If button is pressed
    {
        if (myMillis - pressStartTime >= longPressDuration)
        {
            if(holdStateIsKnown == false)
            {
                holdStateIsKnown = true;
                lastActionTime = myMillis;
                //Serial.println(F("BTN HLD"));
                return BUTTON_HELD;
            }
            else
            {
                lastActionTime = myMillis;
                return BUTTON_RELEASED;
            }
        }
    }

    return BUTTON_RELEASED;
}
uint8_t AskButton::getLastCommand(bool removeAfterRead)
{
    uint8_t commandToReturn = lastCommand;
    if (removeAfterRead)lastCommand = BUTTON_RELEASED;
    return commandToReturn;
}
unsigned long AskButton::getLastTimeDelta()
{
    return (myMillis - lastActionTime);
}

bool AskButton::getPressState()
{
    bool hardwareState = digitalRead(pin);

    //Serial.print(F("Button state: "));
    //Serial.println(hardwareState);
    
    if (hardwareState == LOW)
    {
        //Serial.println(F("Button pressed"));
        return BUTTON_PRESSSTATE_HIGH;
    }
    else
    {
        //Serial.println(F("Button released"));
        return BUTTON_RELEASED;
    }
}