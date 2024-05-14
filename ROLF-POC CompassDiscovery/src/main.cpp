#include <Arduino.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Askbutton/AskButton.h"
#include "connectorManager/ConnectorManager.h"




uint8_t moduleAdress = 0;
ConnectorManager *connectorManager;
AskButton *button;
uint8_t discoveryMode;
uint8_t compassRequestDirection = 0;
uint8_t ledLevel = 255;
uint64_t currentMillis = 0;
bool startFlashRegistered = false;
uint64_t lastdStartFlashMillis = 0;
WiFiUDP Udp;

void triggerLedSync()
{
  Serial.println(F("!!!!!!!!!LED Sync triggered!!!!!!!!"));
  delay(1000);
  connectorManager->sendSyncSignal();
  ledLevel = 255;
}
void sendUDP(String message)
{
  Udp.beginPacket(SERVER_IP, SERVER_UDPPORT);
  Udp.write((uint8_t *)message.c_str(), message.length());
  Udp.endPacket();
}
void connectToWiFi()
{
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
  Serial.println("Connected to WiFi");
  // Start UDP
  Udp.begin(SERVER_UDPPORT);

  // Send a message to the server
  String identMessage = "Hello: " + WiFi.macAddress();
  sendUDP(identMessage);
}



void setup()
{
  Serial.begin(BAUDRATE_MONITOR);
  Serial.println(F("---===Setup started===---"));

  connectToWiFi();

  button = new AskButton(0, 1000);

  //Get new module adress. Can't be 0 or 1
  while (moduleAdress <= 1) moduleAdress = (uint8_t)esp_random(); //0-4096 to 0-255
  Serial.print(F("Module adress: "));
  Serial.println(moduleAdress);
  
  //Prepare fake LEDS
  pinMode(PIN_LED, OUTPUT);
  Serial.println(F("Leds started"));

  //Connectors setup
  connectorManager = new ConnectorManager(&moduleAdress);
  Serial.println(F("ConnectionManager created"));

  uint32_t *memcheck;
  memcheck = (uint32_t *)malloc(1000);
  Serial.print(F("Memory check: "));
  Serial.println((uint32_t)memcheck);
  

  

  Serial.println(F("---===Setup done===---"));  
}

void loop()
{
  //LED controls
  if (ledLevel > 0)
  {
    ledLevel--;
    analogWrite(PIN_LED, ledLevel);
    Serial.print(F("LED Level: "));
    Serial.println(ledLevel); 
  }

  //Connector comms
  connectorManager->tick();

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
