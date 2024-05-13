#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Askbutton/AskButton.h"
#include "connectorManager/ConnectorManager.h"

uint8_t moduleAdress = 0;

ConnectorManager *connectorManager;


AskButton *button;
uint8_t discoveryMode;

uint8_t compassRequestDirection = 0;

uint8_t ledLevel = 255;

uint64_t currentMillis = 0;


bool startFlashRegistered = false;
uint64_t lastdStartFlashMillis = 0;
#define STARTFLASH_DURATION 1000




void triggerLedSync()
{
  Serial.println(F("!!!!!!!!!LED Sync triggered!!!!!!!!"));
  delay(1000);
  connectorManager->sendSyncSignal();
  ledLevel = 255;
}

void setup()
{
  Serial.begin(BAUDRATE_MONITOR);
  Serial.println(F("---===Setup started===---"));

  button = new AskButton(PIN_BUTTON, 1000);

  //Get new module adress. Can't be 0 or 1
  while (moduleAdress <= 1) moduleAdress = analogRead(A0)/4; //0-1023 to 0-255
  Serial.print(F("Module adress: "));
  Serial.println(moduleAdress);
  
  //Prepare fake LEDS
  pinMode(PIN_LED, OUTPUT);
  Serial.println(F("Leds started"));

  //Connectors setup
  connectorManager = new ConnectorManager(&moduleAdress);
  Serial.println(F("ConnectionManager created"));

  uint32_t *memcheck;
  memcheck = (uint32_t *)malloc(1000);
  Serial.print(F("Memory check: "));
  Serial.println((uint32_t)memcheck);
  
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
    Serial.print(F("LED Level: "));
    Serial.println(ledLevel); 
  }

  //Connector comms
  if(connectorManager->tick()) triggerLedSync();
}
