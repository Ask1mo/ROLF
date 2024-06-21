#include "main.h"



void setAnimation_Default()
{
  trinity->setSpeed(15);
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_SPECIAL_SYNTH, COLOUR_BLACK, (uint16_t)(random(0, 10)*15), 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_SPECIAL_SYNTH, COLOUR_BLACK, uint16_t(j*10), 1, true});
    }
  }
}
void playAnimation_Reset()
{
  Serial.println(F("playAnimation_Reset"));
  trinity->setSpeed(1);

  uint16_t diodeNumbers = 0;
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLACK, 0, 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      diodeNumbers++;
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_STATIC, COLOUR_VIOLET, diodeNumbers, 1, false});
    }
  }

  trinity->forceTick(diodeNumbers, true, 10);
  delay(1000);
}
void setAnimation_Westpoint_FullWhite()
{
  Serial.println(F("setAnimation_Westpoint_FullWhite"));
  trinity->setSpeed(1);

  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_WHITE, 0, 1, true});
  }
}
void setAnimation_Westpoint_Default()
{
  Serial.println(F("setAnimation_Westpoint_Default"));
  trinity->setSpeed(1);

  uint16_t offset = 0;
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, (uint16_t)(random(0, 10)*15), 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_SPECIAL_RAINBOW, COLOUR_RED, 0, 5, true});
      offset++;
    }
  }
}
void setAnimation_Westpoint_BreathingLines()
{
  Serial.println(F("setAnimation_Westpoint_BREATHINGLINES"));
  trinity->setSpeed(1);

  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, (uint16_t)(random(0, 10)*15), 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_PAUSEDBREATHING, COLOUR_CYCLE, j, 20, true});
    }
  }
}
void setAnimation_Westpoint_FlashingLines()
{
  Serial.println(F("setAnimation_Westpoint_FlashingGLINES"));
  trinity->setSpeed(1);

  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, (uint16_t)(random(0, 10)*15), 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_PAUSEDFLASH, COLOUR_WHITE, j, 10, true});
    }  
  }
}
void setAnimation_Westpoint_Rain()
{
  Serial.println(F("setAnimation_Westpoint_Rain"));
  trinity->setSpeed(1);

  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, (uint16_t)(random(0, 10)*15), 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_PAUSEDFLASH, COLOUR_BLUE, j, (uint8_t)random(0, 10), true});
    }
  }
}
void setAnimation_Westpoint_Matrix()
{
  Serial.println(F("setAnimation_Westpoint_Matrix"));
  trinity->setSpeed(1);

  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, (uint16_t)(random(0, 10)*15), 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_DECODE, COLOUR_GREEN, j, 10, true});
    }
  }
}
void setAnimation_Westpoint_SuperRainbow()
{
  Serial.println(F("setAnimation_Westpoint_SuperRainbow"));
  trinity->setSpeed(3);

  uint16_t offset = 0;
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, 0, 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_SPECIAL_RAINBOW, COLOUR_RED, offset, 10, true});
      offset++;
    }
  }
}
void setAnimation_Westpoint_ADHDRainbow()
{
  Serial.println(F("setAnimation_Westpoint_ADHDRAINBOW"));
  trinity->setSpeed(1);

  uint16_t offset = 0;
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_SPECIAL_RAINBOW, COLOUR_RED, offset, 100, true});
      offset++;
    }
  }
  trinity->forceTick(500, false, 0);
}
void setAnimation_Westpoint_BurningRainbow()
{
  Serial.println(F("setAnimation_Westpoint_BurningRainbow"));
  trinity->setSpeed(4);

  uint16_t offset = 0;
    for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
    {
      trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, (uint16_t)(i*5), 10, true});
      for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
      {
        trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_SPECIAL_RAINBOW, COLOUR_RED, (uint16_t)(j*5), 10, true});
        offset++;
      }
  

      
    }
}
void setAnimation_Westpoint_HeartbeatTower()
{
  Serial.println(F("setAnimation_Westpoint_HeartbeatTower"));
  trinity->setSpeed(5);

  uint16_t offset = 0;
    for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
    {
      trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, (uint16_t)(i*4), 1, true});
      for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
      {
        trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_HEARTBEAT, COLOUR_RED, 0, 1, true});
        offset++;
      }
  

      
    }
}
void setAnimation_Westpoint_Stoplight()
{
  Serial.println(F("setAnimation_Westpoint_Stoplight"));
  trinity->setSpeed(8);

  uint16_t offset = 0;
    for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
    {
      trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, offset, 1, true});
      for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
      {
        trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_PAUSEDFLASH, COLOUR_CYCLE, offset, 1, true});
        offset+=3;
      }
  

      
    }
}
void setAnimation_Westpoint_PowerRise()
{
  Serial.println(F("setAnimation_Westpoint_Powerrize"));
  trinity->setSpeed(15);

  uint16_t offset = 0;
    for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
    {
      trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, offset, 1, true});
      for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
      {
        trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_PAUSEDFLASH, COLOUR_CYAN, offset, 1, true});
        offset++;
      }
  

      
    }
}
void setAnimation_Westpoint_Fishbowl()
{
  Serial.println(F("setAnimation_Westpoint_Fishbowl"));
  trinity->setSpeed(10);

  uint16_t offset = 0;
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, offset, 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_PAUSEDFLASH, COLOUR_CYCLE, offset, 1, true});
      offset+=10;
    }
  }
  trinity->forceTick(900, false, 0);
}
void setAnimation_Westpoint_Coils()
{
  Serial.println(F("setAnimation_Westpoint_Coils"));
  trinity->setSpeed(10);

  uint16_t offset = 0;

  for (uint8_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, 0, 1, true});
    for (uint8_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, VFXData{ EFFECT_STOCK_DECODE, COLOUR_CYCLE, offset, 1, true});
      offset++;
    }
  }
}
void setAnimation_Westpoint_AppearThing()
{
  Serial.println(F("setAnimation_Westpoint_AppearThing"));
  trinity->setSpeed(20);

  uint16_t offset = 0;

  for (uint8_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, (uint16_t)random(0, 100), 1, true});
    for (uint8_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j,VFXData{ EFFECT_STOCK_DECODE, COLOUR_CYCLE, (uint16_t)random(0, 100), 1, true});
      offset++;
    }
  }

}
void setAnimation_Westpoint_AppearThing2()
{
  Serial.println(F("setAnimation_Westpoint_AppearThing2"));
  trinity->setSpeed(10);

  uint16_t offset = 0;

  for (uint8_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, (uint16_t)random(0, 100), 1, true});
    for (uint8_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j,VFXData{ EFFECT_STOCK_STATIC, COLOUR_CYCLE, (uint16_t)random(0, 100), 1, true});
      offset++;
    }
  }

}
void setAnimation_Westpoint_AppearThing3()
{
  Serial.println(F("setAnimation_Westpoint_AppearThing3"));
  trinity->setSpeed(10);

  uint16_t offset = 0;

  for (uint8_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, 0, 1, true});
    for (uint8_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j,VFXData{ EFFECT_STOCK_STATIC, COLOUR_CYCLE, (uint16_t)random(0, 1000), 1, true});
      offset++;
    }
  }
}
void setAnimation_Westpoint_AppearThing4()
{
  Serial.println(F("setAnimation_Westpoint_AppearThing4"));
  trinity->setSpeed(4);

  uint16_t offset = 0;

  for (uint8_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, 0, 1, true});
    for (uint8_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j,VFXData{ EFFECT_STOCK_PAUSEDFLASH, COLOUR_CYCLE, (uint16_t)random(0, 250), 1, true});
      offset++;
    }
  }
}
void setAnimation_Westpoint_ColourBlink()
{
  Serial.println(F("setAnimation_Westpoint_ColourBlink"));
  trinity->setSpeed(4);

  uint16_t offset = 0;

  for (uint8_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, 0, 1, true});
    for (uint8_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j,VFXData{ EFFECT_STOCK_STATIC, COLOUR_CYCLE, i, 1, true});
      offset++;
    }
  }
  trinity->forceTick(64, false, 0);
}


void setup()
{
  Serial.begin(BAUDRATE);
  
  trinity = new Trinity(PIN_LEDS, PIN_BUTTON, PIN_LIGHTSENSOR, 60);

  #ifdef PANELSETUP_PROTO
  trinity->addPanel(new Panel(0, 0, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(1, 1, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(2, 2, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(3, 3, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(4, 4, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(5, 5, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, LEDSAMOUNT_TRIANGLE));
  #endif
  #ifdef PANELSETUP_ATOS
  trinity->addPanel(new Panel(0, 0, 0, CLOCK_COUNTERWISE, COMPASS_WEST, 38));
  #endif
  #ifdef PANELSETUP_EVA
  trinity->addPanel(new Panel(0, 0, 0, CLOCK_COUNTERWISE, COMPASS_SOUTH_EAST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(1, 0, 0, CLOCK_COUNTERWISE, COMPASS_SOUTH,      LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(2, 0, 0, CLOCK_COUNTERWISE, COMPASS_SOUTH_WEST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(3, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH_EAST, LEDSAMOUNT_TRIANGLE));
  #endif
  #ifdef PANELSETUP_LIAM
  trinity->addPanel(new Panel(0, 0, 0, CLOCK_COUNTERWISE, COMPASS_SOUTH_EAST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(1, 0, 0, CLOCK_COUNTERWISE, COMPASS_SOUTH,      LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(2, 0, 0, CLOCK_COUNTERWISE, COMPASS_SOUTH_WEST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(3, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH_EAST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(4, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(5, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH_WEST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(6, 0, 0, CLOCK_COUNTERWISE, COMPASS_SOUTH_WEST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(7, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH_EAST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(8, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(9, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH_WEST, LEDSAMOUNT_TRIANGLE));
  #endif
  #ifdef PANELSETUP_PRIME
  trinity->addPanel(new Panel( 0, 0, 3, CLOCK_COUNTERWISE, COMPASS_SOUTH_EAST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel( 1, 1, 3, CLOCK_COUNTERWISE, COMPASS_SOUTH,      LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel( 2, 2, 3, CLOCK_COUNTERWISE, COMPASS_SOUTH_WEST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel( 3, 3, 3, CLOCK_CLOCKWISE,   COMPASS_NORTH_EAST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel( 4, 4, 3, CLOCK_CLOCKWISE,   COMPASS_NORTH,      LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel( 5, 5, 3, CLOCK_CLOCKWISE,   COMPASS_NORTH_WEST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel( 6, 6, 3, CLOCK_COUNTERWISE, COMPASS_SOUTH_WEST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel( 7, 5, 2, CLOCK_CLOCKWISE,   COMPASS_NORTH_EAST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel( 8, 5, 2, CLOCK_CLOCKWISE,   COMPASS_NORTH,      LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel( 9, 0, 1, CLOCK_CLOCKWISE,   COMPASS_NORTH_WEST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(10, 0, 1, CLOCK_CLOCKWISE,   COMPASS_NORTH,      LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(11, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH_WEST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(12, 0, 1, CLOCK_COUNTERWISE, COMPASS_SOUTH_WEST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(13, 0, 2, CLOCK_CLOCKWISE,   COMPASS_NORTH_EAST, LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(14, 0, 2, CLOCK_CLOCKWISE,   COMPASS_NORTH,      LEDSAMOUNT_TRIANGLE));
  trinity->addPanel(new Panel(15, 0, 2, CLOCK_CLOCKWISE,   COMPASS_NORTH_WEST, LEDSAMOUNT_TRIANGLE));
  #endif
  #ifdef PANELSETUP_CHRISTMAS
  for (uint8_t i = 0; i < 10; i++)trinity->addPanel(new Panel(i, i, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 5));
  #endif
  #ifdef PANELSETUP_WESTPOINT
  //Tower Base
  trinity->addPanel(new Panel( 0, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel( 1, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      11));
  trinity->addPanel(new Panel( 2, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel( 3, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      11));
  trinity->addPanel(new Panel( 4, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel( 5, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel( 6, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      11));
  trinity->addPanel(new Panel( 7, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel( 8, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel( 9, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(10, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      11));
  trinity->addPanel(new Panel(11, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(12, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      11));
  trinity->addPanel(new Panel(13, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(14, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      9 ));
  //Tower Top
  trinity->addPanel(new Panel(15, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(16, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      11));
  trinity->addPanel(new Panel(17, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(18, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(19, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(20, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(21, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      11));
  trinity->addPanel(new Panel(22, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(23, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(24, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      11));
  trinity->addPanel(new Panel(25, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(26, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      11));
  trinity->addPanel(new Panel(27, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      10));
  trinity->addPanel(new Panel(28, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      12));
  //Rigns
  trinity->addPanel(new Panel(29, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      37));
  trinity->addPanel(new Panel(30, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      37));
  trinity->addPanel(new Panel(31, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      37));
  trinity->addPanel(new Panel(32, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      37));
  #endif
  #ifdef PANELSETUP_MINITOWER
  for (uint8_t i = 0; i < 4; i++) trinity->addPanel(new Panel(i, i, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 10));
  trinity->addPanel(new Panel( 4, 4, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      6 ));
  #endif
  #ifdef PANELSETUP_TEST
  trinity->addPanel(new Panel(0, 0, 0, CLOCK_COUNTERWISE, COMPASS_SOUTH_EAST, 5));
  trinity->addPanel(new Panel(1, 0, 0, CLOCK_COUNTERWISE, COMPASS_SOUTH,      5));
  trinity->addPanel(new Panel(2, 0, 0, CLOCK_COUNTERWISE, COMPASS_SOUTH_WEST, 5));
  trinity->addPanel(new Panel(3, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH_EAST, 5));
  trinity->addPanel(new Panel(4, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH,      5));
  trinity->addPanel(new Panel(5, 0, 0, CLOCK_CLOCKWISE,   COMPASS_NORTH_WEST, 5));
  #endif
  #ifdef PANELSETUP_HOUSECUBE
  for (uint8_t i = 0; i < 20; i++) trinity->addPanel(new Panel(i, i, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 6));
  #endif
  #ifdef PANELSETUP_HOUSECUBEV2
  for (uint8_t i = 0; i < 10; i++) trinity->addPanel(new Panel(i, i, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 60));
  #endif
  #ifdef PANELSETUP_POWERWIRE
  for (uint8_t i = 0; i < 60; i++) trinity->addPanel(new Panel(i, i, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 1));
  #endif
  #ifdef PANELSETUP_VOICETUBE
  for (uint8_t i = 0; i < 10; i++) trinity->addPanel(new Panel(i, i, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 6));
  #endif
  #ifdef PANELSETUP_VOICETUBE_PSV
  for (uint8_t i = 0; i < 20; i++) trinity->addPanel(new Panel(i, 0, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 15));
  #endif

  trinity->begin();

  currentShowingEffect = 0;

  Serial.println(F("---===SETUP COMPLETED===---"));




  

  #ifdef PANELSETUP_HOUSECUBE
  //One time LED setup
  /*
  trinity->setSpeed(1);

  uint16_t offset = 0;
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_WHITE, offset, 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_STATIC, COLOUR_WHITE, offset, 1, true});
      offset+=5;
    }
  }
  */
  trinity->setSpeed(1);

  uint16_t offset = 0;
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_PLANE, COLOUR_RED, 0, 1, true});
  }
 #endif

 #ifdef PANELSETUP_HOUSECUBEV2
  //One time LED setup
  lastPowerUpdate = 0;
  pinMode(PIN_RELAY, INPUT);
  button = new AskButton(PIN_BUTTON, 100);

  trinity->setSpeed(1);
  trinity->setBrightnessMode(BRIGHTNESS_2_NOR);


  uint16_t offset = 0;
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_RED, 0, 1, true});
    for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
    {
      trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_STATIC, COLOUR_RED, offset, 1, true});
      offset++;
    }
  }
    powerPercentage = 50;

/*
  trinity->setSpeed(1);

  uint16_t offset = 0;
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_PLANE, COLOUR_RED, 0, 1, true});
  }
  */




 #endif


  #ifdef PANELSETUP_POWERWIRE
  
  pinMode(PIN_RELAY, OUTPUT);
  //One time LED setup
  trinity->setSpeed(3);

  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_PAUSEDBREATHING, COLOUR_YELLOW, i*10, 5, true});
  }
  #endif

  #ifdef PANELSETUP_VOICETUBE

  Serial.println(F("TEST MESSAGE AAAAAAAAAAAAAAAAAAAAAAAAA"));

  pinMode(PIN_RELAY, INPUT);
  //One time LED setup
  trinity->setSpeed(10);
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLACK, 0, 5, true});
  }

  #endif

  #ifdef PANELSETUP_VOICETUBE_PSV

  Serial.println(F("TEST MESSAGE AAAAAAAAAAAAAAAAAAAAAAAAA"));

  dingusButtonA = new AskButton(PIN_RELAY, 100);
  dingusButtonB = new AskButton(PIN_BUTTON, 100);
  //One time LED setup
  trinity->setSpeed(10);
  for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
  {
    trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLACK, 0, 5, true});
  }

  #endif

}

void loop()
{
  trinity->tick();
  uint64_t currentMillis = millis();


  #ifdef PANELSETUP_POWERWIRE
  if(currentMillis >= (prevMillis+10000))
  {
    prevMillis = currentMillis;
    dayNight = !dayNight;

    if (dayNight)// If it's day
    {
      Serial.println(F("Day"));
      digitalWrite(PIN_RELAY, HIGH);
      trinity->setBrightnessMode(BRIGHTNESS_3_MAX);
    }
    else
    {
      Serial.println(F("Night"));
      digitalWrite(PIN_RELAY, LOW);
      trinity->setBrightnessMode(BRIGHTNESS_0_OFF);
    }
    
  }
  #endif

  #ifdef PANELSETUP_HOUSECUBEV2


  
  if (currentMillis >= (lastPowerUpdate+POWERUPDATEINTERVAL))
  {
    lastPowerUpdate = currentMillis;

    dayNight = digitalRead(PIN_RELAY);
    if (dayNight) //If it's day
    {
      if(!button->getPressState())
      {
        if(powerPercentage < 100)
        {
          powerPercentage+=POWERUPDATEVARIABLE;
          if (powerPercentage >= 100)
          {
            Serial.println(F("Power overload!"));

            for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
            {
              trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_BLINK, COLOUR_YELLOW, random16(0, 200), random8(2, 6), true});
            }
            trinity->forceTick(100, true, 0);

            trinity->setBrightnessMode(BRIGHTNESS_0_OFF);
            trinity->forceTick(100, true, 0);
            for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
            {
              trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLACK, 0, 1, true});
            }

            powerPercentage = 50;
            trinity->setBrightnessMode(BRIGHTNESS_2_NOR);
            delay(1000);
          }
        }

        colour = COLOUR_YELLOW;
      }
      else
      {
        colour = COLOUR_GREEN;
      }
    }
    else
    {
      if(button->getPressState())
      {
        if(powerPercentage > 0)
        {
          powerPercentage-=POWERUPDATEVARIABLE;
          if(powerPercentage <= 0)
          {
            Serial.println(F("Power depleted!"));
            for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
            {
              trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_BLINK, COLOUR_RED, random16(0, 200), random8(2, 6), true});
            }
            trinity->forceTick(100, true, 0);

            trinity->setBrightnessMode(BRIGHTNESS_0_OFF);
            trinity->forceTick(100, true, 0);
            for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
            {
              trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLACK, 0, 1, true});
            }

            powerPercentage = 50;
            trinity->setBrightnessMode(BRIGHTNESS_2_NOR);
            delay(1000);
          }
        }

        colour = COLOUR_RED;
      }
      else
      {
        colour = COLOUR_GREEN;
      }
    }
  }




  

    uint16_t diodeNumber = 0;
    for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
    {
      for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
      {
        if(powerPercentage > ((float)diodeNumber/3)) //Magic number makes diodeamount back to 3
        {
          if(button->getPressState())
          {
            trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_STATIC, colour, 0, 1, true});
          }
          else
          {
            trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_STATIC, colour, 0, 1, true});
          }
        }
        else
        {
          trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLACK, 0, 1, true});
        }
        diodeNumber++;
      }
    }
  
  
  #endif

  #ifdef PANELSETUP_VOICETUBE

  

    
    if (digitalRead(PIN_RELAY))sensorHits++;
    sensorPolls++;


    if(currentMillis >= (prevSensorMillis+SENSORPOLLTIME))
    {
      prevSensorMillis = currentMillis;
      uint16_t requiredSensorHits = sensorPolls * SENSORTRIGGERPERCENTAGE;
      
      if((sensorHits*100) > requiredSensorHits)
      {
        Serial.println(F("TRIGGERED"));

        uint8_t colour = random8(1, 7);

        uint16_t offset = 0;
        for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
        {
          for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
          {
            offset+=5;
          }
        }

        for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
        {
          trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_FLASH, colour, 0, 1, false});
          for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
          {
            trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_FLASH, colour, offset, 1, false});
            offset-=5;
          }
        }
      }

      sensorHits = 0;
      sensorPolls = 0;
    }  

  #endif

    #ifdef PANELSETUP_VOICETUBE_PSV

  if (dingusButtonA->getPressState() && !someoneIsTalking)
  {
    someoneIsTalking = true;
    lastMillis_buttonPressA = currentMillis;
    currentEffectColour = random8(1, 7);
    for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
    {
      trinity->setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLACK, 0, 1, true});
      for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
      {
        trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLACK, 0, 1, true});
      }
    }
    trinity->forceTick(1, true, 0);
  }
  
  if(someoneIsTalking && dingusButtonA->getPressState())
  {    
    uint64_t elapsedMillis_someoneIsTalking = currentMillis - lastMillis_buttonPressA;
    uint16_t elapsedleds_someoneIsTalking = elapsedMillis_someoneIsTalking / 20;
    uint16_t offset = 0;
    uint16_t diodeNumber = 0;
    for (uint16_t i = 0; i < trinity->getPanelAmount(); i++)
    {
      for (uint16_t j = 0; j < trinity->getPanelDiodeAmount(i); j++)
      {
        if(diodeNumber < elapsedleds_someoneIsTalking)
        {
          trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_FLASH, COLOUR_RED, 0, 1, false});
        }
        else
        {
          trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_FLASH, COLOUR_RED, offset, 1, false});
          offset+=5;
        }
        diodeNumber++;
      }
    }
  }  





  if (!dingusButtonA->getPressState() && !dingusButtonB->getPressState() && someoneIsTalking) someoneIsTalking = false;





  if (dingusButtonB->getPressState() && !someoneIsTalking)
  {
    someoneIsTalking = true;
    lastMillis_buttonPressB = currentMillis;
    currentEffectColour = random8(1, 7);
    for (int16_t i = trinity->getPanelAmount() - 1; i >= 0; i--)
    {
      for (int16_t j = trinity->getPanelDiodeAmount(i) - 1; j >= 0; j--)
      {
        trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLACK, 0, 1, true});
      }
    }
    trinity->forceTick(1, true, 0);
  }

  

  if(someoneIsTalking && dingusButtonB->getPressState())
  {
    
    uint64_t elapsedMillis_someoneIsTalking = currentMillis - lastMillis_buttonPressB;
    uint16_t elapsedleds_someoneIsTalking = elapsedMillis_someoneIsTalking / 20;

    uint16_t offset = 0;
    int16_t diodeNumber = 0;//= trinity->getPanelAmount() * trinity->getPanelDiodeAmount(0) - 1;
    for (int16_t i = trinity->getPanelAmount() - 1; i >= 0; i--)
    {
      for (int16_t j = trinity->getPanelDiodeAmount(i) - 1; j >= 0; j--)
      {
        if(diodeNumber < elapsedleds_someoneIsTalking)
        {
          trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_FLASH, COLOUR_BLUE, 0, 1, false});
        }
        else
        {
          trinity->setPanelDiodeVfx(i, j, (VFXData){EFFECT_STOCK_FLASH, COLOUR_BLUE, offset, 1, false});
          offset+=5;
        }
        diodeNumber++;
      }
    }
  }
  #endif




  #if ENABLECYCLING
  uint64_t currentMillis = millis();
  if(currentMillis >= (prevMillis+NEXTEFFECTTIME))
  {
    prevMillis = currentMillis;
    currentShowingEffect++;
    if (currentShowingEffect == 18)
    {
      currentShowingEffect = 0;
    }
    
    #ifdef ANIMATIONSET_WESTPOINT
    playAnimation_Reset();
    #endif

    switch (currentShowingEffect)
    {
      #ifdef ANIMATIONSET_STOCK
      #endif
      #ifdef ANIMATIONSET_WESTPOINT
      case 0:
      setAnimation_Westpoint_Default();
      break;
      case 1:
      setAnimation_Westpoint_BreathingLines();
      break;
      case 2:
      setAnimation_Westpoint_FlashingLines();
      break;
      case 3:
      setAnimation_Westpoint_Rain();
      break;
      case 4:
      setAnimation_Westpoint_Matrix();
      break;
      case 5:
      setAnimation_Westpoint_SuperRainbow();
      break;
      case 6:
      setAnimation_Westpoint_ADHDRainbow();
      break;
      case 7:
      setAnimation_Westpoint_BurningRainbow();
      break;
      case 8:
      setAnimation_Westpoint_HeartbeatTower();
      break;
      case 9:
      setAnimation_Westpoint_Stoplight();
      break;
      case 10:
      setAnimation_Westpoint_PowerRise();
      break;
      case 11:
      setAnimation_Westpoint_Fishbowl();
      break;
      case 12:
      setAnimation_Westpoint_Coils();
      break;
      case 13:
      setAnimation_Westpoint_AppearThing();
      break;
      case 14:
      setAnimation_Westpoint_AppearThing2();
      break;
      case 15:
      setAnimation_Westpoint_AppearThing3();
      break;
      case 16:
      setAnimation_Westpoint_AppearThing4();
      break;
      #endif
      default:
      if(DEBUGLEVEL >= DEBUG_ERRORS) Serial.println(F("ERROR: NO ANIMATIONSET SELECTED"));
      break;
    }
  }
  #endif
}


