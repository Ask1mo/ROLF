#include "effectProcessor.h"


bool processEffect(uint8_t effect, EffectVariables *effectVariables, CustomPalette *customPalette)
{
  bool effectFinished = false;

  switch (effect)
  {
    case EFFECT_STOCK_STATIC:
    effectFinished = stock_static(effectVariables);
    break;
    case EFFECT_STOCK_BLINK:
    effectFinished = stock_blink(effectVariables);
    break;
    case EFFECT_STOCK_PLANE:
    effectFinished = stock_plane(effectVariables);
    break;
    case EFFECT_STOCK_BREATHING:
    effectFinished = stock_breathing(effectVariables);
    break;
    case EFFECT_STOCK_PAUSEDBREATHING:
    effectFinished = stock_pausedbreathing(effectVariables);
    break;
    case EFFECT_STOCK_FLASH:
    effectFinished = stock_flash(effectVariables);
    break;
    case EFFECT_STOCK_PAUSEDFLASH:
    effectFinished = stock_pausedFlash(effectVariables);
    break;
    case EFFECT_STOCK_HEARTBEAT:
    effectFinished = stock_heartbeat(effectVariables);
    break;
    case EFFECT_STOCK_DECODE:
    effectFinished = stock_decode(effectVariables);
    break;
    
    case EFFECT_CUSTOM_STATIC:
    effectFinished = custom_static(effectVariables, customPalette);
    break;
    case EFFECT_CUSTOM_BLINK:
    effectFinished = custom_blink(effectVariables, customPalette);
    break;
    case EFFECT_CUSTOM_PLANE:
    effectFinished = custom_plane(effectVariables, customPalette);
    break;
    case EFFECT_CUSTOM_BREATHING:
    effectFinished = custom_breathing(effectVariables, customPalette);
    break;
    case EFFECT_CUSTOM_PAUSEDBREATHING:
    effectFinished = custom_pausedbreathing(effectVariables, customPalette);
    break;
    case EFFECT_CUSTOM_FLASH:
    effectFinished = custom_flash(effectVariables, customPalette);
    break;
    case EFFECT_CUSTOM_PAUSEDFLASH:
    effectFinished = custom_pausedFlash(effectVariables, customPalette);
    break;
    case EFFECT_CUSTOM_HEARTBEAT:
    effectFinished = custom_heartbeat(effectVariables, customPalette);
    break;
    case EFFECT_CUSTOM_DECODE:
    effectFinished = custom_decode(effectVariables, customPalette);
    break;
    case EFFECT_CUSTOM_FADE:
    effectFinished = custom_fade(effectVariables, customPalette);
    break;

    case EFFECT_SPECIAL_RAINBOW:
    effectFinished = special_rainbow(effectVariables);
    break;
    case EFFECT_SPECIAL_SYNTH:
    effectFinished = special_synth(effectVariables);
    break;

    case EFFECT_DEV_UNBOUND:
    effectFinished = dev_unbound(effectVariables);
    break;
      
    default:
    if(DEBUGLEVEL >= DEBUG_ERRORS) Serial.println(F("ERROR: effectProcessor.processEffect() - Selected Effect not programmed"));
    break;
  }

  return effectFinished;
}

bool getColourClearance(uint8_t colourToClear, uint8_t colourChannel)
{
  switch (colourChannel)
  {
  case COLOUR_RED:
  {
    if (colourToClear == COLOUR_WHITE || colourToClear == COLOUR_RED || colourToClear == COLOUR_YELLOW || colourToClear == COLOUR_VIOLET)
      return true;
    return false;
  }
  break;

  case COLOUR_GREEN:
  {
    if (colourToClear == COLOUR_WHITE || colourToClear == COLOUR_YELLOW || colourToClear == COLOUR_GREEN || colourToClear == COLOUR_CYAN)
      return true;
    return false;
  }
  break;

  case COLOUR_BLUE:
  {
    if (colourToClear == COLOUR_WHITE || colourToClear == COLOUR_CYAN || colourToClear == COLOUR_BLUE || colourToClear == COLOUR_VIOLET)
      return true;
    return false;
  }
  break;
  }
  return false;
}

bool stock_static           (EffectVariables *effectVariables)
{
  bool allowR = getColourClearance(effectVariables->c, COLOUR_RED);
  bool allowG = getColourClearance(effectVariables->c, COLOUR_GREEN);
  bool allowB = getColourClearance(effectVariables->c, COLOUR_BLUE);

  switch (effectVariables->fxProgression)
  {
    case 0:
      if(allowR)effectVariables->r = 255;
      else effectVariables->r = 0;
      if(allowG)effectVariables->g = 255;
      else effectVariables->g = 0;
      if(allowB)effectVariables->b = 255;
      else effectVariables->b = 0;
      effectVariables->d = 0;

      

      (effectVariables->fxProgression)++;
    break;

    case 1:
      (effectVariables->d)++;

      if (effectVariables->d == 255)
      {
        (effectVariables->fxProgression) = 0;
        return true;
      }
    break;
  }

  return false;
}
bool stock_blink            (EffectVariables *effectVariables)
{
  bool allowR = getColourClearance(effectVariables->c, COLOUR_RED);
  bool allowG = getColourClearance(effectVariables->c, COLOUR_GREEN);
  bool allowB = getColourClearance(effectVariables->c, COLOUR_BLUE);

  switch (effectVariables->fxProgression)
  {
    case 0:
      if(allowR)effectVariables->r = 255;
      else effectVariables->r = 0;
      if(allowG)effectVariables->g = 255;
      else effectVariables->g = 0;
      if(allowB)effectVariables->b = 255;
      else effectVariables->b = 0;
      effectVariables->d = 0;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      (effectVariables->d)++;

      if (effectVariables->d == 20) (effectVariables->fxProgression)++;
    break;

    case 2:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 0;

      (effectVariables->fxProgression)++;
    break;

    case 3:
      (effectVariables->d)++;

      if (effectVariables->d == 255)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool stock_plane            (EffectVariables *effectVariables)
{
  bool allowR = getColourClearance(effectVariables->c, COLOUR_RED);
  bool allowG = getColourClearance(effectVariables->c, COLOUR_GREEN);
  bool allowB = getColourClearance(effectVariables->c, COLOUR_BLUE);

  switch (effectVariables->fxProgression)
  {
    case 0:
      if (allowR)effectVariables->r = 255;
      else effectVariables->r = 0;
      if (allowG)effectVariables->g = 255;
      else effectVariables->g = 0;
      if (allowB)effectVariables->b = 255;
      else effectVariables->b = 0;
      effectVariables->d = 0;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      (effectVariables->d)++;

      if(effectVariables->d == 20) (effectVariables->fxProgression)++;
    break;

    case 2:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 0;

      (effectVariables->fxProgression)++;
    break;

    case 3:
      (effectVariables->d)++;

      if(effectVariables->d == 40) (effectVariables->fxProgression)++;
    break;

    case 4:
      if(allowR) effectVariables->r = 255;
      if(allowG) effectVariables->g = 255;
      if(allowB) effectVariables->b = 255;

      (effectVariables->fxProgression)++;
    break;

    case 5:
      (effectVariables->d)++;

      if(effectVariables->d == 60) (effectVariables->fxProgression)++;
    break;

    case 6:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 0;

      (effectVariables->fxProgression)++;
    break;

    case 7:
      (effectVariables->d)++;

      if(effectVariables->d == 255) 
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool stock_breathing        (EffectVariables *effectVariables)
{
  bool allowR = getColourClearance(effectVariables->c, COLOUR_RED);
  bool allowG = getColourClearance(effectVariables->c, COLOUR_GREEN);
  bool allowB = getColourClearance(effectVariables->c, COLOUR_BLUE);

  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 0;
      effectVariables->d = 0;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if (allowR)effectVariables->r++;
      if (allowG)effectVariables->g++;
      if (allowB)effectVariables->b++;
      (effectVariables->d)++;

      if (effectVariables->d == 255)(effectVariables->fxProgression)++;
    break;

    case 2:
      if (allowR)effectVariables->r--;
      if (allowG)effectVariables->g--;
      if (allowB)effectVariables->b--;
      (effectVariables->d)--;

      if (effectVariables->d == 0)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool stock_pausedbreathing  (EffectVariables *effectVariables)
{
  bool allowR = getColourClearance(effectVariables->c, COLOUR_RED);
  bool allowG = getColourClearance(effectVariables->c, COLOUR_GREEN);
  bool allowB = getColourClearance(effectVariables->c, COLOUR_BLUE);

  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 0;
      effectVariables->d = 0;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if (allowR)effectVariables->r++;
      if (allowG)effectVariables->g++;
      if (allowB)effectVariables->b++;
      (effectVariables->d)++;

      if (effectVariables->d == 255)(effectVariables->fxProgression)++;
    break;

    case 2:
      if (allowR)effectVariables->r--;
      if (allowG)effectVariables->g--;
      if (allowB)effectVariables->b--;
      (effectVariables->d)--;

      if (effectVariables->d == 0)(effectVariables->fxProgression)++;
    break;

    case 3:
      (effectVariables->d)++;

      if (effectVariables->d == 255)(effectVariables->fxProgression)++;
    break;

    case 4:
      (effectVariables->d)--;

      if (effectVariables->d == 0)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool stock_flash            (EffectVariables *effectVariables)
{
  bool allowR = getColourClearance(effectVariables->c, COLOUR_RED);
  bool allowG = getColourClearance(effectVariables->c, COLOUR_GREEN);
  bool allowB = getColourClearance(effectVariables->c, COLOUR_BLUE);

  switch (effectVariables->fxProgression)
  {
    case 0:
      if(allowR)effectVariables->r = 255;
      else effectVariables->r = 0;
      if(allowG) effectVariables->g = 255;
      else effectVariables->g = 0;
      if(allowB) effectVariables->b = 255;
      else effectVariables->b = 0;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if(allowR)effectVariables->r--;
      if(allowG)effectVariables->g--;
      if(allowB)effectVariables->b--;
      (effectVariables->d)--;

      if (effectVariables->d == 0)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool stock_pausedFlash      (EffectVariables *effectVariables)
{
  bool allowR = getColourClearance(effectVariables->c, COLOUR_RED);
  bool allowG = getColourClearance(effectVariables->c, COLOUR_GREEN);
  bool allowB = getColourClearance(effectVariables->c, COLOUR_BLUE);

  switch (effectVariables->fxProgression)
  {
    case 0:
      if(allowR)effectVariables->r = 255;
      else effectVariables->r = 0;
      if(allowG) effectVariables->g = 255;
      else effectVariables->g = 0;
      if(allowB) effectVariables->b = 255;
      else effectVariables->b = 0;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if(allowR)effectVariables->r--;
      if(allowG)effectVariables->g--;
      if(allowB)effectVariables->b--;
      (effectVariables->d)--;

      if (effectVariables->d == 0)(effectVariables->fxProgression)++;
    break;

    case 2:
      (effectVariables->d)++;
      if (effectVariables->d == 255)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool stock_heartbeat        (EffectVariables *effectVariables)
{
  bool allowR = getColourClearance(effectVariables->c, COLOUR_RED);
  bool allowG = getColourClearance(effectVariables->c, COLOUR_GREEN);
  bool allowB = getColourClearance(effectVariables->c, COLOUR_BLUE);

  switch (effectVariables->fxProgression)
  {
    case 0:
      if(allowR)effectVariables->r = 255;
      else effectVariables->r = 0;
      if(allowG)effectVariables->g = 255;
      else effectVariables->g = 0;
      if(allowB)effectVariables->b = 255;
      else effectVariables->b = 0;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if(allowR)effectVariables->r--;
      if(allowG)effectVariables->g--;
      if(allowB)effectVariables->b--;
      (effectVariables->d)--;

      if(effectVariables->d == 100) (effectVariables->fxProgression)++;
    break;

    case 2:
      if(allowR) effectVariables->r = 255;
      if(allowG) effectVariables->g = 255;
      if(allowB) effectVariables->b = 255;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 3:
      if(allowR)effectVariables->r--;
      if(allowG)effectVariables->g--;
      if(allowB)effectVariables->b--;
      (effectVariables->d)--;

      if (effectVariables->d == 0)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool stock_decode           (EffectVariables *effectVariables)
{
  bool allowR = getColourClearance(effectVariables->c, COLOUR_RED);
  bool allowG = getColourClearance(effectVariables->c, COLOUR_GREEN);
  bool allowB = getColourClearance(effectVariables->c, COLOUR_BLUE);

  switch (effectVariables->fxProgression)
  {
    case 0:
      if(allowR)effectVariables->r = 255;
      else effectVariables->r = 0;
      if(allowG)effectVariables->g = 255;
      else effectVariables->g = 0;
      if(allowB)effectVariables->b = 255;
      else effectVariables->b = 0;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if(allowR)effectVariables->r--;
      if(allowG)effectVariables->g--;
      if(allowB)effectVariables->b--;
      (effectVariables->d)--;

      if(effectVariables->d == 100) (effectVariables->fxProgression)++;
    break;

    case 2:
      (effectVariables->d)++;
      if(effectVariables->d == 255) (effectVariables->fxProgression)++;
    break;

    case 3:
      if(allowR)effectVariables->r = 255;
      if(allowG)effectVariables->g = 255;
      if(allowB)effectVariables->b = 255;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 4:
      if(allowR)effectVariables->r--;
      if(allowG)effectVariables->g--;
      if(allowB)effectVariables->b--;
      (effectVariables->d)--;

      if (effectVariables->d == 0) (effectVariables->fxProgression)++;
    break;

    case 5:
      (effectVariables->d)++;
      if (effectVariables->d == 223) 
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}

bool custom_static          (EffectVariables *effectVariables, CustomPalette *customPalette)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = customPalette->customRGB[effectVariables->c].r;
      effectVariables->g = customPalette->customRGB[effectVariables->c].g;
      effectVariables->b = customPalette->customRGB[effectVariables->c].b;
      effectVariables->d = 0;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      (effectVariables->d)++;

      if (effectVariables->d == 255)
      {
        (effectVariables->fxProgression) = 0;
        return true;
      }
    break;
  }

  return false;
}
bool custom_blink           (EffectVariables *effectVariables, CustomPalette *customPalette)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = customPalette->customRGB[effectVariables->c].r;
      effectVariables->g = customPalette->customRGB[effectVariables->c].g;
      effectVariables->b = customPalette->customRGB[effectVariables->c].b;
      effectVariables->d = 0;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      (effectVariables->d)++;

      if (effectVariables->d == 20) (effectVariables->fxProgression)++;
    break;

    case 2:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 0;

      (effectVariables->fxProgression)++;
    break;

    case 3:
      (effectVariables->d)++;

      if (effectVariables->d == 255)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool custom_plane           (EffectVariables *effectVariables, CustomPalette *customPalette)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = customPalette->customRGB[effectVariables->c].r;
      effectVariables->g = customPalette->customRGB[effectVariables->c].g;
      effectVariables->b = customPalette->customRGB[effectVariables->c].b;
      effectVariables->d = 0;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      (effectVariables->d)++;

      if(effectVariables->d == 20) (effectVariables->fxProgression)++;
    break;

    case 2:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 0;

      (effectVariables->fxProgression)++;
    break;

    case 3:
      (effectVariables->d)++;

      if(effectVariables->d == 40) (effectVariables->fxProgression)++;
    break;

    case 4:
      effectVariables->r = customPalette->customRGB[effectVariables->c].r;
      effectVariables->g = customPalette->customRGB[effectVariables->c].g;
      effectVariables->b = customPalette->customRGB[effectVariables->c].b;

      (effectVariables->fxProgression)++;
    break;

    case 5:
      (effectVariables->d)++;

      if(effectVariables->d == 60) (effectVariables->fxProgression)++;
    break;

    case 6:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 0;

      (effectVariables->fxProgression)++;
    break;

    case 7:
      (effectVariables->d)++;

      if(effectVariables->d == 255) 
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool custom_breathing       (EffectVariables *effectVariables, CustomPalette *customPalette)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 0;
      effectVariables->d = 0;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if (effectVariables->r < customPalette->customRGB[effectVariables->c].r) effectVariables->r++;
      if (effectVariables->g < customPalette->customRGB[effectVariables->c].g) effectVariables->g++;
      if (effectVariables->b < customPalette->customRGB[effectVariables->c].b) effectVariables->b++;   
      (effectVariables->d)++;

      if (effectVariables->d == 255)(effectVariables->fxProgression)++;
    break;

    case 2:
      if (effectVariables->r > 0) effectVariables->r--;
      if (effectVariables->g > 0) effectVariables->g--;
      if (effectVariables->b > 0) effectVariables->b--; 
      (effectVariables->d)--;

      if (effectVariables->d == 0)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool custom_pausedbreathing (EffectVariables *effectVariables, CustomPalette *customPalette)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 0;
      effectVariables->d = 0;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if (effectVariables->r < customPalette->customRGB[effectVariables->c].r) effectVariables->r++;
      if (effectVariables->g < customPalette->customRGB[effectVariables->c].g) effectVariables->g++;
      if (effectVariables->b < customPalette->customRGB[effectVariables->c].b) effectVariables->b++; 
      (effectVariables->d)++;

      if (effectVariables->d == 255)(effectVariables->fxProgression)++;
    break;

    case 2:
      if (effectVariables->r > 0) effectVariables->r--;
      if (effectVariables->g > 0) effectVariables->g--;
      if (effectVariables->b > 0) effectVariables->b--; 
      (effectVariables->d)--;

      if (effectVariables->d == 0)(effectVariables->fxProgression)++;
    break;

    case 3:
      (effectVariables->d)++;

      if (effectVariables->d == 255)(effectVariables->fxProgression)++;
    break;

    case 4:
      (effectVariables->d)--;

      if (effectVariables->d == 0)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool custom_flash           (EffectVariables *effectVariables, CustomPalette *customPalette)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = customPalette->customRGB[effectVariables->c].r;
      effectVariables->g = customPalette->customRGB[effectVariables->c].g;
      effectVariables->b = customPalette->customRGB[effectVariables->c].b;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if (effectVariables->r > 0) effectVariables->r--;
      if (effectVariables->g > 0) effectVariables->g--;
      if (effectVariables->b > 0) effectVariables->b--; 
      (effectVariables->d)--;

      if (effectVariables->d == 0)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool custom_pausedFlash     (EffectVariables *effectVariables, CustomPalette *customPalette)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = customPalette->customRGB[effectVariables->c].r;
      effectVariables->g = customPalette->customRGB[effectVariables->c].g;
      effectVariables->b = customPalette->customRGB[effectVariables->c].b;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if (effectVariables->r > 0) effectVariables->r--;
      if (effectVariables->g > 0) effectVariables->g--;
      if (effectVariables->b > 0) effectVariables->b--; 
      (effectVariables->d)--;

      if (effectVariables->d == 0)(effectVariables->fxProgression)++;
    break;

    case 2:
      (effectVariables->d)++;
      if (effectVariables->d == 255)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool custom_heartbeat       (EffectVariables *effectVariables, CustomPalette *customPalette)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = customPalette->customRGB[effectVariables->c].r;
      effectVariables->g = customPalette->customRGB[effectVariables->c].g;
      effectVariables->b = customPalette->customRGB[effectVariables->c].b;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if (effectVariables->r > 0) effectVariables->r--;
      if (effectVariables->g > 0) effectVariables->g--;
      if (effectVariables->b > 0) effectVariables->b--; 
      (effectVariables->d)--;

      if(effectVariables->d == 100) (effectVariables->fxProgression)++;
    break;

    case 2:
      effectVariables->r = customPalette->customRGB[effectVariables->c].r;
      effectVariables->g = customPalette->customRGB[effectVariables->c].g;
      effectVariables->b = customPalette->customRGB[effectVariables->c].b;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 3:
      if (effectVariables->r > 0) effectVariables->r--;
      if (effectVariables->g > 0) effectVariables->g--;
      if (effectVariables->b > 0) effectVariables->b--; 
      (effectVariables->d)--;

      if (effectVariables->d == 0)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool custom_decode          (EffectVariables *effectVariables, CustomPalette *customPalette)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = customPalette->customRGB[effectVariables->c].r;
      effectVariables->g = customPalette->customRGB[effectVariables->c].g;
      effectVariables->b = customPalette->customRGB[effectVariables->c].b;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      if (effectVariables->r > 0) effectVariables->r--;
      if (effectVariables->g > 0) effectVariables->g--;
      if (effectVariables->b > 0) effectVariables->b--; 
      (effectVariables->d)--;

      if(effectVariables->d == 100) (effectVariables->fxProgression)++;
    break;

    case 2:
      (effectVariables->d)++;
      if(effectVariables->d == 255) (effectVariables->fxProgression)++;
    break;

    case 3:
      effectVariables->r = customPalette->customRGB[effectVariables->c].r;
      effectVariables->g = customPalette->customRGB[effectVariables->c].g;
      effectVariables->b = customPalette->customRGB[effectVariables->c].b;
      effectVariables->d = 255;

      (effectVariables->fxProgression)++;
    break;

    case 4:
      if (effectVariables->r > 0) effectVariables->r--;
      if (effectVariables->g > 0) effectVariables->g--;
      if (effectVariables->b > 0) effectVariables->b--; 
      (effectVariables->d)--;

      if (effectVariables->d == 0) (effectVariables->fxProgression)++;
    break;

    case 5:
      (effectVariables->d)++;
      if (effectVariables->d == 223) 
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}
bool custom_fade              (EffectVariables *effectVariables, CustomPalette *customPalette)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->d = 0;

      effectVariables->fxProgression++;
    break;

    case 1:
      if (effectVariables->r < customPalette->customRGB[effectVariables->c].r) effectVariables->r++;
      if (effectVariables->r > customPalette->customRGB[effectVariables->c].r) effectVariables->r--;

      if (effectVariables->g < customPalette->customRGB[effectVariables->c].g) effectVariables->g++;
      if (effectVariables->g > customPalette->customRGB[effectVariables->c].g) effectVariables->g--;

      if (effectVariables->b < customPalette->customRGB[effectVariables->c].b) effectVariables->b++;
      if (effectVariables->b > customPalette->customRGB[effectVariables->c].b) effectVariables->b--;

      effectVariables->d++;

      if (effectVariables->d == 255)return true;
    break;

    default:
      effectVariables->fxProgression = 0;
    break;
  }
  return false;
}

bool special_rainbow        (EffectVariables *effectVariables)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 255;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      (effectVariables->r)++;
      if (effectVariables->r == 255) (effectVariables->fxProgression)++;
    break;

    case 2:
      effectVariables->b--;
      if (effectVariables->b == 0) (effectVariables->fxProgression)++;
    break;

    case 3:
      effectVariables->g++;
      if (effectVariables->g == 255) (effectVariables->fxProgression)++;
    break;

    case 4:
      effectVariables->r--;
      if (effectVariables->r == 0) (effectVariables->fxProgression)++;
    break;

    case 5:
      effectVariables->b++;
      if (effectVariables->b == 255) (effectVariables->fxProgression)++;
    break;

    case 6:
      effectVariables->g--;
      if (effectVariables->g == 0)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;

    default:
      effectVariables->fxProgression = 0;
    break;
  }
  return false;
}
bool special_synth          (EffectVariables *effectVariables)
{
  switch (effectVariables->fxProgression)
  {
    case 0:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->b = 255;

      (effectVariables->fxProgression)++;
    break;

    case 1:
      (effectVariables->r)++;
      if (effectVariables->r == 255) (effectVariables->fxProgression)++;
    break;

    case 2:
      effectVariables->b--;
      if (effectVariables->b == 0) (effectVariables->fxProgression)++;
    break;

    case 3:
      effectVariables->r--;
      if (effectVariables->r == 0) (effectVariables->fxProgression)++;
    break;

    case 4:
      effectVariables->b++;
      if (effectVariables->b == 255)
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;

    default:
      effectVariables->fxProgression = 0;
    break;
  }
  return false;
}

bool dev_unbound            (EffectVariables *effectVariables)
{
  switch (effectVariables->fxProgression)
  {
    //First Flash
    case 0:
      effectVariables->r = 255;
      effectVariables->g = FXSET_DEVUNBOUND_YLWVAL;
      effectVariables->b = 0;
      effectVariables->d = 0;
      (effectVariables->fxProgression)++;
    break;

    case 1:
      (effectVariables->d)++;
      if(effectVariables->d == FXSET_DEVUNBOUND_DELTIM) (effectVariables->fxProgression)++;
    break;


    case 2:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->d = 0;
      (effectVariables->fxProgression)++;
    break;

    case 3:
      (effectVariables->d)++;
      if(effectVariables->d == FXSET_DEVUNBOUND_DELTIM) (effectVariables->fxProgression)++;
    break;




    //Second Flash
    case 4:
      effectVariables->r = 255;
      effectVariables->g = FXSET_DEVUNBOUND_YLWVAL;
      effectVariables->d = 0;
      (effectVariables->fxProgression)++;
    break;

    case 5:
      (effectVariables->d)++;
      if(effectVariables->d == FXSET_DEVUNBOUND_DELTIM) (effectVariables->fxProgression)++;
    break;



    case 6:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->d = 0;
      (effectVariables->fxProgression)++;
    break;

    case 7:
      (effectVariables->d)++;
       if(effectVariables->d == FXSET_DEVUNBOUND_DELTIM) (effectVariables->fxProgression)++;
    break;




    //Third Flash (Long)
    case 8:
      effectVariables->r = 255;
      effectVariables->g = FXSET_DEVUNBOUND_YLWVAL;
      effectVariables->d = 0;
      (effectVariables->fxProgression)++;
    break;

    case 9:
      (effectVariables->d)++;
      if(effectVariables->d == FXSET_DEVUNBOUND_DELTIM*3) (effectVariables->fxProgression)++;
    break;



    case 10:
      effectVariables->r = 0;
      effectVariables->g = 0;
      effectVariables->d = 0;
      (effectVariables->fxProgression)++;
    break;

    case 11:
      (effectVariables->d)++;

      if(effectVariables->d == FXSET_DEVUNBOUND_DELTIM*3) 
      {
        effectVariables->fxProgression = 0;
        return true;
      }
    break;
  }
  return false;
}



