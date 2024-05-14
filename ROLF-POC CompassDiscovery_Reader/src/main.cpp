#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define TIMEOUTATTEMPTS 10


// Replace with your network credentials
const char* ssid = "GLOWII";
const char* password = "AskimoGlow";

WiFiUDP Udp;
#define SERVER_IP "192.168.137.199"
#define SERVER_UDPPORT 4210  // local port to listen on

void sendUDP(String message)
{
  Udp.beginPacket(SERVER_IP, SERVER_UDPPORT);
  Udp.write((uint8_t *)message.c_str(), message.length());
  Udp.endPacket();
}

void connectToWiFi()
{
  uint8_t attempts = 0;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (attempts++ > TIMEOUTATTEMPTS)
    {
      Serial.println("WIFI connection failed. Restarting...");
      ESP.restart();
    }
    delay(1000);
    Serial.print("Attempting connection to ");
    Serial.print(ssid);
    Serial.print(" (");
    Serial.print(attempts);
    Serial.print("/");
    Serial.print(TIMEOUTATTEMPTS);
    Serial.println(")");
  }
  Serial.println("Connected to WiFi");
  // Start UDP
  Udp.begin(SERVER_UDPPORT);

  // Send a message to the server
  String identMessage = "Hello: " + WiFi.macAddress();
  sendUDP(identMessage);
}


void setup()
{
  Serial.begin(115200);
  connectToWiFi();
}

void loop()
{
  // Check if the WiFi is still connected
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi connection lost. Reconnecting...");
    connectToWiFi();
  }

  // Check if there are any UDP packets available
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // Read the packet into packetBuffer
    char packetBuffer[255];
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;

    // Print the packet
    Serial.println("Received packet from server: ");
    Serial.println(packetBuffer);
  }
}