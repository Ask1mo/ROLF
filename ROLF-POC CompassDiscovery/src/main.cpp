#include <Arduino.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Askbutton/AskButton.h"
#include "connectorManager/ConnectorManager.h"


WiFiUDP udp;
ConnectorManager *connectorManager;

uint8_t moduleAdress = 0;
uint8_t sessionID = 0;

uint64_t currentMillis = 0;
uint64_t lastMillis_SessionCheck = 0;




#define SYSTEMSTATE_CONNECTING_WIFI       0
#define SYSTEMSTATE_CONNECTING_CONTROLLER 1
#define SYSTEMSTATE_CONNECTED             2

void reboot(String message)
{
  Serial.print(F("MANUAL REBOOT: "));
  Serial.println(message);
  ESP.restart();
}

void    udp_transmit(String message)
{
  Serial.print(F("UDP TRANSMIT: "));
  Serial.println(message);
  udp.beginPacket(SERVER_IP, SERVER_UDPPORT);
  udp.write((uint8_t *)message.c_str(), message.length());
  udp.endPacket();
}
void    udp_receive()
{
  //Message Receiving
  if (udp.parsePacket())
  { 
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = 0;
    //Serial.printf("UDP RECEIVED:  %s\n", incomingPacket);


    //Message handling
    String message = String(incomingPacket);
    message.trim();

    if (message.startsWith(MESSAGE_COCL_IDASSIGNMENT))
    {
      moduleAdress = message.substring(strlen(MESSAGE_COCL_IDASSIGNMENT)).toInt();
      Serial.println("New ID received: " );
      Serial.println(moduleAdress);
    }
  
    if (message.startsWith(MESSAGE_COCL_UPDATEREQUEST))
    {
      Serial.println("Update requested, not implemented");
    }

    if (message.startsWith(MESSAGE_COCL_NEWEFFECT))
    {
      Serial.println("New effect received, not implemented");
    }

    if (message.startsWith(MESSAGE_DUPL_SESSIONCHECK))
    {
      uint8_t newSessionID = (uint8_t)message.substring(strlen(MESSAGE_DUPL_SESSIONCHECK)).toInt();
      Serial.print("Parsed session: ");
      Serial.println(newSessionID);

      if (sessionID == 0) sessionID = newSessionID;
      else if (sessionID != newSessionID) reboot("Session changed. Restarting...");
    }
  }
}
void    udp_connect()
{
  Serial.println("(Re)Connecting to WiFi");

  uint8_t attempts = 0;
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (attempts++ > TIMEOUTATTEMPTS)
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
    Serial.print(TIMEOUTATTEMPTS);
    Serial.println(")");
  }
  Serial.println("WiFi Connected");


  udp.begin(SERVER_UDPPORT);
  String identMessage = MESSAGE_CLCO_NEWCLIENT + WiFi.macAddress();
  udp_transmit(identMessage);

  uint16_t timeout = 10000;
  while (moduleAdress == 0 || sessionID == 0) 
  {
    if (timeout-- == 0) reboot("Server connection failed. Restarting...");
    delay(1);
    udp_receive();
  }
  Serial.println("Server Connected");
}
void    udp_tick()
{
  // Check if the WiFi is still connected, if not, reconnect
  if (WiFi.status() != WL_CONNECTED) udp_connect();

  udp_receive();
}

void setup()
{
  Serial.begin(BAUDRATE_MONITOR);
  Serial.println(F("---===Setup started===---"));

  udp_connect();

  connectorManager = new ConnectorManager(&moduleAdress);

  Serial.println(F("---===Setup finished===---"));  
}
void loop()
{
  currentMillis = millis();

  if (currentMillis - lastMillis_SessionCheck > INTERVAL_SESSIONCHECK)
  {
    lastMillis_SessionCheck = currentMillis;
    udp_transmit(MESSAGE_DUPL_SESSIONCHECK+String(sessionID));
  }

  connectorManager->tick();
  udp_tick();
}
