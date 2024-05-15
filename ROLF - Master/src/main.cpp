#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Askbutton/AskButton.h"
#include "modulemanager/moduleManager.h"


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

    if (message.startsWith(MESSAGE_CLCO_NEWCLIENT))//New client, add to module list and respond with ID
    {
      String mac = message.substring(strlen(MESSAGE_CLCO_NEWCLIENT));
      Serial.println("Parsed MAC: " + mac);

      uint8_t newModuleID = moduleManager.addNewModule(mac, remoteIp, 1);
      udp_transmit(MESSAGE_COCL_IDASSIGNMENT + String(newModuleID), remoteIp);
      udp_transmit(MESSAGE_DUPL_SESSIONCHECK + String(currentSession), remoteIp);
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