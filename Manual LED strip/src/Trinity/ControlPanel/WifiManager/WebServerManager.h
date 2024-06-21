// WebServerManager.h
#include <WebServer.h>
#include <WiFi.h>

#define SSID "Askimo"
#define PASSWORD "CringeDingus2003!"

class WebServerManager
{
public:
  WebServerManager(uint8_t panelAmount, uint8_t canvasWidth, uint8_t canvasHeight);
  void tick();

private:
  WebServer server;
  WiFiClient client;

  uint8_t panelAmount;
  uint8_t canvasWidth;
  uint8_t canvasHeight;

  void handleRoot();
  void handlePOT();
};