#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "modulemanager/moduleManager.h"
#include "comms/Comms.h"


#define DURATION_HORNTOHORNTALK 3000

WiFiUDP       udp;
ModuleManager moduleManager;
Comms         comms;
uint64_t      lastMillis_SystemScan = 0;
uint64_t      lastMillis_HornToHornTalkBegin = 0;
uint8_t       sessionID = 0;

void setup();
void loop();

#endif