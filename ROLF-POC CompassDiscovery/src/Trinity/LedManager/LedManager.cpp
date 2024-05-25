#include "ledManager.h"

//Constructor
LedManager::LedManager                                  ()
{
  if(DEBUGLEVEL >= DEBUG_OPERATIONS)
  {
    Serial.print(F("Creating LedManager at adress "));
    Serial.println((int)this, DEC);
  }

  canvasWidth   = 0;
  canvasHeight  = 0;
  goalBrightness= 255;
  brightness    = 0;
  speed         = 1;
  panels        = NULL;
  panelAmount   = 0;
  diodeAmount   = 0;
  

  //create custom palettes
  for (uint8_t i = 0; i < CUSTOMPALETTEAMOUNT; i++)
  {
    customPalette[i] = new struct CustomPalette;
    for (uint8_t j = 0; j < AMOUNTOFCOLOURS; j++)
    {
      customPalette[i]->customRGB[j].r = 0;
      customPalette[i]->customRGB[j].g = 0;
      customPalette[i]->customRGB[j].b = 0;
    }
  }

  Serial.println(F("...LedManager Started (NOT READY YET, DONT FORGET TO USE finaliseSetup() after adding your panels!!!)"));
}
void      LedManager::addPanel                          (Panel *panel)
{
  panels = (Panel**)realloc(panels, sizeof(Panel*) * (panelAmount + 1));
  if (panels == NULL)
  {
    Serial.println(F("ERROR: Could not allocate memory for new panel"));
    return;
  }
  panels[panelAmount] = panel;
  panelAmount++;
  Serial.print(F("New panel added, Amount is now: "));
  Serial.println(panelAmount);
}
void      LedManager::begin                             ()
{
  //Count diodes and give them to the panels (tell them where they start)
  for (uint8_t i = 0; i < panelAmount; i++)
  {
    panels[i]->setDiodeStart(diodeAmount);
    diodeAmount += panels[i]->getDiodeAmount();
  }

  //create static led array
  const int staticDiodeAmount = diodeAmount;
  leds = new CRGB[staticDiodeAmount];
  if(DEBUGLEVEL >= DEBUG_OPERATIONS)
  {
    Serial.print(F("Allocated  "));
    Serial.print(staticDiodeAmount);
    Serial.print(F(" LED's at adress "));
    Serial.println((int)leds, DEC);
  }

  //start FastLED
  #ifdef PLATFORM_ESP32_FIREBEETLE2_DEBUG
  FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, staticDiodeAmount);
  #endif
  #ifndef PLATFORM_ESP32_FIREBEETLE2_DEBUG
  FastLED.addLeds<WS2812, PIN_LEDS, LEDCOLORDER>(leds, staticDiodeAmount);
  #endif

  /*
  Serial.println(F("Prepping canvasWidth and canvasHeight"));
  //Prepare the panel matrix x and y
  for (uint8_t i = 0; i < panelAmount; i++)
  {
    if (panels[i]->getX() > canvasWidth)
    {
      canvasWidth = panels[i]->getX()+1;
      Serial.print(F("New canvasWidth: "));
      Serial.println(canvasWidth);
    }
    if (panels[i]->getY() > canvasHeight)
    {
      canvasHeight = panels[i]->getY()+1;
      Serial.print(F("New canvasHeight: "));
      Serial.println(canvasHeight);
    }
  }
  
  Serial.println(F("Allocating the panel matrix"));
  // Allocate memory for the 2D array
  panelMatrix = new Panel**[canvasWidth];
  for(uint8_t i = 0; i < canvasWidth; ++i)
  {
    panelMatrix[i] = new Panel*[canvasHeight];
  }

  Serial.println(F("Setting all elements to nullptr"));
  // Initialize all elements to nullptr
  for(uint8_t i = 0; i < canvasWidth; ++i)
  {
    for(uint8_t j = 0; j < canvasHeight; ++j)
    {
      panelMatrix[i][j] = nullptr;
    }
  }

  Serial.println(F("Adding all panels to the matrix"));
  //Add all panels to their corresponding spots in the matrix
  for (uint8_t i = 0; i < panelAmount; i++)
  {
    
    Serial.println(F("Panel index out of bounds"));

    Serial.print(F("Adding panel to position "));
    Serial.print(panels[i]->getX());
    Serial.print(F(","));
    Serial.println(panels[i]->getY());
    
    if (panels[i]->getX() >= canvasWidth || panels[i]->getY() >= canvasHeight)
    {
      Serial.print(F("Panel index out of bounds: "));
      Serial.print(panels[i]->getX());
      Serial.print(F(","));
      Serial.print(panels[i]->getY());

      Serial.print(F("Canvas size: "));
      Serial.print(canvasWidth);
      Serial.print(F(","));
      Serial.println(canvasHeight);
    }
    
    panelMatrix[panels[i]->getX()][panels[i]->getY()] = panels[i];
    
  }
  */

  Serial.println(F("LedManager Ready!"));
}
//Public
//Standard
void      LedManager::tick                              () 
{
  if      (brightness < goalBrightness) brightness++;
  else if (brightness > goalBrightness) brightness--;

  


  for (uint8_t i = 0; i < speed; i++)
  {
    if(DEBUGLEVEL >=DEBUG_OPERATIONS) Serial.print(F("t"));

    for (uint8_t i = 0; i < panelAmount; i++)
    {
      panels[i]->tick();

      for(uint16_t j = 0; j < panels[i]->getDiodeAmount(); j++)
      {
        leds[panels[i]->getDiodeStart()+j] = panels[i]->getDiodeRGB(j, brightness);
      }
    }
  }
} 
void      LedManager::print                             () 
{
  if(DEBUGLEVEL >= DEBUG_OPERATIONS) Serial.print(F("p"));


  //Black out every even led
  if(filter)
  {
    for (uint16_t i = 0; i < diodeAmount; i++)
    {
      if (i % 2 == 0)
      {
        leds[i] = CRGB(0, 0, 0);
      }
    }
  }

  FastLED.show(); 
}
//Effects
uint8_t   LedManager::getGoalBrightness                 ()
{
  return goalBrightness;
}
void      LedManager::setGoalBrightness                 (uint8_t goalBrightness)
{
  this->goalBrightness = goalBrightness;
}
void      LedManager::setSpeed                          (uint8_t speed)
{
  this->speed = speed;
}
//Panel Effects
void      LedManager::setPanelBrightness                (uint8_t panelNumber, uint8_t goalBrightness)
{
  //Serial.println("Setting panel data (In ledmanager)");
  panels[panelNumber]->setGoalBrightness(goalBrightness);
}
void      LedManager::setPanelVfx                       (uint8_t panelNumber, VFXData vfxData)
{
  //Serial.println("Setting panel data (In ledmanager)");
  panels[panelNumber]->setVfx(vfxData);

  if (CUSTOMEFFECTNUMBER_FIRST <= vfxData.effect && vfxData.effect <= CUSTOMEFFECTNUMBER_LAST) //If it's a custom colour effect: Apply colour palette
  {
    panels[panelNumber]->setDataCustom(customPalette[vfxData.colour]);
  }
}
//Diode Effects
void      LedManager::setPanelBrightness                (uint8_t panelNumber, uint16_t diodeNumber, uint8_t goalBrightness)
{
  
}
void      LedManager::setPanelDiodeVfx                  (uint8_t panelNumber, uint16_t diodeNumber, VFXData vfxData)
{
  if (panelNumber > panelAmount)
  {
    Serial.print(F("LedManager::setPanelDiodeVfx() Too high panel number requested: "));
    Serial.print(panelNumber);
    Serial.print(F(". Max: "));
    Serial.println(panelAmount);
  }
  //Serial.println("Setting diode data (In ledmanager)");
  panels[panelNumber]->setDiodeVfx(diodeNumber, vfxData);
}
//Technical
uint8_t   LedManager::getPanelAmount                    ()
{
  return panelAmount;
}
uint16_t  LedManager::getDiodeAmount                    ()
{
  return diodeAmount;
}
uint16_t  LedManager::getPanelDiodeAmount               (uint8_t panelNumber)
{
  return panels[panelNumber]->getDiodeAmount();
}
uint8_t   LedManager::getCanvasWidth                    ()
{
  return canvasWidth;
}
uint8_t   LedManager::getCanvasHeight                   ()
{
  return canvasHeight;
}
//Transmissions
String    LedManager::convertToTansmission              ()
{
  String data = "TXLED";
  data += (char)goalBrightness;
  data += (char)speed;
  return data;
}
String    LedManager::convertPanelToTransmission        (uint8_t panelNumber)
{
  return panels[panelNumber]->convertToTransmission();
}
String    LedManager::convertPanelDiodeToTransmission   (uint8_t panelNumber,uint16_t diodeNumber)
{
  return panels[panelNumber]->convertDiodeToTransmission(diodeNumber);
}

void      LedManager::setCustomPaletteColours           (uint8_t slot, uint8_t colourRGBNumber, ColourRGB colourRGB)
{
  if (slot >= CUSTOMPALETTEAMOUNT)
  {
    if (DEBUGLEVEL >=  DEBUG_ERRORS)
    {
      Serial.print(F("ERROR: LedManager.setCustomPaletteColours() Slot selected too high: "));
      Serial.print(slot);
      Serial.print(F(" Max:"));
      Serial.println(CUSTOMPALETTEAMOUNT);
      return;
    }
  }

  if (colourRGBNumber >= AMOUNTOFCOLOURS)
  {
    if (DEBUGLEVEL >=  DEBUG_ERRORS)
    {
      Serial.print(F("ERROR: LedManager.setCustomPaletteColours() Colour number selected too high: "));
      Serial.print(slot);
      Serial.print(F(" Max:"));
      Serial.println(AMOUNTOFCOLOURS);
      return;
    }
  }

  
  customPalette[slot]->customRGB[colourRGBNumber].r = colourRGB.r;
  customPalette[slot]->customRGB[colourRGBNumber].g = colourRGB.g;
  customPalette[slot]->customRGB[colourRGBNumber].b = colourRGB.b;
}
void      LedManager::setCustomPaletteAvailableColours  (uint8_t slot, uint8_t avalaibleColours)
{
  customPalette[slot]->customRGBSlots = avalaibleColours;
}

void     LedManager::toggleFilter                         ()
{
  filter = !filter;
}