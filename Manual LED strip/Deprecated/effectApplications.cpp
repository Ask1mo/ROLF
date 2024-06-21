#include "effectApplications.h"

//Constructor
EffectApplications::EffectApplications()
{
  //Serial.println(F("EffectApplications Imported"));
}

//Private
bool EffectApplications::getColourClearance(byte colourToClear, byte colourChannel)
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

//Public
bool EffectApplications::stock_static           (ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour)
{
  bool allowR = getColourClearance(colour, COLOUR_RED);
  bool allowG = getColourClearance(colour, COLOUR_GREEN);
  bool allowB = getColourClearance(colour, COLOUR_BLUE);

  switch (*progFx)
  {
    case 0:
      if(allowR)rgb->r = 255;
      else rgb->r = 0;
      if(allowG)rgb->g = 255;
      else rgb->g = 0;
      if(allowB)rgb->b = 255;
      else rgb->b = 0;
      *d = 0;

      (*progFx)++;
    break;

    case 1:
      (*d)++;

      if (*d == 255)
      {
        (*progFx) = 0;
        return true;
      }
    break;
  }

  return false;
}
bool EffectApplications::stock_blink            (ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour)
{
  bool allowR = getColourClearance(colour, COLOUR_RED);
  bool allowG = getColourClearance(colour, COLOUR_GREEN);
  bool allowB = getColourClearance(colour, COLOUR_BLUE);

  switch (*progFx)
  {
    case 0:
      if(allowR)rgb->r = 255;
      else rgb->r = 0;
      if(allowG)rgb->g = 255;
      else rgb->g = 0;
      if(allowB)rgb->b = 255;
      else rgb->b = 0;
      *d = 0;

      (*progFx)++;
    break;

    case 1:
      (*d)++;

      if (*d == 20) (*progFx)++;
    break;

    case 2:
      rgb->r = 0;
      rgb->g = 0;
      rgb->b = 0;

      (*progFx)++;
    break;

    case 3:
      (*d)++;

      if (*d == 255)
      {
        *progFx = 0;
        return true;
      }
    break;
  }
  return false;
}
bool EffectApplications::stock_plane            (ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour)
{
  bool allowR = getColourClearance(colour, COLOUR_RED);
  bool allowG = getColourClearance(colour, COLOUR_GREEN);
  bool allowB = getColourClearance(colour, COLOUR_BLUE);

  switch (*progFx)
  {
    case 0:
      if (allowR)rgb->r = 255;
      else rgb->r = 0;
      if (allowG)rgb->g = 255;
      else rgb->g = 0;
      if (allowB)rgb->b = 255;
      else rgb->b = 0;
      d = 0;

      (*progFx)++;
    break;

    case 1:
      (*d)++;

      if(*d == 20) (*progFx)++;
    break;

    case 2:
      rgb->r = 0;
      rgb->g = 0;
      rgb->b = 0;

      (*progFx)++;
    break;

    case 3:
      (*d)++;

      if(*d == 40) (*progFx)++;
    break;

    case 4:
      if(allowR) rgb->r = 255;
      if(allowG) rgb->g = 255;
      if(allowB) rgb->b = 255;

      (*progFx)++;
    break;

    case 5:
      (*d)++;

      if(*d == 60) (*progFx)++;
    break;

    case 6:
      rgb->r = 0;
      rgb->g = 0;
      rgb->b = 0;

      (*progFx)++;
    break;

    case 7:
      (*d)++;

      if(*d == 255) 
      {
        *progFx = 0;
        return true;
      }
    break;
  }
  return false;
}
bool EffectApplications::stock_breathing        (ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour)
{
  bool allowR = getColourClearance(colour, COLOUR_RED);
  bool allowG = getColourClearance(colour, COLOUR_GREEN);
  bool allowB = getColourClearance(colour, COLOUR_BLUE);

  switch (*progFx)
  {
    case 0:
      rgb->r = 0;
      rgb->g = 0;
      rgb->b = 0;
      *d = 0;

      (*progFx)++;
    break;

    case 1:
      if (allowR)rgb->r++;
      if (allowG)rgb->g++;
      if (allowB)rgb->b++;
      (*d)++;

      if (*d == 255)(*progFx)++;
    break;

    case 2:
      if (allowR)rgb->r--;
      if (allowG)rgb->g--;
      if (allowB)rgb->b--;
      (*d)--;

      if (*d == 0)
      {
        *progFx = 0;
        return true;
      }
    break;
  }
  return false;
}
bool EffectApplications::stock_pausedbreathing  (ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour)
{
  bool allowR = getColourClearance(colour, COLOUR_RED);
  bool allowG = getColourClearance(colour, COLOUR_GREEN);
  bool allowB = getColourClearance(colour, COLOUR_BLUE);

  switch (*progFx)
  {
    case 0:
      rgb->r = 0;
      rgb->g = 0;
      rgb->b = 0;
      *d = 0;

      (*progFx)++;
    break;

    case 1:
      if (allowR)rgb->r++;
      if (allowG)rgb->g++;
      if (allowB)rgb->b++;
      (*d)++;

      if (*d == 255)(*progFx)++;
    break;

    case 2:
      if (allowR)rgb->r--;
      if (allowG)rgb->g--;
      if (allowB)rgb->b--;
      (*d)--;

      if (*d == 0)(*progFx)++;
    break;

    case 3:
      (*d)++;

      if (*d == 255)(*progFx)++;
    break;

    case 4:
      (*d)--;

      if (*d == 0)
      {
        *progFx = 0;
        return true;
      }
    break;
  }
  return false;
}
bool EffectApplications::stock_flash            (ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour)
{
  bool allowR = getColourClearance(colour, COLOUR_RED);
  bool allowG = getColourClearance(colour, COLOUR_GREEN);
  bool allowB = getColourClearance(colour, COLOUR_BLUE);

  switch (*progFx)
  {
    case 0:
      if(allowR)rgb->r = 255;
      else rgb->r = 0;
      if(allowG) rgb->g = 255;
      else rgb->g = 0;
      if(allowB) rgb->b = 255;
      else rgb->b = 0;
      *d = 255;

      (*progFx)++;
    break;

    case 1:
      if(allowR)rgb->r--;
      if(allowG)rgb->g--;
      if(allowB)rgb->b--;
      (*d)--;

      if (*d == 0)
      {
        *progFx = 0;
        return true;
      }
    break;
  }
  return false;
}
bool EffectApplications::stock_pausedFlash      (ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour)
{
  bool allowR = getColourClearance(colour, COLOUR_RED);
  bool allowG = getColourClearance(colour, COLOUR_GREEN);
  bool allowB = getColourClearance(colour, COLOUR_BLUE);

  switch (*progFx)
  {
    case 0:
      if(allowR)rgb->r = 255;
      else rgb->r = 0;
      if(allowG) rgb->g = 255;
      else rgb->g = 0;
      if(allowB) rgb->b = 255;
      else rgb->b = 0;
      *d = 255;

      (*progFx)++;
    break;

    case 1:
      if(allowR)rgb->r--;
      if(allowG)rgb->g--;
      if(allowB)rgb->b--;
      (*d)--;

      if (*d == 0)(*progFx)++;
    break;

    case 2:
      (*d)++;
      if (*d == 255)
      {
        *progFx = 0;
        return true;
      }
    break;
  }
  return false;
}
bool EffectApplications::stock_heartbeat        (ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour)
{
  bool allowR = getColourClearance(colour, COLOUR_RED);
  bool allowG = getColourClearance(colour, COLOUR_GREEN);
  bool allowB = getColourClearance(colour, COLOUR_BLUE);

  switch (*progFx)
  {
    case 0:
      if(allowR)rgb->r = 255;
      else rgb->r = 0;
      if(allowG)rgb->g = 255;
      else rgb->g = 0;
      if(allowB)rgb->b = 255;
      else rgb->b = 0;
      *d = 255;

      (*progFx)++;
    break;

    case 1:
      if(allowR)rgb->r--;
      if(allowG)rgb->g--;
      if(allowB)rgb->b--;
      (*d)--;

      if(*d == 100) (*progFx)++;
    break;

    case 2:
      if(allowR) rgb->r = 255;
      if(allowG) rgb->g = 255;
      if(allowB) rgb->b = 255;
      *d = 255;

      (*progFx)++;
    break;

    case 3:
      if(allowR)rgb->r--;
      if(allowG)rgb->g--;
      if(allowB)rgb->b--;
      (*d)--;

      if (*d == 0)
      {
        *progFx = 0;
        return true;
      }
    break;
  }
  return false;
}
bool EffectApplications::stock_appear           (ColourRGB *rgb, uint8_t *d, uint8_t *progFx, uint8_t colour)
{
  bool allowR = getColourClearance(colour, COLOUR_RED);
  bool allowG = getColourClearance(colour, COLOUR_GREEN);
  bool allowB = getColourClearance(colour, COLOUR_BLUE);

  switch (*progFx)
  {
    case 0:
      if(allowR)rgb->r = 255;
      else rgb->r = 0;
      if(allowG)rgb->g = 255;
      else rgb->g = 0;
      if(allowB)rgb->b = 255;
      else rgb->b = 0;
      *d = 255;

      (*progFx)++;
    break;

    case 1:
      if(allowR)rgb->r--;
      if(allowG)rgb->g--;
      if(allowB)rgb->b--;
      (*d)--;

      if(*d == 100) (*progFx)++;
    break;

    case 2:
      (*d)++;
      if(*d == 255) (*progFx)++;
    break;

    case 3:
      if(allowR)rgb->r = 255;
      if(allowG)rgb->g = 255;
      if(allowB)rgb->b = 255;
      *d = 255;

      (*progFx)++;
    break;

    case 4:
      if(allowR)rgb->r--;
      if(allowG)rgb->g--;
      if(allowB)rgb->b--;
      (*d)--;

      if (*d == 0) (*progFx)++;
    break;

    case 5:
      (*d)++;
      if (*d == 223) 
      {
        *progFx = 0;
        return true;
      }
    break;
  }
  return false;
}

bool EffectApplications::custom_static          (ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour)
{
  /*
  if (colour != COLOUR_CYCLE) c = colour;

  switch ((*progFx))
  {
  case 0:
  {
    rgb->r = customRGB[c]->r;
    rgb->g = customRGB[c]->g;
    rgb->b = customRGB[c]->b;

    d = 0;

    (*progFx)++;
  }
  break;

  case 1:
  {
    d++;

    if (d == 255) (*progFx)++;
  }
  break;

  default:
  {
    (*progFx) = 0;
    if(colour == COLOUR_CYCLE)
    {
      c++;
      if(c == AMOUNTOFCOLOURS) c = 0;
    }
  }
  break;
  }
  */
}

bool EffectApplications::special_rainbow        (ColourRGB *rgb, uint8_t *d, uint8_t *progFx)
{
  switch (*progFx)
  {
    case 0:
      rgb->r = 0;
      rgb->g = 0;
      rgb->b = 255;

      (*progFx)++;
    break;

    case 1:
      (rgb->r)++;
      if (rgb->r == 255) (*progFx)++;
    break;

    case 2:
      rgb->b--;
      if (rgb->b == 0) (*progFx)++;
    break;

    case 3:
      rgb->g++;
      if (rgb->g == 255) (*progFx)++;
    break;

    case 4:
      rgb->r--;
      if (rgb->r == 0) (*progFx)++;
    break;

    case 5:
      rgb->b++;
      if (rgb->b == 255) (*progFx)++;
    break;

    case 6:
      rgb->g--;
      if (rgb->g == 0)
      {
        *progFx = 0;
        return true;
      }
    break;

    default:
      *progFx = 0;
    break;
  }
  return false;
}
bool EffectApplications::special_fire           (ColourRGB rgb, uint8_t d, uint8_t c, uint8_t colour)
{
  /*
  if ((*progFx) == 0)
  {
    rgb->r = 255;
    rgb->g = random(0, 200);
    rgb->b = 0;
  }
  else if (rgb->g < 90)
  {
    rgb->g++;
  }
  (*progFx)++;

  if (rgb->g > 25 && random(0, offset) == 0)
  {
    rgb->g = rgb->g - 5;
  }

  if (g < 150 && offset > 5)
  {
    if (random(10, offset) > 10)
    {
      rgb->g = rgb->g + 6;
    }
  }
  */
}







