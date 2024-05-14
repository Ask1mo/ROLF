#include <Arduino.h>
#include <WiFi.h>

#define TIMEOUTATTEMPTS 10

void connectToWiFi()
{
  uint8_t attempts = 0;
  WiFi.begin("Controller", "password");
  while (WiFi.status() != WL_CONNECTED)
  {
    if (attempts++ > TIMEOUTATTEMPTS)
    {
      Serial.println("WIFI connection failed. Restarting...");
      ESP.restart();
    }
    delay(1000);
    Serial.print("Attempting connection (");
    Serial.print(attempts);
    Serial.print("/");
    Serial.print(TIMEOUTATTEMPTS);
    Serial.println(")");
  }

  // Connect to the server
  WiFiClient client;
  if (!client.connect(WiFi.gatewayIP(), 80))
  {
    Serial.println("Connection to server failed");
    ESP.restart();
  }
}

void setup() {
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

  // Check if the server is available
  WiFiClient client;
  if (client.connect(WiFi.gatewayIP(), 80))
  {
    // Wait for the server to send a message
    while (client.available() == 0) {
      delay(1);
      Serial.print(".");
    }

    // Read the server's message
    String message = client.readStringUntil('\n');
    Serial.println("Received message from server: " + message);

    // If the server asked for the MAC address, send it
    if (message.startsWith("Hello, client! Please send your MAC address."))
    {
      String macAddress = WiFi.macAddress();
      client.println(macAddress);
      Serial.println("Sent MAC address to server: " + macAddress);
    }

    client.stop();
  }

  delay(1000);
}