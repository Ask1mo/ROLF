/*
#include "MaskManager.h"

//Constructor
MaskManager::MaskManager                              (Panel **panels)
{
  Serial.println(F("MaskManager Starting..."));

  maskMode = MASKMODE_DISABLED;

  for (uint8_t i = 0; i < CANVASWIDTH; i++)
  {
    for (uint8_t j = 0; j < CANVASWIDTH; j++)
    {
      canvasPanels[i][j] = NULL;
    }
  }

  for (uint8_t i = 0; i < PANELAMOUNT; i++)
  {
    canvasPanels[panels[i]->getX()][panels[i]->getY()] = panels[i];
  }

  printDebugCanvas();
  


  Serial.println(F("...MaskManager Started"));
}

//Private
void    MaskManager::printDebugCanvas                 ()
{
  Serial.println(F("Printing Panel Canvas"));

  for (uint8_t i = 0; i < CANVASWIDTH; i++)
  {
    Serial.print(F("X"));
    Serial.print(i);
    Serial.print("] ");
    for (uint8_t j = 0; j < CANVASWIDTH; j++)
    {
      if (canvasPanels[i][j])
      {
        uint8_t panelNumber = canvasPanels[i][j]->getPanelNumber();
        if(panelNumber < 10)
        {
          Serial.print(0);//Addds a 0 to numbers lower than 10 for uniformity
        }
        Serial.print(panelNumber);
        Serial.print(F(", "));
      }
      else
      {
        Serial.print(F("__"));
      }
    }
    Serial.println();
  }
}

//Public
void    MaskManager::tick                             () 
{
  switch (maskMode)
  {
    case MASKMODE_DISABLED:
    return;
    break;

    case MASKMODE_ONETIME_FADEIN:
    break;

    case MASKMODE_ONETIME_FADEOUT:
    break;

    case MASKMODE_REPEATING_MUSIC:
    break;

    case MASKMODE_REPEATING_EMERGENCY:
    break;

    case MASKMODE_REPEATING_STROKE:
    break;
  }
} 
void    MaskManager::print                            () 
{
  FastLED.show(); 
}
void    MaskManager::setMode                    	    (uint8_t maskMode)
{
  this->maskMode = maskMode;
}
*/
