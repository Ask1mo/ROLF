#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>
#include <map>
#include "TransmissionTypes.h"

#define IDENTLENGTH 5
#define COMPLETEDBUFFERSIZE



class Comms
{
    private:
    uint8_t                             readyTransmissionType;
    uint8_t                             transmissionData[IDENTLENGTH];

    struct Transmission_LedManager      *buffer_LedManager;
    struct Transmission_Panel           *buffer_Panel;
    struct Transmission_Diode           *buffer_Diode;
    struct Transmission_CustomPalette   *buffer_CustomPalette;
    struct Transmission_SleepTimer      *buffer_SleepTimer;
    int8_t                              buffer_LightSensorOffset;

    uint8_t                             decodeTransmissionType();   
    uint8_t                             waitAndRead();
    bool                                doTransmissionEndCheck();
    void                                printBuffer();

    public:
    Comms();
    void                            tick();
    void                            transmit(String data);
    uint8_t                         getReadyTransmissionType();
    Transmission_LedManager         getTransmission_LedManager();
    Transmission_Panel              getTransmission_Panel();
    Transmission_Diode              getTransmission_Diode();
    Transmission_CustomPalette      getTransmission_CustomPalette();
    Transmission_SleepTimer         getTransmission_SleepTimer();
    int8_t                          getTransmission_LightSensorOffset();
};
#endif