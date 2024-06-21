#include "LightSensor.h"

LightSensor::LightSensor(uint8_t pin)
{
  if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating LightSensor at adress "));
        Serial.println((int)this, DEC);
    }
  

  this->pin = pin;
  pinMode(pin, INPUT_PULLUP);
  enabled = false;
  recommendedBrightness = 100;
  brightnessOffset = 11;

  Serial.println(F("...LightSensor Started"));
}


void LightSensor::tick()
{
  uint16_t reading = analogRead(pin)/16; //Lower the resolution to 8 bit
  reading = reading*2;
  reading += brightnessOffset;

  if(reading >= BRIGHTNESS_3_MAX_VAL) reading = BRIGHTNESS_3_MAX_VAL;
  recommendedBrightness = reading;
}
bool LightSensor::getEnabled()
{
  return enabled;
}
void LightSensor::setEnabled(bool enabled)
{
  this->enabled = enabled;
}
uint8_t LightSensor::getBrightnessOffset()
{
  return brightnessOffset;
}
void LightSensor::setBrightnessOffset(int brightnessOffset)
{
  this->brightnessOffset = brightnessOffset;
}
uint8_t LightSensor::getRecommendedBrightness()
{
  return recommendedBrightness;
}