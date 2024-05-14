#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <vector>

#include "Askbutton/AskButton.h"
#include "connectedModule/connectedModule.h"

// Replace with your network credentials
const char* ssid = "GLOWII";
const char* password = "AskimoGlow";

#define SERVER_UDPPORT 4210  // local port to listen on

WiFiUDP udp;

std::vector<ConnectedModule*> connectedModules ;


#define SYSTEMSCAN_INTERVAL 3000
uint64_t lastSystemScanMillis = 0;


void addNewModule(String macAdress, String ipAdress, uint8_t shape)
{
  uint16_t newModuleID = connectedModules.size()+1;
  connectedModules.push_back(new ConnectedModule(macAdress, ipAdress, newModuleID, shape));
  Serial.print("New module added with ID: ");
  Serial.println(newModuleID);
  Serial.print("MAC address: ");
  Serial.println(macAdress);
  Serial.print("Shape: ");
  Serial.println(shape);
}

void sendUDP(String message, String clientIP)
{
  udp.beginPacket(clientIP.c_str(), SERVER_UDPPORT);
  udp.write((uint8_t *)message.c_str(), message.length());
  udp.endPacket();
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
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

void loop()
{
  uint64_t currentMillis = millis();

  if (currentMillis - lastSystemScanMillis > SYSTEMSCAN_INTERVAL)
  {
    lastSystemScanMillis = currentMillis;
    Serial.println("Scanning module changes...");
    //scanForNewModules();

    for (int i = 0; i < connectedModules.size(); i++)
    {
      sendUDP("UpReq", connectedModules[i]->getIpAdress());
    }
  }

  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    IPAddress remoteIp = udp.remoteIP();
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, remoteIp.toString().c_str(), udp.remotePort());
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

    // parse MAC address from incomingPacket here
    String message = String(incomingPacket);
    int colonIndex = message.indexOf(':');
    if (colonIndex != -1) {
      message = message.substring(colonIndex + 1);
      message.trim();
      String mac = message;
      Serial.println("Parsed MAC: " + mac);
      addNewModule(mac, remoteIp.toString(), 1);
    }
  }
}