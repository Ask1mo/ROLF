#include <Arduino.h>
#include <SoftwareSerial.h>
#include "connectorManager/ConnectorManager.h"
#include "trinity/Trinity.h"
#include "comms/Comms.h"
#include "setup.h"

// define two tasks for Blink & AnalogRead
TaskHandle_t Task1;
TaskHandle_t Task2;
void task_main( void *pvParameters );
void task_leds( void *pvParameters );


#define RESERVATION_NONE 0
#define RESERVATION_MAIN 1
#define RESERVATION_LEDS 2






ConnectorManager *connectorManager;
Trinity *trinity;
Comms comms;
uint64_t currentMillis = 0;




#define PULSELENGTH_ID 5
#define INTERVAL_SESSIONCHECK 60000         // 1 minute

#define SYSTEMSTATE_CONNECTING_WIFI       0
#define SYSTEMSTATE_CONNECTING_CONTROLLER 1
#define SYSTEMSTATE_CONNECTED             2



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
BaseInfo getBaseInfo(uint8_t presetID)
{
  switch (presetID)
  {
    case PRESET_1_DEBUGCROSS:
      return BaseInfo{PRESET_1_DEBUGCROSS,BASE_HEART_X,         2,2*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    0,0,            0,0};
    break;
    case PRESET_2_AllCross1:
      return BaseInfo{PRESET_2_AllCross1,BASE_HEART_XUPDOWN,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR};
    break;
  }
  Serial.println("Preset not found");
  return BaseInfo{0,0,0,0,0,0,0,0,0,0,0,0,0};
}






void task_main( void *pvParameters )
{
  //Setup
  Serial.println("Task Main started");

  comms.connect();
  connectorManager = new ConnectorManager(comms.getModuleID());
  Serial.println("Task Main setup complete");

  //Loop
  while (1)
  {
    //Serial.print("M");
    currentMillis = millis();
    /*
    if (currentMillis - lastMillis_SessionCheck > INTERVAL_SESSIONCHECK)
    {
      lastMillis_SessionCheck = currentMillis;
      udp_transmit(MESSAGE_DUPL_SESSIONCHECK+String(sessionID));
    }
    */

    if (memoryReservedBy == RESERVATION_NONE)
    {
      memoryReservedBy = RESERVATION_MAIN;

      for (uint8_t i = 0; i < ledUpdates_buffer.size(); i++)
      {
        ledUpdates.push_back(ledUpdates_buffer[i]);
        ledUpdates_buffer.erase(ledUpdates_buffer.begin() + i);
      }

      memoryReservedBy = RESERVATION_NONE;
    }
    
    

    String updateCode = connectorManager->getUpdateCode();
    if (updateCode != "") comms.transmit(MESSAGE_CLCO_CONNECTIONCHANGED+updateCode);

    connectorManager->tick();
    comms.tick();

    vTaskDelay(1);
  }
}
void task_leds( void *pvParameters )
{
  //Setup
  Serial.println("Task Leds started");

  BaseInfo baseInfo = getBaseInfo(SELECTEDPRESET);

  trinity = new Trinity(60);
  if (baseInfo.northPipeLength  > 0) trinity->addPanel(new Panel(DIRECTION_NORTH,0, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, baseInfo.northPipeDelay));
  if (baseInfo.eastPipeLength   > 0) trinity->addPanel(new Panel(DIRECTION_EAST, 0, 0, CLOCK_CLOCKWISE, COMPASS_EAST,  baseInfo.eastPipeDelay));
  if (baseInfo.southPipeLength  > 0) trinity->addPanel(new Panel(DIRECTION_SOUTH,0, 0, CLOCK_CLOCKWISE, COMPASS_SOUTH, baseInfo.southPipeDelay));
  if (baseInfo.westPipeLength   > 0) trinity->addPanel(new Panel(DIRECTION_WEST, 0, 0, CLOCK_CLOCKWISE, COMPASS_WEST,  baseInfo.westPipeDelay));
  if (baseInfo.upPipeLength     > 0) trinity->addPanel(new Panel(DIRECTION_UP,   0, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, baseInfo.upPipeDelay));
  if (baseInfo.downPipeLength   > 0) trinity->addPanel(new Panel(DIRECTION_DOWN, 0, 0, CLOCK_CLOCKWISE, COMPASS_SOUTH, baseInfo.downPipeDelay));
  trinity->begin();

  Serial.println("Task Leds setup complete");

  //Loop
  while (1)
  {
    //Serial.print("L");
    trinity->tick();

    if(memoryReservedBy == RESERVATION_NONE)
    {
      memoryReservedBy = RESERVATION_LEDS;

      for (uint8_t i = 0; i < ledUpdates.size(); i++)
      {
        uint8_t inputDirection = ledUpdates[i].inputPanel;
        uint8_t outputDirection = ledUpdates[i].outputPanel;
        uint8_t color = ledUpdates[i].colour;
        uint16_t delayOffset = ledUpdates[i].offset;
        ledUpdates.erase(ledUpdates.begin() + i);

        uint16_t inputDelay = delayOffset + trinity->getPanelDiodeAmount(inputDirection);
        for (uint16_t i = 0; i < trinity->getPanelDiodeAmount(inputDirection); i++)
        {
          trinity->setPanelDiodeVfx(inputDirection, i, VFXData{EFFECT_STOCK_FLASH, color, inputDelay, 1, false});
          inputDelay--;
        }

        uint16_t outputDelay = delayOffset + trinity->getPanelDiodeAmount(inputDirection);
        for (uint16_t i = 0; i < trinity->getPanelDiodeAmount(outputDirection); i++)
        {
          trinity->setPanelDiodeVfx(outputDirection, i, VFXData{EFFECT_STOCK_FLASH, color, outputDelay, 1, false});
          outputDelay++;
        }
      }

      memoryReservedBy = RESERVATION_NONE;
    }

    


    vTaskDelay(1);
  }
}

void setup()
{
  Serial.begin(BAUDRATE_MONITOR);
  Serial.println(F("---===Setup started===---"));

  xTaskCreatePinnedToCore
  (
    task_main,       /* Task function. */
    "task_main",         /* name of task. */
    10000,           /* Stack size of task */
    NULL,            /* parameter of the task */
    1,               /* priority of the task */
    &Task1,          /* Task handle to keep track of created task */
    0                /* pin task to core 0 */
  );              

  xTaskCreatePinnedToCore
  (
    task_leds,       /* Task function. */
    "task_leds",         /* name of task. */
    10000,           /* Stack size of task */
    NULL,            /* parameter of the task */
    1,               /* priority of the task */
    &Task2,          /* Task handle to keep track of created task */
    1               /* pin task to core 1 */
  ); 


  Serial.println(F("---===Setup finished===---"));
}
void loop()
{
  
}
  