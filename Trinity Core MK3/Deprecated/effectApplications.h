#ifndef EFFECTAPPLICATIONS_H
#define EFFECTAPPLICATIONS_H

#include <Arduino.h>

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
#define EFFECT_STOCK_APPEAR           8
#define EFFECT_CUSTOM_STATIC          9
#define EFFECT_CUSTOM_BLINK           10
#define EFFECT_CUSTOM_PLANE           11
#define EFFECT_CUSTOM_BREATHING       12
#define EFFECT_CUSTOM_PAUSEDBREATHING 13
#define EFFECT_CUSTOM_FLASH           14
#define EFFECT_CUSTOM_PAUSEDFLASH     15
#define EFFECT_CUSTOM_HEARTBEAT       16
#define EFFECT_SPECIAL_RAINBOW        17
#define EFFECT_SPECIAL_FIRE           18
#define EFFECT_SPECIAL_SOUND          19

struct ColourRGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class EffectApplications
{
private:
  bool getColourClearance(byte colourToClear, byte colourChannel);

public:
  EffectApplications();
  bool stock_static(ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour);
  bool stock_blink(ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour);
  bool stock_plane(ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour);
  bool stock_breathing(ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour);
  bool stock_pausedbreathing(ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour);
  bool stock_flash(ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour);
  bool stock_pausedFlash(ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour);
  bool stock_heartbeat(ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour);
  bool stock_appear(ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour);

  bool custom_static(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
  bool custom_blink(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
  bool custom_plane(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
  bool custom_breathing(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
  bool custom_pausedbreathing(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
  bool custom_flash(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
  bool custom_pausedFlash(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
  bool custom_heartbeat(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
  bool custom_appear(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
  
  bool special_rainbow(ColourRGB *rgb, uint8_t *d, uint8_t *progFx);
  bool special_fire(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
  bool special_sound(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);

  bool special_fade(ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour);
};



#endif