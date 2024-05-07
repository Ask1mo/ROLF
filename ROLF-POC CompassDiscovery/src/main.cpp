#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Askbutton/AskButton.h"



#define DISCOVERYMODE_IDLE 0
#define DISCOVERYMODE_GETTING 1
#define DISCOVERYMODE_INFORMING 2

#define COMPASSDIRECTIONS 6
#define COMPASS_NORTH 0
#define COMPASS_EAST  1
#define COMPASS_SOUTH 2
#define COMPASS_WEST  3
#define COMPASS_UP    4
#define COMPASS_DOWN  5

//Listening serial
SoftwareSerial *serialNorth;
SoftwareSerial *serialEast;
SoftwareSerial *serialSouth;
SoftwareSerial *serialWest;


AskButton *button;
uint8_t discoveryMode = DISCOVERYMODE_IDLE;

uint8_t compassRequestDirection = 0;


void setDiscoveryMode(uint8_t mode)
{
  discoveryMode = mode;

  switch (discoveryMode)
  {
    case DISCOVERYMODE_IDLE:
    {
      Serial.println(F("Discovery mode idle"));
      delete static_cast<SoftwareSerial*>(serialNorth);
      delete static_cast<SoftwareSerial*>(serialEast);
      delete static_cast<SoftwareSerial*>(serialSouth);
      delete static_cast<SoftwareSerial*>(serialWest);
      serialNorth = new SoftwareSerial(PIN_COMPASS_NORTH, PIN_DEAD);
      serialEast = new SoftwareSerial(PIN_COMPASS_EAST,   PIN_DEAD);
      serialSouth = new SoftwareSerial(PIN_COMPASS_SOUTH, PIN_DEAD);
      serialWest = new SoftwareSerial(PIN_COMPASS_WEST,   PIN_DEAD);
    }
    break;

    case DISCOVERYMODE_GETTING:
    {
      Serial.println(F("Discovery mode getting"));

      compassRequestDirection = COMPASS_NORTH;
    }
    break;

    case DISCOVERYMODE_INFORMING:
    {
      Serial.println(F("Discovery mode informing"));
    }
    break;
  }
}

void handleCompassRequestMode()
{
  switch (compassRequestDirection)
  {
    case COMPASS_NORTH:
    {
      Serial.println(F("Requesting from North"));
      delete static_cast<SoftwareSerial*>(serialNorth);
      serialNorth = new SoftwareSerial(PIN_DEAD, PIN_COMPASS_NORTH);
      serialNorth->begin(9600);

      while (true)
      {
        Serial.println(F("Sending N"));
        serialNorth->write("N");
      }
      

      serialNorth->end();
    }
    break;

    case COMPASS_EAST:
    {
      Serial.println(F("Requesting from East"));
      serialEast->begin(9600);
      serialEast->write("E");
      serialEast->end();
    }
    break;

    case COMPASS_SOUTH:
    {
      Serial.println(F("Requesting from South"));
      serialSouth->begin(9600);
      serialSouth->write("S");
      serialSouth->end();
    }
    break;

    case COMPASS_WEST:
    {
      Serial.print(F("Requesting from West"));
      serialWest->begin(9600);
      serialWest->write("W");
      serialWest->end();
    }
    break;
  }
}

void setup()
{
  Serial.begin(9600);
  button = new AskButton(PIN_BUTTON, 1000);

  setDiscoveryMode(DISCOVERYMODE_IDLE);
  Serial.println(F("Setup done"));
}

void loop()
{
  switch (discoveryMode)
  {
    case DISCOVERYMODE_IDLE:
      if (button->getCommand() == BUTTON_TAPPED) setDiscoveryMode(DISCOVERYMODE_GETTING);
    break;

    case DISCOVERYMODE_GETTING:
      handleCompassRequestMode();
    break;
    
    case DISCOVERYMODE_INFORMING:
      Serial.println(F("Informing devices"));
    break;
  }
}
