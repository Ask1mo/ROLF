#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Askbutton/AskButton.h"

#include "connectedModule/connectedModule.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <vector>

WiFiServer server(80);

std::vector<ConnectedModule*> connectedModules;

void addNewModule(String macAdress, uint8_t shape)
{
  uint16_t newModuleID = connectedModules.size()+1;
  connectedModules.push_back(new ConnectedModule(macAdress, newModuleID, shape));
  Serial.print("New module added with ID: ");
  Serial.println(newModuleID);
  Serial.print("MAC address: ");
  Serial.println(macAdress);
  Serial.print("Shape: ");
  Serial.println(shape);
}

void setup() {
  Serial.begin(115200);

  // Set up the access point
  WiFi.softAP("Controller", "password");

  // Start the server
  server.begin();


}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("New client.");
    // A client has connected, you can now communicate with it
    client.println("Hello, client! Please send your MAC address.");

    
    // Wait for client to send MAC address
    while (client.available() == 0) {
      delay(1);
      Serial.print(".");
    }
    
    // Read the MAC address sent by the client
    String macAddress = client.readStringUntil('\n');
    
    client.stop();
    addNewModule(macAddress, 1);
  }
}