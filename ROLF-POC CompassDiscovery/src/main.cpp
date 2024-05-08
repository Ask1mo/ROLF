#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Askbutton/AskButton.h"
#include "connectorManager/ConnectorManager.h"

uint8_t moduleAdress = 0x01;

ConnectorManager *connectorManager;


AskButton *button;
uint8_t discoveryMode;

uint8_t compassRequestDirection = 0;

uint8_t ledLevel = 0;

uint64_t currentMillis = 0;


bool startFlashRegistered = false;
uint64_t lastdStartFlashMillis = 0;
#define STARTFLASH_DURATION 1000




void triggerLedSync()
{
  Serial.println(F("!!!!!!!!!LED Sync triggered!!!!!!!!"));
  connectorManager->sendSyncSignal();
  ledLevel = 255;
}

void setup()
{
  Serial.begin(9600);
  Serial.println(F("---===Setup started===---"));

  button = new AskButton(PIN_BUTTON, 1000);

  //Get new module adress. Can't be 0 or 1
  while (moduleAdress < 1) moduleAdress = analogRead(A0)/4; //0-1023 to 0-255
  
  //Prepare fake LEDS
  pinMode(PIN_LED, OUTPUT);
  Serial.println(F("Leds started"));

  //Connectors setup
  connectorManager = new ConnectorManager(&moduleAdress);
  Serial.println(F("ConnectionManager created"));
  connectorManager->connect();


  Serial.println(F("---===Setup done===---"));
}

void loop()
{
  //LED controls
  if (ledLevel > 0)
  {
    ledLevel--;
    analogWrite(PIN_LED, ledLevel);
  }

  //Connector comms
  if(connectorManager->tick()) triggerLedSync();
}
