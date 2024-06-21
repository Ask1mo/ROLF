#include "globals.h"

BaseInfo getBaseInfo(uint8_t presetID)
{
  switch (presetID)
  {
    case PRESET_1_DEBUGCROSS:
      return BaseInfo{PRESET_1_DEBUGCROSS,BASE_HEART_X,         2,2*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    0,0,            0,0};
    break;
    case PRESET_2_AllCross1:
      return BaseInfo{PRESET_2_AllCross1,BASE_HEART_XUPDOWN,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR};
    break;
    
    case PRESET_3_STRIJP_SINGLEPIPE:
      return BaseInfo{PRESET_3_STRIJP_SINGLEPIPE,BASE_HEART_X,  1,28,           0,0,            0,0,            0,0,            0,0,            0,0};
    break;
    case PRESET_253_STRIJP_HORNWEIRD:
      return BaseInfo{PRESET_253_STRIJP_HORNWEIRD,BASE_HEART_X, 2,24,           0,0,            0,0,            0,0,            0,0,            0,0};
    break;
    case PRESET_254_STRIJP_HORNLONG:
      return BaseInfo{PRESET_254_STRIJP_HORNLONG,BASE_HEART_X,  2,43,           0,0,            0,0,            0,0,            0,0,            0,0};
    case PRESET_255_Horn:
      return BaseInfo{PRESET_255_Horn,0b10000000,    1,1*XFACTOR,    0,0,    0,0,    0,0,    0,0,    0,0};
    break;
  }
  Serial.println("Preset not found");
  return BaseInfo{0,0,0,0,0,0,0,0,0,0,0,0,0};
}


String directionToString(uint8_t direction)
{
    switch (direction)
    {
        case DIRECTION_NONE:
        return "None0 ";
        case DIRECTION_NORTH:
        return "North1";
        case DIRECTION_EAST:
        return "East2 ";
        case DIRECTION_SOUTH:
        return "South3";
        case DIRECTION_WEST:
        return "West4 ";
        case DIRECTION_UP:
        return "Up5   ";
        case DIRECTION_DOWN:
        return "Down6 ";
        default:
        return "DIRERR";
    }
}