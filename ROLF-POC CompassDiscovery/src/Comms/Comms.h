#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <vector>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "setup.h"

#define SERVER_UDPPORT 4210
#define TIMEOUTATTEMPTS 10

#define RESERVATION_NONE 0
#define RESERVATION_MAIN 1
#define RESERVATION_LEDS 2

#define INTERVAL_SESSIONCHECK 60000         // 1 minute







  

class Comms
{
private:

    WiFiUDP udp;
    volatile uint8_t memoryReservedBy; //reservation to make sure memory is not accessed by multiple tasks at the same time
    std::vector<LedUpdate> ledUpdates; //Only when task_leds and task_main are not using the memory, transmisisons from buffer are put in here. This is to prevent memory corruption by multiple tasks accessing the same memory
    std::vector<LedUpdate> ledUpdates_buffer; //Quick dump so incoming transmissions are always stored
    uint64_t lastMillis_SessionCheck = 0;
    void reboot(String message);
    
    void receiveAndParse();

public:
    Comms();
    void tick();
    void connect();
    void transmit(String message);
    uint8_t *getModuleID();;
    std::vector<LedUpdate> getLedUpdates();
};

#endif