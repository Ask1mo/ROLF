#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "connectorManager/ConnectorManager.h"
#include "trinity/Trinity.h"
#include "comms/Comms.h"
#include "setup.h"


#define PULSELENGTH_ID 5


struct BaseInfo
{
  uint8_t id;
  uint8_t heartPiece;
  
  uint8_t northPipeLength;
  uint16_t northPipeDelay;
  
  uint8_t eastPipeLength;
  uint16_t eastPipeDelay;
  
  uint8_t southPipeLength;
  uint16_t southPipeDelay;
  
  uint8_t westPipeLength;
  uint16_t westPipeDelay;
  
  uint8_t upPipeLength;
  uint16_t upPipeDelay;
  
  uint8_t downPipeLength;
  uint16_t downPipeDelay;
};

// define two tasks for Blink & AnalogRead
TaskHandle_t Task1;
TaskHandle_t Task2;

ConnectorManager *connectorManager;
Trinity *trinity;
Comms comms;

void task_main( void *pvParameters );
void task_leds( void *pvParameters );




#endif