#ifndef TRINITY_H
#define TRINITY_H

#include "setup.h"
#include "LedManager/ledManager.h"


#define BRIGHTNESS_0_OFF 0
#define BRIGHTNESS_0_OFF_VAL 0

#define BRIGHTNESS_1_DIM 1
#define BRIGHTNESS_1_DIM_VAL 10

#define BRIGHTNESS_2_NOR 2
#define BRIGHTNESS_2_NOR_VAL 100

#define BRIGHTNESS_3_MAX 3
#define BRIGHTNESS_3_MAX_VAL 255

class Trinity
{
private:
    LedManager          *ledManager;

    bool setupComplete;

    uint8_t         brightnessMode;
    uint64_t        prevFrameMillis;
    uint16_t        frameTime;

public:
    Trinity(uint8_t maxFramerate);
    void addPanel(Panel *panel);
    void begin();
    void tick();
    void forceTick(uint16_t ticks, bool keepPrinting, uint16_t delayTime);  //Manually force the ledmanager to tick without running any other Trinity code.
    void setSpeed(uint8_t speed);
    void setBrightnessMode(uint8_t brightness);
    uint16_t getDiodeAmount();
    
    void setPanelVfx(uint8_t panelNumber, VFXData vfxData);
    uint16_t getPanelDiodeAmount(uint8_t panelNumber);
    void setPanelDiodeVfx(uint8_t panelNumber, uint16_t diodeNumber, VFXData vfxData);
    void setPanelBrightness(uint8_t panelNumber, uint8_t brightness, bool smoothEnabled);
    
    bool getPanelEffectFinished(uint8_t panelNumber);

    void setCustomPaletteColours(uint8_t slot, uint8_t colourRGBNumber, ColourRGB colourRGB);
    void setCustomPaletteAvailableColours(uint8_t slot, uint8_t avalaibleColours);
    uint8_t getPanelAmount();
    void toggleFilter();
};
#endif