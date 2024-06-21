#include "Comms.h"

Comms::Comms()
{
    Serial.println(F("Comms: Starting"));

    readyTransmissionType = TRANSMISSION_IN_NONE;

    buffer_LedManager       = new struct Transmission_LedManager;
    buffer_Panel            = new struct Transmission_Panel;
    buffer_Diode            = new struct Transmission_Diode;
    buffer_CustomPalette    = new struct Transmission_CustomPalette;
    for (uint8_t i = 0; i < AMOUNTOFCOLOURS; i++) buffer_CustomPalette->customRGB[i]= new struct ColourRGB;
    buffer_SleepTimer       = new struct Transmission_SleepTimer;
    buffer_LightSensorOffset= 0;

    Serial.println(F("Comms: Started"));
}

//Private
uint8_t Comms::decodeTransmissionType()
{
    if(!memcmp(transmissionData, "Reque", IDENTLENGTH)) return TRANSMISSION_IN_REQUEST;
    if(!memcmp(transmissionData, "Ident", IDENTLENGTH)) return TRANSMISSION_IN_IDENT;
    if(!memcmp(transmissionData, "LedMa", IDENTLENGTH)) return TRANSMISSION_IN_LEDMANAGER;
    if(!memcmp(transmissionData, "Panel", IDENTLENGTH)) return TRANSMISSION_IN_PANEL;
    if(!memcmp(transmissionData, "Diode", IDENTLENGTH)) return TRANSMISSION_IN_DIODE;
    if(!memcmp(transmissionData, "Custo", IDENTLENGTH)) return TRANSMISSION_IN_CUSTOMPALETTES;
    if(!memcmp(transmissionData, "Sleep", IDENTLENGTH)) return TRANSMISSION_IN_SLEEPTIMER;
    if(!memcmp(transmissionData, "Light", IDENTLENGTH)) return TRANSMISSION_IN_LIGHTSENSOR;
    return TRANSMISSION_IN_NONE;
}
uint8_t Comms::waitAndRead()
{
    while (!Serial.available()) delay(1);
    return (Serial.read());
}
bool    Comms::doTransmissionEndCheck()
{
    for(uint8_t i = 0; i < IDENTLENGTH; i++)
    {
        transmissionData[i] = waitAndRead();
    }
    
    //printBuffer();
    if(!memcmp(transmissionData, "Clear", IDENTLENGTH)) return true;

    return false;
}
void    Comms::printBuffer()
{
    Serial.println(F("[TRI-PRINTBUF "));
    for(uint8_t i = 0; i < IDENTLENGTH; i++)
    {
        Serial.write(transmissionData[i]);
    }
    Serial.println("]");
}

//Public
void                            Comms::tick()
{
    while(Serial.available() > 0 && readyTransmissionType == TRANSMISSION_IN_NONE)
    {
        for(byte i = 0; i < IDENTLENGTH-1; i++)
        {
            transmissionData[i] = transmissionData[i+1];
        }
        transmissionData[IDENTLENGTH-1] = Serial.read();

        uint8_t receivedTransmissionType = decodeTransmissionType();
        //Serial.print(F("Transmission "));
        //Serial.println(receivedTransmissionType);

        switch (receivedTransmissionType)
        {
            case TRANSMISSION_IN_LEDMANAGER:
            {
                buffer_LedManager->brightness        = waitAndRead();
                buffer_LedManager->speed            = waitAndRead();
            }
            break;
            case TRANSMISSION_IN_PANEL:
            {
                buffer_Panel->panelNumber         = waitAndRead();
                buffer_Panel->vfxData.effect      = waitAndRead();
                buffer_Panel->vfxData.colour      = waitAndRead();
                buffer_Panel->vfxData.offset      = waitAndRead();
                buffer_Panel->vfxData.speed       = waitAndRead();
                buffer_Panel->vfxData.repeat      = true;
                Serial.println("DD");
            }
            break;
            
            case TRANSMISSION_IN_DIODE:
            {
                buffer_Diode->panelNumber         = waitAndRead();
                buffer_Diode->diodeNumber         = waitAndRead();
                buffer_Diode->vfxData.effect      = waitAndRead();
                buffer_Diode->vfxData.colour      = waitAndRead();
                buffer_Diode->vfxData.offset      = waitAndRead();
                buffer_Diode->vfxData.speed       = waitAndRead();
                buffer_Diode->vfxData.repeat      = true;
                Serial.println("DD");
            }
            break;
            case TRANSMISSION_IN_CUSTOMPALETTES:
            {
                buffer_CustomPalette->slot = waitAndRead();

                for (uint8_t i = 0; i < AMOUNTOFCOLOURS; i++)
                {
                    buffer_CustomPalette->customRGB[i]->r = waitAndRead();
                    buffer_CustomPalette->customRGB[i]->g = waitAndRead();
                    buffer_CustomPalette->customRGB[i]->b = waitAndRead();
                }
            }
            break;
            case TRANSMISSION_IN_SLEEPTIMER:
            {
                buffer_SleepTimer->timerID   = waitAndRead();
                buffer_SleepTimer->hour      = waitAndRead();
                buffer_SleepTimer->minute    = waitAndRead();
                buffer_SleepTimer->enabled   = waitAndRead();
            }
            break;
            case TRANSMISSION_IN_LIGHTSENSOR:
            {
                buffer_LightSensorOffset = waitAndRead();
            }
            break;
        }

        if(receivedTransmissionType != TRANSMISSION_IN_NONE)
        {
            if (doTransmissionEndCheck())
            {
                readyTransmissionType = receivedTransmissionType;
                Serial.println(F("[TRI-TRANS-ACK]"));
            }
            else
            {
                Serial.println(F("[TRI-TRANS-ERR]"));
            }
        }
        

        if(!Serial.available())
        {            
            delay(1); //Wait a bit in case the transmisison was not actually finished
        }
    }
}
void                            Comms::transmit(String data)
{
    Serial.print(data);
    Serial.println("Clear");
}
uint8_t                         Comms::getReadyTransmissionType()
{
    if(readyTransmissionType == TRANSMISSION_IN_REQUEST || readyTransmissionType == TRANSMISSION_IN_IDENT)
    {
        uint8_t transmissionTypeToSend = readyTransmissionType;
        readyTransmissionType = TRANSMISSION_IN_NONE;
        return transmissionTypeToSend;
    }
    return readyTransmissionType;
}
Transmission_LedManager         Comms::getTransmission_LedManager()
{
    readyTransmissionType = TRANSMISSION_IN_NONE;
    return *buffer_LedManager;
}
Transmission_Panel              Comms::getTransmission_Panel()
{
    readyTransmissionType = TRANSMISSION_IN_NONE;
    return *buffer_Panel;
}
Transmission_Diode              Comms::getTransmission_Diode()
{
    readyTransmissionType = TRANSMISSION_IN_NONE;
    return *buffer_Diode;
}
Transmission_CustomPalette      Comms::getTransmission_CustomPalette()
{
    readyTransmissionType = TRANSMISSION_IN_NONE;
    return *buffer_CustomPalette;
}
Transmission_SleepTimer         Comms::getTransmission_SleepTimer()
{
    readyTransmissionType = TRANSMISSION_IN_NONE;
    return *buffer_SleepTimer;
}
int8_t                          Comms::getTransmission_LightSensorOffset()
{
    readyTransmissionType = TRANSMISSION_IN_NONE;
    return buffer_LightSensorOffset;
}