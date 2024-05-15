#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Askbutton/AskButton.h"
#include "modulemanager/moduleManager.h"

#define MESSAGE_CLCO_NEWCLIENT          "NewCl" //NewCl(macAdress)(heartPiece)(n)(e)(s)(w)(u)(d)
#define MESSAGE_COCL_IDASSIGNMENT       "IDAss" //IDAss(moduleAdress)(sessionID)
#define MESSAGE_CLCO_CONNECTIONCHANGED  "ConCh" //ConCh(updateCode)
#define MESSAGE_COCL_UPDATEREQUEST      "UpReq" //UpReq
#define MESSAGE_COCL_NEWEFFECT          "NewFX" //NewFX(effectCode)
#define MESSAGE_DUPL_SESSIONCHECK       "SeChk" //SeChk(sessionID)

WiFiUDP udp;
ModuleManager moduleManager;

uint8_t currentSession = 0;
uint64_t lastSystemScanMillis = 0;



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
    

    if (message.startsWith(MESSAGE_CLCO_NEWCLIENT))//Message contains 8 pieces of data: macAdress, heartPiece, n, e, s, w, u, d
    {
      String macAdress = message.substring(6, 22);
      BaseInfo baseInfo = {0, 0, 0, 0, 0, 0, 0};
      baseInfo.heartPiece = message.substring(22, 25).toInt();
      baseInfo.northPipe = message.substring(25, 26).toInt();
      baseInfo.eastPipe = message.substring(26, 27).toInt();
      baseInfo.southPipe = message.substring(27, 28).toInt();
      baseInfo.westPipe = message.substring(28, 29).toInt();
      baseInfo.upPipe = message.substring(29, 30).toInt();
      baseInfo.downPipe = message.substring(30, 31).toInt();

      uint8_t moduleID = moduleManager.addNewModule(macAdress, remoteIp, baseInfo);
      if (moduleID == 0) Serial.println("New module could not be added");
      udp_transmit(MESSAGE_COCL_IDASSIGNMENT + String(moduleID) + String(currentSession), remoteIp);
    }

    if (message.startsWith(MESSAGE_CLCO_CONNECTIONCHANGED)) 
    {
      //Original code: String(char(moduleAdress)+ char(direction) + char(neighborAdress) + char(neighborDirection)

      uint8_t moduleAdress = message.charAt(6);
      uint8_t direction = message.charAt(7);
      uint8_t neighborAdress = message.charAt(8);
      uint8_t neighborDirection = message.charAt(9);   

      Serial.print("Connection changed at module ");
      Serial.print(moduleAdress);
      Serial.print(": in direction ");
      Serial.print(direction);
      Serial.print(" to module ");
      Serial.print(neighborAdress);
      Serial.print(" on their direction ");
      Serial.println(neighborDirection);
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
  WiFi.setHostname("GLOWMASTER");
  // Connect to Wi-Fi
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
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
  /*
  if (currentMillis - lastSystemScanMillis > INTERVAL_MODULECHANGESCAN)
  {
    lastSystemScanMillis = currentMillis;
    Serial.println("Scanning module changes... (Not Implemented)");
  }
  */
}