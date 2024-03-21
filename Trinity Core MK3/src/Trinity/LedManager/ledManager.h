#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include "Panel/Panel.h"
#include "../src/Trinity/setup.h"

class LedManager
{
private:
  uint8_t                                 panelAmount;
  uint16_t                                diodeAmount;
  uint8_t                                 canvasWidth;
  uint8_t                                 canvasHeight;
  uint8_t                                 goalBrightness;
  uint8_t                                 brightness;
  uint8_t                                 speed;
  Panel                                   **panels;
  Panel                                   ***panelMatrix;
  CRGB                                    *leds;
  CustomPalette                           *customPalette[CUSTOMPALETTEAMOUNT];

  

public:
  LedManager                              ();
  void    addPanel                        (Panel *panel);
  void    begin                           ();
  //Standard
  void    tick                            ();
  void    print                           ();
  //Effects
  uint8_t getGoalBrightness                   ();
  void    setGoalBrightness                   (uint8_t goalGoalBrightness);
  void    setSpeed                        (uint8_t speed);
  //Panel Effects
  void    setPanelBrightness              (uint8_t panelNumber, uint8_t goalBrightness);
  void    setPanelVfx                     (uint8_t panelNumber, VFXData vfxData);
  //Diode Effects
  void    setPanelBrightness              (uint8_t panelNumber, uint16_t diodeNumber, uint8_t goalBrightness);
  void    setPanelDiodeVfx                (uint8_t panelNumber, uint16_t diodeNumber, VFXData vfxData);
  //Technical
  uint8_t getPanelAmount                  ();
  uint16_t getDiodeAmount                 ();
  uint16_t getPanelDiodeAmount            (uint8_t panelNumber);
  uint8_t getCanvasWidth                  ();
  uint8_t getCanvasHeight                 ();
  //Transmissions
  String  convertToTansmission            ();
  String  convertPanelToTransmission      (uint8_t panelNumber);
  String  convertPanelDiodeToTransmission (uint8_t panelNumber,uint16_t diodeNumber);

  void setCustomPaletteColours(uint8_t slot, uint8_t colourRGBNumber, ColourRGB colourRGB);
  void setCustomPaletteAvailableColours(uint8_t slot, uint8_t avalaibleColours);
};  

#endif