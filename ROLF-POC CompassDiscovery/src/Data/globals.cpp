#include "globals.h"

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