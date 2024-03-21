// WebServerManager.cpp
#include "WebServerManager.h"
#include "webpage.html"

WebServerManager::WebServerManager(uint8_t panelAmount, uint8_t canvasWidth, uint8_t canvasHeight)
{
  this->panelAmount = panelAmount;
  this->canvasWidth = canvasWidth;
  this->canvasHeight = canvasHeight;

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting to WiFi");
  /*Tijdelijk
  while(WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(500); Serial.print(".");
  }
  */
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/",        std::bind(&WebServerManager::handleRoot, this));
  server.on("/readPOT", std::bind(&WebServerManager::handlePOT, this));
  server.begin();
  Serial.println("HTTP server started");
}

void WebServerManager::tick()
{
  server.handleClient();
}
void WebServerManager::handleRoot()
{
  String html = webpageCode;
  html.replace("{canvasWidth}", String(canvasWidth));
  html.replace("{canvasHeight}", String(canvasHeight));
  server.send(200, "text/html", html);
}
void WebServerManager::handlePOT()
{
  /*
  String POTval = "color: rgb(";
  POTval += String(random(0, 255));
  POTval += String(", ");
  POTval += String(random(0, 255));
  POTval += String(", ");
  POTval += String(random(0, 255));
  POTval += String(");");
  */
  String POTval = String(random(0, 360));
  server.send(200, "text/plane", POTval);
}