#ifndef TRINITY_H
#define TRINITY_H

#include "setup.h"
#include "LedManager/ledManager.h"
#include "ControlPanel/AskButton/AskButton.h"
#include "ControlPanel/LightSensor/LightSensor.h"
#include "ControlPanel/SleepTimer/SleepTimer.h"
#include "ControlPanel/Comms/Comms.h"
//#include "ControlPanel/Link/Link.h"
#include "ControlPanel/WifiManager/WebServerManager.h"
//#include "ControlPanel/WifiManager/index.html"

class Trinity
{
private:
    LedManager          *ledManager;
    AskButton           *button;
    LightSensor         *lightSensor;
    SleepTimer          *sleepTimer;
    Comms               *comms;
    //Link              *link;
    WebServerManager    *webServerManager;

    bool setupComplete;


    uint8_t         brightnessMode;
    uint64_t        prevFrameMillis;
    uint16_t        frameTime;

public:
    Trinity(uint8_t ledPin, uint8_t buttonPin, uint8_t ldrPin, uint8_t maxFramerate);
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

    void setCustomPaletteColours(uint8_t slot, uint8_t colourRGBNumber, ColourRGB colourRGB);
    void setCustomPaletteAvailableColours(uint8_t slot, uint8_t avalaibleColours);
    uint8_t getPanelAmount();
};
#endif