#ifndef EFFECTPROCESSOR_H
#define EFFECTPROCESSOR_H

#include <Arduino.h>
#include "trinity/setup.h"

#define AMOUNTOFCOLOURS 7 //The amount of preset colours in the system, also functions as the amount of slots for custom colours

#define COLOUR_BLACK    0
#define COLOUR_RED      1
#define COLOUR_YELLOW   2
#define COLOUR_GREEN    3
#define COLOUR_CYAN     4
#define COLOUR_BLUE     5
#define COLOUR_VIOLET   6
#define COLOUR_WHITE    7
#define COLOUR_CYCLE    8

#define EFFECT_STOCK_STATIC           0
#define EFFECT_STOCK_BLINK            1
#define EFFECT_STOCK_PLANE            2
#define EFFECT_STOCK_BREATHING        3
#define EFFECT_STOCK_PAUSEDBREATHING  4
#define EFFECT_STOCK_FLASH            5
#define EFFECT_STOCK_PAUSEDFLASH      6
#define EFFECT_STOCK_HEARTBEAT        7
#define EFFECT_STOCK_DECODE           8

#define CUSTOMEFFECTNUMBER_FIRST      9
#define EFFECT_CUSTOM_STATIC          9
#define EFFECT_CUSTOM_BLINK           10
#define EFFECT_CUSTOM_PLANE           11
#define EFFECT_CUSTOM_BREATHING       12
#define EFFECT_CUSTOM_PAUSEDBREATHING 13
#define EFFECT_CUSTOM_FLASH           14
#define EFFECT_CUSTOM_PAUSEDFLASH     15
#define EFFECT_CUSTOM_HEARTBEAT       16
#define EFFECT_CUSTOM_DECODE          17
#define EFFECT_CUSTOM_FADE            18
#define CUSTOMEFFECTNUMBER_LAST       18

#define EFFECT_SPECIAL_RAINBOW        19
#define EFFECT_SPECIAL_SYNTH          20
#define EFFECT_SPECIAL_FIRE           21
#define EFFECT_SPECIAL_SOUND          22
#define EFFECT_DEV_UNBOUND            255



#define FXSET_DEVUNBOUND_DELTIM 20
#define FXSET_DEVUNBOUND_YLWVAL 100

struct ColourRGB
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};
struct VFXData
{
  uint8_t effect;
  uint8_t colour;
  uint16_t offset;
  uint8_t speed;
  bool repeat;
};
struct EffectVariables
{
  uint8_t                                   r;
  uint8_t                                   g;
  uint8_t                                   b;
  uint8_t                                   d; //Dummy
  uint8_t                                   c; //Current colour (Can cycle because of COLOUR_COLOURCYCLE)
  uint8_t                                   fxProgression; //Current part of the effect
};
struct CustomPalette 
{
  uint8_t                                   customRGBSlots; //Actually used amount of slots.
  ColourRGB                                 customRGB[AMOUNTOFCOLOURS];
};

bool processEffect(uint8_t effect, EffectVariables *effectVariables, CustomPalette *customPalette);
bool getColourClearance(uint8_t colourToClear, uint8_t colourChannel);

bool stock_static           (EffectVariables *effectVariables);
bool stock_blink            (EffectVariables *effectVariables);
bool stock_plane            (EffectVariables *effectVariables);
bool stock_breathing        (EffectVariables *effectVariables);
bool stock_pausedbreathing  (EffectVariables *effectVariables);
bool stock_flash            (EffectVariables *effectVariables);
bool stock_pausedFlash      (EffectVariables *effectVariables);
bool stock_heartbeat        (EffectVariables *effectVariables);
bool stock_decode           (EffectVariables *effectVariables);

bool custom_static          (EffectVariables *effectVariables, CustomPalette *customPalette);
bool custom_blink           (EffectVariables *effectVariables, CustomPalette *customPalette);
bool custom_plane           (EffectVariables *effectVariables, CustomPalette *customPalette);
bool custom_breathing       (EffectVariables *effectVariables, CustomPalette *customPalette);
bool custom_pausedbreathing (EffectVariables *effectVariables, CustomPalette *customPalette);
bool custom_flash           (EffectVariables *effectVariables, CustomPalette *customPalette);
bool custom_pausedFlash     (EffectVariables *effectVariables, CustomPalette *customPalette);
bool custom_heartbeat       (EffectVariables *effectVariables, CustomPalette *customPalette);
bool custom_decode          (EffectVariables *effectVariables, CustomPalette *customPalette);
bool custom_fade            (EffectVariables *effectVariables, CustomPalette *customPalette);

bool special_rainbow        (EffectVariables *effectVariables);
bool special_synth          (EffectVariables *effectVariables);

bool dev_unbound            (EffectVariables *effectVariables);

#endif