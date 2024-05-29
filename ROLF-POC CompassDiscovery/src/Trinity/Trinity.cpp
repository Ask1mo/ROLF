#include "Trinity.h"



//Constructor
Trinity::Trinity(uint8_t maxFramerate)
{
  Serial.println(F("Trinity MK3 - Ask Blommaert"));

  if(DEBUGLEVEL >= DEBUG_OPERATIONS)
  {
    Serial.print(F("Creating Trinity at adress "));
    Serial.println((int)this, DEC);
  }

  ledManager        = new LedManager(); //Todo: Made LedManager accept const ints

  setupComplete = false;

  

  setBrightnessMode(BRIGHTNESS_3_MAX);
  prevFrameMillis = 0;
  frameTime = 1000/maxFramerate;

  Serial.print(F("...Trinity Initialised with frameRate of "));
  Serial.print(maxFramerate);
  Serial.print(F(" and frameTime of "));
  Serial.println(frameTime);

  Serial.println(F("...Trinity Started"));
}
void Trinity::addPanel(Panel *panel)
{
  ledManager->addPanel(panel);
}
void Trinity::begin()
{
  ledManager->begin();
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
  

  //Frame pushing
  uint64_t currentMillis = millis();
  if(currentMillis >= (prevFrameMillis+frameTime))
  {
    prevFrameMillis = currentMillis;

    
    ledManager->tick();
    ledManager->print();
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
  return ledManager->getDiodeAmount();
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
void Trinity::setPanelBrightness(uint8_t panelNumber, uint8_t brightness, bool smoothEnabled)
{
  ledManager->setPanelBrightness(panelNumber, brightness, smoothEnabled);
}

bool Trinity::getPanelEffectFinished(uint8_t panelNumber)
{
  return ledManager->getPanelEffectFinished(panelNumber);
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

void Trinity::toggleFilter()
{
  ledManager->toggleFilter();
}