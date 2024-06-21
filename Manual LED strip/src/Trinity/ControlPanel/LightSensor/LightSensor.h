#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <Arduino.h>
#include "Trinity/setup.h"

#define BRIGHTNESS_0_OFF 0
#define BRIGHTNESS_0_OFF_VAL 0

#define BRIGHTNESS_1_DIM 1
#define BRIGHTNESS_1_DIM_VAL 10

#define BRIGHTNESS_2_NOR 2
#define BRIGHTNESS_2_NOR_VAL 100

#define BRIGHTNESS_3_MAX 3
#define BRIGHTNESS_3_MAX_VAL 255

#define BRIGHTNESS_4_AUT 4



class LightSensor
{
private:
  uint8_t pin;
  bool enabled;

  uint8_t recommendedBrightness;
  int brightnessOffset;

public:
  LightSensor(uint8_t pin);
  void    tick();
  bool    getEnabled();
  void    setEnabled(bool enabled);
  uint8_t getBrightnessOffset();
  void    setBrightnessOffset(int brightnessOffset);
  uint8_t getRecommendedBrightness();
};



#endif