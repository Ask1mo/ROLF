#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Askbutton/AskButton.h"



#define DISCOVERYMODE_IDLE        0 //Nothing
#define DISCOVERYMODE_GETTING     1 //(Commanded from master) Getting data from neighbors 
#define DISCOVERYMODE_RESPONDING  2 //(Commanded from neighbor) Responding to neighbor
#define DISCOVERYMODE_INFORMING   3 //Informing other devices that I'm new
#define DISCOVERYMODE_LISTENING   4 //Listening to response from neigbor

#define COMPASSDIRECTIONS 6
#define COMPASS_NONE  0
#define COMPASS_NORTH 1
#define COMPASS_EAST  2
#define COMPASS_SOUTH 3
#define COMPASS_WEST  4
#define COMPASS_UP    5
#define COMPASS_DOWN  6

uint8_t neigbor_north = 0;
uint8_t neigbor_east  = 0;
uint8_t neigbor_south = 0;
uint8_t neigbor_west  = 0;
uint8_t neigbor_up    = 0;
uint8_t neigbor_down  = 0;


uint8_t moduleAdress = 0x01;
//Listening serial
SoftwareSerial *softwareSerial;

AskButton *button;
uint8_t discoveryMode;

uint8_t compassRequestDirection = 0;

uint8_t ledLevel = 0;

uint64_t currentMillis = 0;

uint8_t lastCompassDirection = 0;

bool startFlashRegistered = false;
uint64_t lastdStartFlashMillis = 0;
#define STARTFLASH_DURATION 1000


void setDiscoveryMode(uint8_t mode)
{
  discoveryMode = mode;

  switch (discoveryMode)
  {
    case DISCOVERYMODE_IDLE:
    {
      Serial.println(F("Discovery mode idle"));
    }
    break;

    case DISCOVERYMODE_GETTING:
    {
      Serial.println(F("Discovery mode getting"));

      compassRequestDirection = COMPASS_NORTH;
    }
    break;

    case DISCOVERYMODE_RESPONDING:
    {
      Serial.println(F("Responding, oriigin: "));
      Serial.println(lastCompassDirection);

      softwareSerial = new SoftwareSerial(PIN_DEAD, lastCompassDirection+1); //+1 because the pins are 2-7. Converts from direction to pin
      softwareSerial->begin(9600);
  
      switch (lastCompassDirection)
      {
        case COMPASS_NORTH:
          Serial.println(F("Responding North"));
          softwareSerial->write("RN");
        break;

        case COMPASS_EAST:
          Serial.println(F("Responding East"));
          softwareSerial->write("RE");
        break;

        case COMPASS_SOUTH:
          Serial.println(F("Responding South"));
          softwareSerial->write("RS");
        break;

        case COMPASS_WEST:
          Serial.println(F("Responding West"));
          softwareSerial->write("RW");
        break;

        case COMPASS_UP:
          Serial.println(F("Responding Up"));
          softwareSerial->write("RU");
        break;

        case COMPASS_DOWN:
          Serial.println(F("Responding Down"));
          softwareSerial->write("RD");
        break;
      }

      softwareSerial->write(moduleAdress);
      softwareSerial->end();
      delete static_cast<SoftwareSerial*>(softwareSerial);

      pinMode(PIN_COMPASS_NORTH,  INPUT_PULLUP);
      pinMode(PIN_COMPASS_EAST,   INPUT_PULLUP);
      pinMode(PIN_COMPASS_SOUTH,  INPUT_PULLUP);
      pinMode(PIN_COMPASS_WEST,   INPUT_PULLUP);
      pinMode(PIN_COMPASS_UP,     INPUT_PULLUP);
      pinMode(PIN_COMPASS_DOWN,   INPUT_PULLUP);
    }

    case DISCOVERYMODE_INFORMING:
    {
      Serial.println(F("Discovery mode informing"));
      pinMode(PIN_COMPASS_NORTH,  OUTPUT);
      pinMode(PIN_COMPASS_EAST,   OUTPUT);
      pinMode(PIN_COMPASS_SOUTH,  OUTPUT);
      pinMode(PIN_COMPASS_WEST,   OUTPUT);
      pinMode(PIN_COMPASS_UP,     OUTPUT);
      pinMode(PIN_COMPASS_DOWN,   OUTPUT);
      digitalWrite(PIN_COMPASS_NORTH, LOW);
      digitalWrite(PIN_COMPASS_EAST, LOW);
      digitalWrite(PIN_COMPASS_SOUTH, LOW);
      digitalWrite(PIN_COMPASS_WEST, LOW);
      digitalWrite(PIN_COMPASS_UP, LOW);
      digitalWrite(PIN_COMPASS_DOWN, LOW);
      delay(100);
      setDiscoveryMode(DISCOVERYMODE_LISTENING);
    }
    break;

    case DISCOVERYMODE_LISTENING:
    {
      Serial.println(F("Discovery mode listening"));
      softwareSerial = new SoftwareSerial(PIN_DEAD, lastCompassDirection+1); //+1 because the pins are 2-7. Converts from direction to pin
      softwareSerial->begin(9600);
      while (softwareSerial->available() > 0)
      {
        char c = softwareSerial->read();
        Serial.print(c);
        delay(10);
      }
      
      softwareSerial->end();
      delete static_cast<SoftwareSerial*>(softwareSerial);

      setDiscoveryMode(DISCOVERYMODE_IDLE);
    }
  }
}

void handleMode_CompassRequestMode()
{
  softwareSerial = new SoftwareSerial(PIN_DEAD, compassRequestDirection+1); //+1 because the pins are 2-7. Converts from direction to pin
  softwareSerial->begin(9600);

  switch (compassRequestDirection)
  {
    case COMPASS_NORTH:
    {
      Serial.println(F("Requesting from North"));
      softwareSerial->write("N");
    }
    break;

    case COMPASS_EAST:
    {
      Serial.println(F("Requesting from East"));
      softwareSerial->write("E");
    }
    break;

    case COMPASS_SOUTH:
    {
      Serial.println(F("Requesting from South"));
      softwareSerial->write("S");
    }
    break;

    case COMPASS_WEST:
    {
      Serial.print(F("Requesting from West"));
      softwareSerial->write("W");
    }
    break;

    case COMPASS_UP:
    {
      Serial.println(F("Requesting from Up"));
      softwareSerial->write("U");
    }
    break;

    case COMPASS_DOWN:
    {
      Serial.println(F("Requesting from Down"));
      softwareSerial->write("D");
    }
    break;
  }

  softwareSerial->end();
  delete static_cast<SoftwareSerial*>(softwareSerial);
}


void sendStartFlash()
{
  if(neigbor_north  != 0) digitalWrite(neigbor_north, LOW);
  if(neigbor_east   != 0) digitalWrite(neigbor_east, LOW);
  if(neigbor_south  != 0) digitalWrite(neigbor_south, LOW);
  if(neigbor_west   != 0) digitalWrite(neigbor_west, LOW);
  if(neigbor_up     != 0) digitalWrite(neigbor_up, LOW);
  if(neigbor_down   != 0) digitalWrite(neigbor_down, LOW);
}
void registerStartFlash()
{
  startFlashRegistered = true;
  ledLevel = 255;
  lastdStartFlashMillis = currentMillis;
}


bool readAllCompass() //Returns true if any compass is LOW. False if ALL are HIGH
{
  return (!digitalRead(PIN_COMPASS_NORTH) || !digitalRead(PIN_COMPASS_EAST) || !digitalRead(PIN_COMPASS_SOUTH) || !digitalRead(PIN_COMPASS_WEST) || !digitalRead(PIN_COMPASS_UP) || !digitalRead(PIN_COMPASS_DOWN));
}
bool readConnectedCompass() //Returns true if any compass is LOW. False if ALL are HIGH. But does so only from compasses that are known to be connected from a neighbor
{
  if(neigbor_north  != 0 && !digitalRead(PIN_COMPASS_NORTH))  return true;
  if(neigbor_east   != 0 && !digitalRead(PIN_COMPASS_EAST))   return true;
  if(neigbor_south  != 0 && !digitalRead(PIN_COMPASS_SOUTH))  return true;
  if(neigbor_west   != 0 && !digitalRead(PIN_COMPASS_WEST))   return true;
  if(neigbor_up     != 0 && !digitalRead(PIN_COMPASS_UP))     return true;
  if(neigbor_down   != 0 && !digitalRead(PIN_COMPASS_DOWN))   return true;
  return false;
}
bool readUnconnectedCompass() //Returns true if any compass is LOW. False if ALL are HIGH. But does so only from compasses that are known to be unconnected from a neighbor
{
  if(neigbor_north  == 0 && !digitalRead(PIN_COMPASS_NORTH))  return true;
  if(neigbor_east   == 0 && !digitalRead(PIN_COMPASS_EAST))   return true;
  if(neigbor_south  == 0 && !digitalRead(PIN_COMPASS_SOUTH))  return true;
  if(neigbor_west   == 0 && !digitalRead(PIN_COMPASS_WEST))   return true;
  if(neigbor_up     == 0 && !digitalRead(PIN_COMPASS_UP))     return true;
  if(neigbor_down   == 0 && !digitalRead(PIN_COMPASS_DOWN))   return true;
  return false;
}

uint8_t getCompassDirection()
{
  if (!digitalRead(PIN_COMPASS_NORTH)) return COMPASS_NORTH;
  if (!digitalRead(PIN_COMPASS_EAST)) return COMPASS_EAST;
  if (!digitalRead(PIN_COMPASS_SOUTH)) return COMPASS_SOUTH;
  if (!digitalRead(PIN_COMPASS_WEST)) return COMPASS_WEST;
  if (!digitalRead(PIN_COMPASS_UP)) return COMPASS_UP;
  if (!digitalRead(PIN_COMPASS_DOWN)) return COMPASS_DOWN;
  return COMPASS_NONE;
}

void setup()
{
  Serial.begin(9600);
  button = new AskButton(PIN_BUTTON, 1000);

  //Get new module adress. Can't be 0 or 1
  while (moduleAdress < 1) moduleAdress = analogRead(A0)/4; //0-1023 to 0-255
  
  

  setDiscoveryMode(DISCOVERYMODE_INFORMING);
  Serial.println(F("Setup done"));
}

void loop()
{
  currentMillis = millis();
  //LED controls
  if (ledLevel > 0)
  {
    ledLevel--;
    analogWrite(PIN_LED, ledLevel);
  }

  //Discovery mode
  switch (discoveryMode)
  {
    case DISCOVERYMODE_IDLE:
    {
      if (button->getCommand() == BUTTON_TAPPED) setDiscoveryMode(DISCOVERYMODE_GETTING);

      if (startFlashRegistered)
      {
        /*
        if (millis() - lastdStartFlashMillis > STARTFLASH_DURATION)
        {
          setDiscoveryMode(DISCOVERYMODE_RESPONDING);
        }
        */
      }
      else
      {
        if (readConnectedCompass()) registerStartFlash();
        if (readUnconnectedCompass()) setDiscoveryMode(DISCOVERYMODE_RESPONDING);
      }
    }
    break;

    case DISCOVERYMODE_GETTING:
      handleMode_CompassRequestMode();
    break;
   
    case DISCOVERYMODE_INFORMING:
      Serial.println(F("Informing devices I'm new"));
    break;
  }
}
