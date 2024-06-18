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

String directionToString(uint8_t compassDirection)
{
    switch (compassDirection)
    {
        case DIRECTION_NONE:
            return "DIRECTION_NONE";
        break;

        case DIRECTION_NORTH:
            return "North";
        break;

        case DIRECTION_EAST:
            return "East ";
        break;

        case DIRECTION_SOUTH:
            return "South";
        break;

        case DIRECTION_WEST:
            return "West ";
        break;

        case DIRECTION_UP:
            return "Up   ";
        break;

        case DIRECTION_DOWN:
            return "Down ";
        break;
    }
    return "ERROR_DIRECTIONTOSTRING";
}


String colonMacToSixCharMac(String colonMac)
{
    if (colonMac.length() != 17)
    {
        Serial.print("Error:colonMacToSixCharMac - Mac adress is not 17 characters long: ");
        Serial.println(colonMac);
        return "??????";
    }

    colonMac.replace(":", ""); // Remove colons from MAC address
    return colonMac;
}


String sixCharMacToColonMac(String sixCharMac)
{
    if (sixCharMac.length() != 6)
    {
        Serial.print("Error:sixCharMacToTwelveCharMac - Mac adress is not 6 characters long: ");
        Serial.println(sixCharMac);
        return "????????????";
    }

    String newMac = "";

    for (uint8_t i = 0; i < 6; i++)
    {
        //Place every char as hex in new string. Convert every char to a hex (So 2 chars)
        newMac += String(sixCharMac[i], HEX);

        //Add colon after every two characters
        if (i != 5)
        {
            newMac += ":";
        }
    }

    return newMac;
}

void printStringAsHex(String data)
{
    Serial.print("(");
    for (uint8_t i = 0; i < data.length(); i++)
    {
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println(")");
}