#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Askbutton/AskButton.h"
#include "modulemanager/moduleManager.h"

#define WIFICONNECTATTEMPTS 10



#define MESSAGE_CLCO_NEWCLIENTTEMPLATE  "NwClT" //NwClT(macAdress)(TemplateID)
#define MESSAGE_CLCO_NEWCLIENTFULL      "NwClF" //NwClF(macAdress)(heartPiece)(nl,nd)(el,ed)(sl,sd)(wl,wd)(ul,ud)(dl,dd)
#define MESSAGE_COCL_IDASSIGNMENT       "IDAss" //IDAss(moduleAdress)(sessionID)
#define MESSAGE_CLCO_CONNECTIONCHANGED  "ConCh" //ConCh(updateCode)
#define MESSAGE_COCL_UPDATEREQUEST      "UpReq" //UpReq
#define MESSAGE_COCL_NEWEFFECT          "NewFX" //NewFX(effectCode)
#define MESSAGE_DUPL_SESSIONCHECK       "SeChk" //SeChk(sessionID)

WiFiUDP udp;
ModuleManager moduleManager;

uint8_t currentSession = 0;
uint64_t lastSystemScanMillis = 0;






#define XFACTOR 100
BaseInfo getBaseInfo(uint8_t presetID) //Different presets are defined here. This should a temporary solution.
{
  switch (presetID)
  {
    case PRESET_1_DEBUGCROSS:
      return BaseInfo{PRESET_1_DEBUGCROSS,BASE_HEART_X,         2,2*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    0,0,            0,0};
    break;
    case PRESET_2_AllCross1:
      return BaseInfo{PRESET_2_AllCross1,BASE_HEART_XUPDOWN,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR};
    break;
  }
  Serial.println("Preset not found");
  return BaseInfo{0,0,0,0,0,0,0,0,0,0,0,0,0};
}

void udp_transmit(String message, String clientIP)
{
  Serial.print(F("UDP TRANSMIT: "));
  Serial.println(message);
  udp.beginPacket(clientIP.c_str(), SERVER_UDPPORT);
  udp.write((uint8_t *)message.c_str(), message.length());
  udp.endPacket();
}
void udp_receive()
{
  //Message Receiving
  if (udp.parsePacket())
  { 
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = 0;
    String remoteIp = udp.remoteIP().toString();
    //Serial.printf("UDP RECEIVED:  %s\n", incomingPacket);

    //Message handling
    String message = String(incomingPacket);
    message.trim();
    
    Serial.print(F("UDP RECEIVED: "));
    Serial.println(message);
    

    if (message.startsWith(MESSAGE_CLCO_NEWCLIENTFULL))//Message contains 8 pieces of data: macAdress, heartPiece, n, e, s, w, u, d
    {
      Serial.println("New client full message received. Not implemented");
    }
    if (message.startsWith(MESSAGE_CLCO_NEWCLIENTTEMPLATE))//Message contains 8 pieces of data: macAdress, heartPiece, n, e, s, w, u, d
    {
      String macAdress = message.substring(6, 22);
      uint8_t templateID      = (uint8_t)message[22];

      BaseInfo baseInfo = getBaseInfo(templateID);

      uint8_t moduleID = moduleManager.addNewModule(macAdress, remoteIp, baseInfo);
      if (moduleID == 0) Serial.println("New module could not be added");
      udp_transmit(MESSAGE_COCL_IDASSIGNMENT + String(moduleID) + String(currentSession), remoteIp);
    }

    if (message.startsWith(MESSAGE_CLCO_CONNECTIONCHANGED)) 
    {
      //Original code: String(char(moduleAdress)+ char(direction) + char(neighborAdress) + char(neighborDirection)

      uint8_t moduleAdress      = (uint8_t)message[5];
      uint8_t direction         = (uint8_t)message[6];
      uint8_t neighborAdress    = (uint8_t)message[7];
      uint8_t neighborDirection = (uint8_t)message[8];   

      Serial.print("Connection changed at module ");
      Serial.print(moduleAdress);
      Serial.print(": in direction ");
      Serial.print(directionToString(direction));
      Serial.print(" to module ");
      Serial.print(neighborAdress);
      Serial.print(" on their direction ");
      Serial.println(directionToString(neighborDirection));

      moduleManager.updateModuleConnection(moduleAdress, direction, neighborAdress, neighborDirection);
    }

    if (message.startsWith(MESSAGE_DUPL_SESSIONCHECK)) //Session check, respond with current session
    {
      udp_transmit(MESSAGE_DUPL_SESSIONCHECK + String(currentSession), remoteIp);
    }
    // Add else if conditions here for other codephrases
  }
}
void udp_connect()
{
  Serial.println("(Re)Connecting to WiFi");

  uint8_t attempts = 0;
  WiFi.setHostname("GLOWMASTER");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (attempts++ > WIFICONNECTATTEMPTS)
    {
      Serial.println("WIFI connection failed. Restarting...");
      ESP.restart();
    }
    delay(1000);
    Serial.print("Attempting connection to ");
    Serial.print(SSID);
    Serial.print(" (");
    Serial.print(attempts);
    Serial.print("/");
    Serial.print(WIFICONNECTATTEMPTS);
    Serial.println(")");
  }
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  // Start UDP
  udp.begin(SERVER_UDPPORT);
  Serial.print("Now listening at IP ");
  Serial.print(WiFi.localIP());
  Serial.print(", UDP port ");
  Serial.println(SERVER_UDPPORT);
}
void udp_tick()
{
  // Check if the WiFi is still connected, if not, reconnect
  if (WiFi.status() != WL_CONNECTED) udp_connect();

  udp_receive();
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("---===Setup started===---"));

  currentSession = (uint8_t)esp_random();
  Serial.println("Session ID: " + String(currentSession));

  udp_connect();

  Serial.println(F("---===Setup finished===---"));  
}

void loop()
{
  uint64_t currentMillis = millis();

  udp_tick();
  moduleManager.tick();
  /*
  if (currentMillis - lastSystemScanMillis > INTERVAL_MODULECHANGESCAN)
  {
    lastSystemScanMillis = currentMillis;
    Serial.println("Scanning module changes... (Not Implemented)");
  }
  */
}