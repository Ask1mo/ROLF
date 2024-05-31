#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "modulemanager/moduleManager.h"
#include "comms/Comms.h"

WiFiUDP       udp;
ModuleManager moduleManager;
Comms         comms;
uint64_t      lastSystemScanMillis = 0;

void setup();
void loop();

#endif