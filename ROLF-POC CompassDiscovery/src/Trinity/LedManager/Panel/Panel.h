#ifndef PANEL_H
#define PANEL_H

#include <Arduino.h>
#include "Diode/Diode.h"

#define LEDSAMOUNT_TRIANGLE 17

//These are mostly used for setup, but are also selectable when applying offset stuff.
#define DIR_STRIP           0
#define CLOCK_CLOCKWISE     1
#define CLOCK_COUNTERWISE   2
#define COMPASS_NORTH       3
#define COMPASS_NORTH_EAST  4
#define COMPASS_EAST        5
#define COMPASS_SOUTH_EAST  6
#define COMPASS_SOUTH       7
#define COMPASS_SOUTH_WEST  8
#define COMPASS_WEST        9
#define COMPASS_NORTH_WEST  10

class Panel
{
private:
  Diode                                     **diodes;

  uint8_t                                   number;

  uint8_t                                   x;
  uint8_t                                   y;
  uint8_t                                   compassDir;
  bool                                      clockDir;

  uint16_t                                  diodeAmount; // Amount of leds in this panel
  uint16_t                                  diodeStart;  // The coordinate of the first LED
  
  uint8_t                                   goalBrightness;
  uint8_t                                   brightness;
  uint8_t                                   effect;
  uint8_t                                   colour;
  uint16_t                                  offset;
  uint8_t                                   speed;
  bool                                      repeat;
  bool                                      detailed;

  CustomPalette                             *customPalette;
  EffectVariables                           effectVariables;
  uint16_t                                  offsetTimer;

public:
  Panel                                     (uint8_t number, uint8_t x, uint8_t y, uint8_t compassDir, bool clockDir, uint16_t diodeAmount);
  //Standard
  void      tick                            ();
  //Effects
  void      setGoalBrightness                   (uint8_t goalBrightness);
  void      setVfx                          (VFXData vfxData);
  void      setDataCustom                   (CustomPalette *customPaletteArg);
  //Diode Effects
  void      setDiodeGoalBrightness              (uint16_t diodeNumber, uint8_t goalBrightness);
  void      setDiodeVfx                     (uint16_t diodeNumber, VFXData vfxData);
  void      setDiodeDataCustom              (uint16_t diodeNumber, CustomPalette *customPalette);
  //Mask Effects
  void      setMaskPercentage               (uint8_t percentage);
  //Technical
  uint8_t   getPanelNumber                  ();
  uint8_t   getX                            ();
  uint8_t   getY                            ();
  CRGB      getDiodeRGB                     (uint8_t number, uint8_t sysBrightness);
  uint16_t  getDiodeAmount                  ();
  uint16_t  getDiodeStart                   ();
  void      setDiodeStart                   (uint16_t ledStart);
  void      resetFXProcessingVars           ();
  //Transmissions
  String    convertToTransmission           ();
  String    convertDiodeToTransmission  	  (uint16_t diodeNumber);
  //Debug
  void      printDebug                      ();
};

#endif