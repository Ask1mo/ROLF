#include "Diode.h"

//Constructor
Diode::Diode(uint16_t number, uint8_t *panelEffect)
{
  this->number = number;

  this->goalBrightness  = 255;
  this->brightness     = 0;
  this->effect      = panelEffect;
  this->colour      = COLOUR_BLACK;
  this->offset      = 0;
  this->speed       = 1;

    this->effectVariables.r                   = 0;
  this->effectVariables.g                   = 0;
  this->effectVariables.b                   = 0;


  resetFXProcessingVars();
}

//Public
//Standard
void      Diode::tick()
{
  if(DEBUGLEVEL >= DEBUG_DAYISRUINED)printDebug();
  
  if (offsetTimer < offset)
  {
    offsetTimer++;
  }
  else
  {
    if      (brightness < goalBrightness) brightness++;
    else if (brightness > goalBrightness) brightness--;

    
    for (uint8_t i = 0; i < speed; i++)
    {
      if (colour != COLOUR_CYCLE)
      {
        effectVariables.c = colour;
      }
      
      if (!effectFinished) effectFinished = processEffect(*effect, &effectVariables, customPalette);


      if(colour == COLOUR_CYCLE && effectFinished)
      {
        effectVariables.c++;
        if(effectVariables.c == AMOUNTOFCOLOURS) effectVariables.c = (COLOUR_BLACK + 1);
      }
      else if (effectFinished)
      {
        if (repeat) effectFinished = false;
      
      }
    }
  }
}
//Effects
void      Diode::setGoalBrightness(uint8_t goalBrightness)
{
  this->goalBrightness  = goalBrightness;
}
void      Diode::setVfx(VFXData vfxData)
{
  if(DEBUGLEVEL >= DEBUG_OPERATIONS)
  {
    Serial.print(F(" Diode.setVFX(); D"));
    Serial.println(number);
  }
  
  this->colour      = vfxData.colour;
  this->offset      = vfxData.offset;
  this->speed       = vfxData.speed;
  this->repeat      = vfxData.repeat;
  resetFXProcessingVars();

  if(DEBUGLEVEL >= DEBUG_DAYISRUINED)
  {
    Serial.println(F("Is now:"));
    printDebug();
  }
}
void      Diode::setDataCustom(CustomPalette *customPalette)
{
  this->customPalette = customPalette;
}
//Technical
CRGB      Diode::getRGB(uint8_t sysBrightness)
{
  uint8_t r = (((this->effectVariables.r * this->goalBrightness) / 255) * sysBrightness) / 255;
  uint8_t g = (((this->effectVariables.g * this->goalBrightness) / 255) * sysBrightness) / 255;
  uint8_t b = (((this->effectVariables.b * this->goalBrightness) / 255) * sysBrightness) / 255;
  return CRGB(r, g, b);
}
void      Diode::resetFXProcessingVars      ()
{
  this->effectVariables.d                       = 0;
  this->effectVariables.fxProgression           = 0;      // In effect cycling
  this->effectVariables.c                       = 0; // Cycles of the whole effect (But with different colourss)
  this->offsetTimer                     = 0;
  this->effectFinished                   = false;
}

//Transmissions
String    Diode::convertToTransmission()
{
  String data = "TXDIO";
  
  data += number;

  data += goalBrightness;
  data += *effect;
  data += colour;
  data += offset;
  data += speed;
  data += repeat;

  data += effectVariables.r;
  data += effectVariables.g;
  data += effectVariables.b;

  return data;
}
//Debug
void      Diode::printDebug()
{
  Serial.print(F("Diode "));
  Serial.println(number);

  Serial.print(F("goalBrightness "));
  Serial.print(goalBrightness);
  Serial.print(F("brightness "));
  Serial.print(brightness);
  Serial.print(F(" | effect "));
  Serial.print(*effect);
  Serial.print(F(" | colour "));
  Serial.print(colour);
  Serial.print(F(" | offset "));
  Serial.print(offset);
  Serial.print(F(" | speed "));
  Serial.print(speed);

  Serial.print(F(" === r "));
  Serial.print(effectVariables.r);
  Serial.print(F(" | g "));
  Serial.print(effectVariables.g);
  Serial.print(F(" | b "));
  Serial.print(effectVariables.b);
  Serial.print(F(" | d "));
  Serial.print(effectVariables.d);
  Serial.print(F(" | c "));
  Serial.print(effectVariables.c);
  Serial.print(F(" | fxProgression "));
  Serial.print(effectVariables.fxProgression);
  Serial.print(F(" | offsetTimer "));
  Serial.println(offsetTimer);
}