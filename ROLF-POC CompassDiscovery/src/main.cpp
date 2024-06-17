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
    
    case PRESET_3_STRIJP_SINGLEPIPE:
      return BaseInfo{PRESET_3_STRIJP_SINGLEPIPE,BASE_HEART_X,  1,28,           0,0,            0,0,            0,0,            0,0,            0,0};
    break;
    case PRESET_253_STRIJP_HORNWEIRD:
      return BaseInfo{PRESET_253_STRIJP_HORNWEIRD,BASE_HEART_X, 2,24,           0,0,            0,0,            0,0,            0,0,            0,0};
    break;
    case PRESET_254_STRIJP_HORNLONG:
      return BaseInfo{PRESET_254_STRIJP_HORNLONG,BASE_HEART_X,  2,43,           0,0,            0,0,            0,0,            0,0,            0,0};
    case PRESET_255_Horn:
      return BaseInfo{PRESET_255_Horn,0b10000000,    1,1*XFACTOR,    0,0,    0,0,    0,0,    0,0,    0,0};
    break;
  }
  Serial.println("Preset not found");
  return BaseInfo{0,0,0,0,0,0,0,0,0,0,0,0,0};
}


void setup()
{
  Serial.begin(BAUDRATE_MONITOR);
  Serial.println(F("----- ===== Setup started ===== -----"));
  
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
}
void loop() //Not used, use task_main and task_leds instead
{
}
void task_main( void *pvParameters ) //Multicore replacement for "loop()"
{
  //Setup
  Serial.println("----- ===== Task Main Setup Started ===== -----");

  //Horn microphone //TODO: REMOVE OR REPLACE
  pinMode(PIN_MICROPHONE, INPUT);

  //Wifi
  WiFi.begin(WIFI_SSID, PASSWORD);
  Serial.print("Attempting connection to ");
  Serial.println(WIFI_SSID);

  //OTA
  String hostname = "ESP_" + colonMacToSixCharMac(WiFi.macAddress());
  ArduinoOTA.setHostname(hostname.c_str()); // Set the hostname
  ArduinoOTA.begin(); // Initialize OTA
  Serial.println("OTA hostname: ");
  Serial.println(hostname);

  //Comms
  uint8_t sixByteMacArray[6];
  WiFi.macAddress(sixByteMacArray);
  String sixByteMacString = "";
  for (uint8_t i = 0; i < 6; i++)
  {
    sixByteMacString += (char)sixByteMacArray[i];
  }
  Serial.print("6 byte Mac adress string: ");
  Serial.println(sixByteMacString);

  String dingus = sixCharMacToColonMac(sixByteMacString);
  

  Serial.println("Beepus");
  connectorManager = new ConnectorManager(sixByteMacString); //Use shortneded macadress

  //Setup end
  Serial.println("----- ===== Task Main Setup Complete ===== -----");
  
  //Loop
  while (1)
  {
    //OTA
    ArduinoOTA.handle();

    //Horn microphone //TODO: REMOVE OR REPLACE
    if (SELECTEDPRESET == PRESET_255_Horn || SELECTEDPRESET == PRESET_254_STRIJP_HORNLONG || SELECTEDPRESET == PRESET_253_STRIJP_HORNWEIRD)
    {
      if (millis() - lastHornMillis > 1000)
      {
        int val = analogRead(PIN_MICROPHONE);
        if (val > 4000)
        {
          Serial.println(F("HORN TRIGGER"));
          lastHornMillis = millis();
          connectorManager->transmit(MESSAGETYPE_HOCO_HORNTRIGGERED);
        }
      }
    }

    //Putting led updates from comms into shared memory
    if (!memoryReserved) //If memory is not reserved by task_leds
    {
      memoryReserved = true;
      std::vector<LedUpdate> newLedUpdates = connectorManager->getLedUpdates();
      for (uint8_t i = 0; i < newLedUpdates.size(); i++)
      {
        ledUpdates_Shared.push_back(newLedUpdates[i]);
      }
      memoryReserved = false;
    }

    
    connectorManager->tick();

    vTaskDelay(1);
  }
}
void task_leds( void *pvParameters ) //Multicore replacement for led printings
{
  //Setup
  Serial.println("----- ===== Task Leds Setup Started ===== -----");

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

  Serial.println("----- ===== Task Leds Setup Complete ===== -----");

  //Loop
  while (1)
  {
    trinity->tick();

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


    //Led updates
    if (!memoryReserved)
    {
      memoryReserved = true;
      if(ledUpdates_Shared.size() > 0)
      {
        for (uint8_t i = 0; i < ledUpdates_Shared.size(); i++)
        {
          uint8_t inputDirection = ledUpdates_Shared[0].inputPanel;
          uint8_t outputDirection = ledUpdates_Shared[0].outputPanel;
          uint8_t color = ledUpdates_Shared[0].colour;
          uint16_t delayOffset = ledUpdates_Shared[0].offset;

          ledUpdates_Shared.erase(ledUpdates_Shared.begin());

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
      memoryReserved = false;
    }

    

    vTaskDelay(1);
  }
}