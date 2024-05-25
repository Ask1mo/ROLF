#include <Arduino.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Askbutton/AskButton.h"
#include "connectorManager/ConnectorManager.h"
#include "trinity/Trinity.h"
#include "setup.h"

// define two tasks for Blink & AnalogRead
TaskHandle_t Task1;
TaskHandle_t Task2;
void task_main( void *pvParameters );
void task_leds( void *pvParameters );




WiFiUDP udp;
ConnectorManager *connectorManager;
Trinity *trinity;

uint8_t moduleAdress = 0;
uint8_t sessionID = 0;

uint64_t currentMillis = 0;
uint64_t lastMillis_SessionCheck = 0;

#define PULSELENGTH_ID 5
#define INTERVAL_SESSIONCHECK 60000         // 1 minute


#define SYSTEMSTATE_CONNECTING_WIFI       0
#define SYSTEMSTATE_CONNECTING_CONTROLLER 1
#define SYSTEMSTATE_CONNECTED             2

#define TIMEOUTATTEMPTS 10

#define MESSAGE_CLCO_NEWCLIENT          "NewCl" //NewCl(macAdress)(heartPiece)(n)(e)(s)(w)(u)(d)
#define MESSAGE_COCL_IDASSIGNMENT       "IDAss" //IDAss(moduleAdress)(sessionID)
#define MESSAGE_CLCO_CONNECTIONCHANGED  "ConCh" //ConCh(updateCode)
#define MESSAGE_COCL_UPDATEREQUEST      "UpReq" //UpReq
#define MESSAGE_COCL_NEWEFFECT          "NewFX" //NewFX(effectCode)
#define MESSAGE_DUPL_SESSIONCHECK       "SeChk" //SeChk(sessionID)



struct BaseInfo
{
  uint8_t heartPiece;
  uint8_t northPipe;
  uint8_t eastPipe;
  uint8_t southPipe;
  uint8_t westPipe;
  uint8_t upPipe;
  uint8_t downPipe;
};
String baseInfoToString(BaseInfo baseInfo)
{
  return String(baseInfo.heartPiece) + String(baseInfo.northPipe) + String(baseInfo.eastPipe) + String(baseInfo.southPipe) + String(baseInfo.westPipe) + String(baseInfo.upPipe) + String(baseInfo.downPipe);
}

BaseInfo systemInfo = {BASE_HEARTPIECE, BASE_NORTHPIPE, BASE_EASTPIPE, BASE_SOUTHPIPE, BASE_WESTPIPE, BASE_UPPIPE, BASE_DOWNPIPE};


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

    if (message.startsWith(MESSAGE_COCL_IDASSIGNMENT))//Message contains 2 bytes: moduleAdress and sessionID
    {
      moduleAdress = (uint8_t)message.substring(strlen(MESSAGE_COCL_IDASSIGNMENT), strlen(MESSAGE_COCL_IDASSIGNMENT)+1).toInt();
      sessionID = (uint8_t)message.substring(strlen(MESSAGE_COCL_IDASSIGNMENT)+1).toInt();
      Serial.print("Module adress: ");
      Serial.println(moduleAdress);
      Serial.print("Session ID: ");
      Serial.println(sessionID);

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
  String identMessage = MESSAGE_CLCO_NEWCLIENT + WiFi.macAddress() + baseInfoToString(systemInfo);
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

void task_main( void *pvParameters )
{
  //Setup
  Serial.println("Task Main started");
  udp_connect();
  connectorManager = new ConnectorManager(&moduleAdress);
  Serial.println("Task Main setup complete");

  //Loop
  while (1)
  {
    //Serial.print("M");
    currentMillis = millis();
    /*
    if (currentMillis - lastMillis_SessionCheck > INTERVAL_SESSIONCHECK)
    {
      lastMillis_SessionCheck = currentMillis;
      udp_transmit(MESSAGE_DUPL_SESSIONCHECK+String(sessionID));
    }
    */

    String updateCode = connectorManager->getUpdateCode();
    if (updateCode != "") udp_transmit(MESSAGE_CLCO_CONNECTIONCHANGED+updateCode);

    connectorManager->tick();
    udp_tick();

    vTaskDelay(1);
  }
}
void task_leds( void *pvParameters )
{
  //Setup
   Serial.println("Task Leds started");
  trinity = new Trinity(60);
  trinity->addPanel(new Panel(0, 0, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 30));//Dead panel
  trinity->addPanel(new Panel(1, 0, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 30));//Horn A panel
  trinity->addPanel(new Panel(2, 0, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 30));//Pipe panel
  trinity->addPanel(new Panel(3, 0, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, 30));//Horn B panel
  trinity->begin();
  Serial.println("Task Leds setup complete");

  //Loop
  while (1)
  {
    //Serial.print("L");
    trinity->tick();
    vTaskDelay(1);
  }
}

void setup()
{
  Serial.begin(BAUDRATE_MONITOR);
  Serial.println(F("---===Setup started===---"));

  xTaskCreatePinnedToCore
  (
    task_main,       /* Task function. */
    "task_main",         /* name of task. */
    10000,           /* Stack size of task */
    NULL,            /* parameter of the task */
    1,               /* priority of the task */
    &Task1,          /* Task handle to keep track of created task */
    0                /* pin task to core 0 */
  );              

  xTaskCreatePinnedToCore
  (
    task_leds,       /* Task function. */
    "task_leds",         /* name of task. */
    10000,           /* Stack size of task */
    NULL,            /* parameter of the task */
    1,               /* priority of the task */
    &Task2,          /* Task handle to keep track of created task */
    1               /* pin task to core 1 */
  ); 


  Serial.println(F("---===Setup finished===---"));
}
void loop()
{
  
}
