#include "main.h"

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
void loop() //Not used, use task_main and task_leds instead
{
  
}
void task_main( void *pvParameters ) //Multicore replacement for "loop()"
{
  //Setup
  Serial.println("Task Main started");

  comms.connect();
  connectorManager = new ConnectorManager(comms.getModuleID());
  Serial.println("Task Main setup complete");
  
  //Loop
  while (1)
  {
    
    String updateCode = connectorManager->getUpdateCode();
    if (updateCode != "") comms.transmit(MESSAGE_CLCO_CONNECTIONCHANGED+updateCode);

    connectorManager->tick();
    comms.tick();
    
    vTaskDelay(1);
  }
}
void task_leds( void *pvParameters ) //Multicore replacement for led printings
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

  trinity->setSpeed(5);

  //Return LEDS to idle animation
  for (uint8_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelBrightness(i, 25, false); 
    trinity->setPanelVfx(i, VFXData{EFFECT_STOCK_STATIC, COLOUR_RED, random16(0, 500), 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, VFXData{EFFECT_STOCK_PAUSEDBREATHING, COLOUR_CYCLE, (uint16_t)(j*XFACTOR), 1, true});
    }
  }

  Serial.println("Task Leds setup complete");

  //Loop
  while (1)
  {
    trinity->tick();

    //Led updates
    std::vector<LedUpdate> ledUpdates = comms.getLedUpdates();
    if(ledUpdates.size() > 0)
    {
      for (uint8_t i = 0; i < ledUpdates.size(); i++)
      {
        uint8_t inputDirection = ledUpdates[i].inputPanel;
        uint8_t outputDirection = ledUpdates[i].outputPanel;
        uint8_t color = ledUpdates[i].colour;
        uint16_t delayOffset = ledUpdates[i].offset;

        ledUpdates.erase(ledUpdates.begin() + i);

        uint16_t inputDelay = delayOffset + trinity->getPanelDiodeAmount(inputDirection);
        trinity->setPanelBrightness(inputDirection, 255, false);
        for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(inputDirection); j++)
        {
          trinity->setPanelDiodeVfx(inputDirection, j, VFXData{EFFECT_STOCK_FLASH, color, inputDelay, 1, false});
          inputDelay-= XFACTOR;
        }
        uint16_t outputDelay = delayOffset + trinity->getPanelDiodeAmount(inputDirection);
        trinity->setPanelBrightness(outputDirection, 255, false);
        for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(outputDirection); j++)
        {
          trinity->setPanelDiodeVfx(outputDirection, j, VFXData{EFFECT_STOCK_FLASH, color, outputDelay, 1, false});
          outputDelay+= XFACTOR;
        }
      }
    }

    //Return LEDS to idle animation
    for (uint8_t i = 0; i < trinity->getPanelAmount(); i++)
    {
      if (trinity->getPanelEffectFinished(i))
      {
        Serial.print("!!! Panel effect finished: ");
        Serial.println(i);

        trinity->setPanelBrightness(i, 50, false);
        for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
        {
          trinity->setPanelDiodeVfx(i, j, VFXData{EFFECT_STOCK_PAUSEDBREATHING, COLOUR_CYCLE, (uint16_t)(j*XFACTOR), 1, true});
        }
      }
    }

    vTaskDelay(1);
  }
}