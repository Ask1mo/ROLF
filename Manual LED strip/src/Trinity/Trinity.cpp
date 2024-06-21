#include "Trinity.h"



//Constructor
Trinity::Trinity(uint8_t ledPin, uint8_t buttonPin, uint8_t ldrPin, uint8_t maxFramerate)
{
  Serial.println(F("Trinity MK3 - Ask Blommaert"));

  if(DEBUGLEVEL >= DEBUG_OPERATIONS)
  {
    Serial.print(F("Creating Trinity at adress "));
    Serial.println((int)this, DEC);
  }

  ledManager        = new LedManager(); //Todo: Made LedManager accept const ints
  button            = new AskButton(buttonPin, 500);
  lightSensor       = new LightSensor(ldrPin);
  sleepTimer        = new SleepTimer();
  comms             = new Comms();

  setupComplete = false;

  

  setBrightnessMode(BRIGHTNESS_3_MAX);
  prevFrameMillis = 0;
  frameTime = 1000/maxFramerate;

  Serial.print(F("...Trinity Initialised with frameRate of "));
  Serial.print(maxFramerate);
  Serial.print(F(" and frameTime of "));
  Serial.println(frameTime);

  /*
  sleepTimer->setTurnOnEnabled(true);
  sleepTimer->setTurnOnTime(10,0);
  sleepTimer->setTurnOffEnabled(true);
  sleepTimer->setTurnOffTime(9,59);
  */

  Serial.println(F("...Trinity Started"));
}
void Trinity::addPanel(Panel *panel)
{
  ledManager->addPanel(panel);
}
void Trinity::begin()
{
  ledManager->begin();
  webServerManager  = new WebServerManager(ledManager->getPanelAmount(), ledManager->getCanvasWidth(), ledManager->getCanvasHeight());
  setupComplete = true;
  Serial.println(F("...Trinity Setup Finalised"));
}
//Private

//Public
void Trinity::tick()
{
  if (!setupComplete)
  {
    if (DEBUGLEVEL >= DEBUG_ERRORS)Serial.println(F("Trinity not setup yet"));
    return;
  }
  
  webServerManager->tick();

  //Frame pushing
  uint64_t currentMillis = millis();
  if(currentMillis >= (prevFrameMillis+frameTime))
  {
    prevFrameMillis = currentMillis;

    
    ledManager->tick();
    ledManager->print();
  }

  //Button press handling
  //TEMP REMOVED FOR GLOW
  
  //Auto brightness handling
  if (lightSensor->getEnabled())
  {
    lightSensor->tick();
    //if(ledManager->getGoalBrightness() != lightSensor->getRecommendedBrightness())
    ledManager->setGoalBrightness(lightSensor->getRecommendedBrightness());
  }

  //Waking up or Shutting down system from sleep timer
  sleepTimer->tick();
  switch (sleepTimer->getTurn())
  {
    case TURN_OFF:
    {
      Serial.println(F("SleepTimer Turning system off"));
      ledManager->setGoalBrightness(BRIGHTNESS_0_OFF);
    }
    break;

    case TURN_ON:
    {
      Serial.println(F("SleepTimer Turning system on"));
      if(ledManager->getGoalBrightness() == BRIGHTNESS_0_OFF)ledManager->setGoalBrightness(BRIGHTNESS_2_NOR);
    }
    break;
  }

  //Communications handling
  comms->tick();
  switch (comms->getReadyTransmissionType())
  {
    case TRANSMISSION_IN_LEDMANAGER:
    {
      Transmission_LedManager data = comms->getTransmission_LedManager();
      ledManager->setGoalBrightness(data.brightness); 
      ledManager->setSpeed(data.speed); 
    }
    break;
    case TRANSMISSION_IN_PANEL:
    {
      Transmission_Panel data = comms->getTransmission_Panel();
      ledManager->setPanelVfx(data.panelNumber, data.vfxData);
    }
    break;
    case TRANSMISSION_IN_DIODE:
    {
      Transmission_Diode data = comms->getTransmission_Diode();
      ledManager->setPanelDiodeVfx(data.panelNumber, data.diodeNumber, data.vfxData);
    }
    break;
    case TRANSMISSION_IN_CUSTOMPALETTES:
    {
      Transmission_CustomPalette data = comms->getTransmission_CustomPalette();
      for (uint8_t i = 0; i < AMOUNTOFCOLOURS; i++)
      {
        ledManager->setCustomPaletteColours(data.slot, i, *data.customRGB[i]);
      }
    }
    break;
    
    case TRANSMISSION_IN_SLEEPTIMER:
    {
      Transmission_SleepTimer data = comms->getTransmission_SleepTimer();
      switch (data.timerID)
      {
        case TIMERID_OFFTIMER:
        {
          sleepTimer->setTurnOffTime(data.hour, data.minute);
          sleepTimer->setTurnOffEnabled(data.enabled);
        }
        break;
        case TIMERID_ONTIMER:
        {
          sleepTimer->setTurnOnTime(data.hour, data.minute);
          sleepTimer->setTurnOnEnabled(data.enabled);
        }
        break;
      }
    }
    break;
    case TRANSMISSION_IN_LIGHTSENSOR:
    {
      lightSensor->setBrightnessOffset(comms->getTransmission_LightSensorOffset());
    }
    break;
    case TRANSMISSION_IN_REQUEST:
    {
      comms->transmit(ledManager->convertToTansmission());
      for (uint8_t i = 0; i < ledManager->getPanelAmount(); i++)
      {
        comms->transmit(ledManager->convertPanelToTransmission(i));
        #if ENABLE_DIODECONTROL
        for (uint8_t j = 0; j < ledManager->getPanelDiodeAmount(i); j++)
        {
          comms->transmit(ledManager->convertPanelDiodeToTransmission(i,j));
        }
        #endif
      }
    }
    break;
    case TRANSMISSION_IN_IDENT:
    {
      comms->transmit("IDENT");
    }
    break;
  }
}
void Trinity::forceTick(uint16_t ticks, bool keepPrinting, uint16_t delayTime)
{
  for (uint16_t i = 0; i < ticks; i++)
  {
    if(DEBUGLEVEL >=DEBUG_DAYISRUINED) Serial.print(F("T"));
    ledManager->tick();

    if (keepPrinting)
    {
      if(DEBUGLEVEL >=DEBUG_DAYISRUINED) Serial.print(("P"));
      ledManager->print();

      if(delayTime > 0 )
      {
        delay(delayTime);
      }
    }
  }
}


void Trinity::setSpeed(uint8_t speed)
{
  ledManager->setSpeed(speed);
}
void Trinity::setBrightnessMode(uint8_t brightnessMode)
{
  this->brightnessMode = brightnessMode;

  switch (brightnessMode)
  {
    case BRIGHTNESS_0_OFF:
    ledManager->setGoalBrightness(BRIGHTNESS_0_OFF_VAL);
    break;
    case BRIGHTNESS_1_DIM:
    ledManager->setGoalBrightness(BRIGHTNESS_1_DIM_VAL);
    break;
    case BRIGHTNESS_2_NOR:
    ledManager->setGoalBrightness(BRIGHTNESS_2_NOR_VAL);
    break;
    case BRIGHTNESS_3_MAX:
    ledManager->setGoalBrightness(BRIGHTNESS_3_MAX_VAL);
    break;
  }
}
uint16_t Trinity::getDiodeAmount()
{
  ledManager->getDiodeAmount();
}

void Trinity::setPanelVfx(uint8_t panelNumber, VFXData vfxData)
{
  ledManager->setPanelVfx(panelNumber, vfxData);
}
uint16_t Trinity::getPanelDiodeAmount(uint8_t panelNumber)
{
  return ledManager->getPanelDiodeAmount(panelNumber);
}
void Trinity::setPanelDiodeVfx(uint8_t panelNumber, uint16_t diodeNumber, VFXData vfxData)
{
  ledManager->setPanelDiodeVfx(panelNumber, diodeNumber, vfxData);
}

void Trinity::setCustomPaletteColours(uint8_t slot, uint8_t colourRGBNumber, ColourRGB colourRGB)
{
  ledManager->setCustomPaletteColours(slot, colourRGBNumber, colourRGB);
}
void Trinity::setCustomPaletteAvailableColours(uint8_t slot, uint8_t avalaibleColours)
{
  ledManager->setCustomPaletteAvailableColours(slot, avalaibleColours);
}

uint8_t Trinity::getPanelAmount()
{
  return ledManager->getPanelAmount();
}