#ifndef DIODE_H
#define DIODE_H

#include <Arduino.h>
#include <FastLED.h>
#include "Trinity/ledManager/effectProcessor/effectProcessor.h"
#include "setup.h"


class Diode
{
private:
  uint16_t                                  number;

  uint8_t                                   goalBrightness;
  uint8_t                                   brightness;
  uint8_t                                   *effect; //Points to the effect of the panel. I don't want to have panels running differing effects on it's diodes
  uint8_t                                   colour;
  uint16_t                                  offset;
  uint8_t                                   speed;
  bool                                      repeat;
  
  CustomPalette                             *customPalette;
  EffectVariables                           effectVariables;
  uint16_t                                  offsetTimer;

  bool                                      effectFinished;

public:
  Diode                                     (uint16_t number, uint8_t *panelEffect);
  //Standard
  void      tick                            ();
  //Effects
  void      setGoalBrightness                   (uint8_t goalBrightness);
  void      setVfx                          (VFXData vfxData);
  void      setDataCustom                   (CustomPalette *customPalette);
  bool      getEffectFinished               ();
  //Technical
  CRGB      getRGB                          (uint8_t sysBrightness);
  void      resetFXProcessingVars           ();
  //Transmisisons
  String    convertToTransmission           ();
  //Debug
  void      printDebug                      ();
};

#endif