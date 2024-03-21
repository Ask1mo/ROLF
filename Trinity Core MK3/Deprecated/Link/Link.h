/*
#ifndef LINK_H
#define LINK_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Webserver.h>
#include "index.html"

//const char* ssid = "Askimo";
//const char* password = "CringeDingus2003!";
#define SSID "Askimo"
#define PASSWORD "CringeDingus2003!"
#define TIMEOUTTIME 2000
#define PIN_A 13
#define PIN_B 14


void            handleRoot(WebServer *server);
void            handleADC(WebServer *server);





class Link
{
private:
    
    String          header;
    String          output26State = "off";
    String          output27State = "off";
    unsigned long   currentTime;
    unsigned long   previousTime = 0; 
    bool            connected;
    //void            printPage(WebServer client);
    
    
public:
    Link();
    void tick();
};

#endif
*/



