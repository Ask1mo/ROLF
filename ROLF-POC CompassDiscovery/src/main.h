#ifndef MAIN_H
#define MAIN_H

//Libraries
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
//Files
#include "connectorManager/ConnectorManager.h"
#include "trinity/Trinity.h"
#include "setup.h"

//Defines
#define PULSELENGTH_ID 5

//Prototype struct
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

//Tasks
TaskHandle_t            Task1;
TaskHandle_t            Task2;
//Classes
ConnectorManager        *connectorManager;
Trinity                 *trinity;
//Variables
uint64_t                lastHornMillis = 0;
volatile bool           memoryReserved; //reservation to make sure memory is not accessed by multiple tasks at the same time
std::vector<LedUpdate>  ledUpdates_Shared; //Only when task_leds and task_main are not using the memory, transmisisons from buffer are put in here. This is to prevent memory corruption by multiple tasks accessing the same memory
    

void task_main( void *pvParameters );
void task_leds( void *pvParameters );

#endif