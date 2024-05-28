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


#define RESERVATION_NONE 0
#define RESERVATION_MAIN 1
#define RESERVATION_LEDS 2
struct LedUpdate
{
  uint8_t inputPanel;
  uint8_t outputPanel;
  uint8_t colour;
  uint16_t offset;
};
volatile uint8_t memoryReservedBy;
std::vector<LedUpdate> ledUpdates;
std::vector<LedUpdate> ledUpdates_buffer;




WiFiUDP udp;
ConnectorManager *connectorManager;
Trinity *trinity;

uint8_t moduleAdress = 0;
uint8_t sessionID = 0;

uint64_t currentMillis = 0;
uint64_t lastMillis_SessionCheck = 0;

#define SERVER_UDPPORT 4210

#define PULSELENGTH_ID 5
#define INTERVAL_SESSIONCHECK 60000         // 1 minute

#define SYSTEMSTATE_CONNECTING_WIFI       0
#define SYSTEMSTATE_CONNECTING_CONTROLLER 1
#define SYSTEMSTATE_CONNECTED             2

#define TIMEOUTATTEMPTS 10

#define MESSAGE_CLCO_NEWCLIENTTEMPLATE  "NwClT" //NwClT(macAdress)(TemplateID)
#define MESSAGE_CLCO_NEWCLIENTFULL      "NwClF" //NwClF(macAdress)(heartPiece)(nl,nd)(el,ed)(sl,sd)(wl,wd)(ul,ud)(dl,dd)
#define MESSAGE_COCL_IDASSIGNMENT       "IDAss" //IDAss(moduleAdress)(sessionID)
#define MESSAGE_CLCO_CONNECTIONCHANGED  "ConCh" //ConCh(updateCode)
#define MESSAGE_COCL_UPDATEREQUEST      "UpReq" //UpReq
#define MESSAGE_COCL_NEWEFFECT          "NewFX" //NewFX(effectCode)
#define MESSAGE_DUPL_SESSIONCHECK       "SeChk" //SeChk(sessionID)

struct BaseInfo
{
  uint8_t id;
  uint8_t heartPiece;
  
  uint8_t northPipeLength;
  uint16_t northPipeDelay;
  
  uint8_t eastPipeLength;
  uint16_t eastPipeDelay;
  
  uint8_t southPipeLength;
  uint16_t southPipeDelay;
  
  uint8_t westPipeLength;
  uint16_t westPipeDelay;
  
  uint8_t upPipeLength;
  uint16_t upPipeDelay;
  
  uint8_t downPipeLength;
  uint16_t downPipeDelay;
};
BaseInfo getBaseInfo(uint8_t presetID)
{
  switch (presetID)
  {
    case PRESET_1_DEBUGCROSS:
      return BaseInfo{PRESET_1_DEBUGCROSS,BASE_HEART_X,         2,2*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    0,0,            0,0};
    break;
    case PRESET_2_AllCross1:
      return BaseInfo{PRESET_2_AllCross1,BASE_HEART_XUPDOWN,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR,    1,1*XFACTOR};
    break;
  }
  Serial.println("Preset not found");
  return BaseInfo{0,0,0,0,0,0,0,0,0,0,0,0,0};
}



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
      uint8_t inputDirection = (uint8_t)message[5];
      uint8_t outputDirection = (uint8_t)message[6];
      uint8_t color = (uint8_t)message[7];
      uint16_t delayOffset = ((uint8_t)message[8] << 8) | (uint8_t)message[9];

      LedUpdate newLedUpdate = LedUpdate{inputDirection, outputDirection, color, delayOffset};

      ledUpdates_buffer.push_back(newLedUpdate);

      Serial.print("New effect received. Input Direction: ");
      Serial.print(inputDirection);
      Serial.print(", Output Direction: ");
      Serial.print(outputDirection);
      Serial.print(", Color: ");
      Serial.print(color);
      Serial.print(", Delay Offset: ");
      Serial.println(delayOffset);

      
      
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
  String identMessage = MESSAGE_CLCO_NEWCLIENTTEMPLATE + WiFi.macAddress() + (char)SELECTEDPRESET;
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

    if (memoryReservedBy == RESERVATION_NONE)
    {
      memoryReservedBy = RESERVATION_MAIN;

      for (uint8_t i = 0; i < ledUpdates_buffer.size(); i++)
      {
        ledUpdates.push_back(ledUpdates_buffer[i]);
        ledUpdates_buffer.erase(ledUpdates_buffer.begin() + i);
      }

      memoryReservedBy = RESERVATION_NONE;
    }
    
    

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

  BaseInfo baseInfo = getBaseInfo(SELECTEDPRESET);

  trinity = new Trinity(60);
  if (baseInfo.northPipeLength  > 0) trinity->addPanel(new Panel(DIRECTION_NORTH,0, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, baseInfo.northPipeDelay));
  if (baseInfo.eastPipeLength   > 0) trinity->addPanel(new Panel(DIRECTION_EAST, 0, 0, CLOCK_CLOCKWISE, COMPASS_EAST,  baseInfo.eastPipeDelay));
  if (baseInfo.southPipeLength  > 0) trinity->addPanel(new Panel(DIRECTION_SOUTH,0, 0, CLOCK_CLOCKWISE, COMPASS_SOUTH, baseInfo.southPipeDelay));
  if (baseInfo.westPipeLength   > 0) trinity->addPanel(new Panel(DIRECTION_WEST, 0, 0, CLOCK_CLOCKWISE, COMPASS_WEST,  baseInfo.westPipeDelay));
  if (baseInfo.upPipeLength     > 0) trinity->addPanel(new Panel(DIRECTION_UP,   0, 0, CLOCK_CLOCKWISE, COMPASS_NORTH, baseInfo.upPipeDelay));
  if (baseInfo.downPipeLength   > 0) trinity->addPanel(new Panel(DIRECTION_DOWN, 0, 0, CLOCK_CLOCKWISE, COMPASS_SOUTH, baseInfo.downPipeDelay));
  trinity->begin();

  Serial.println("Task Leds setup complete");

  //Loop
  while (1)
  {
    //Serial.print("L");
    trinity->tick();

    if(memoryReservedBy == RESERVATION_NONE)
    {
      memoryReservedBy = RESERVATION_LEDS;

      for (uint8_t i = 0; i < ledUpdates.size(); i++)
      {
        uint8_t inputDirection = ledUpdates[i].inputPanel;
        uint8_t outputDirection = ledUpdates[i].outputPanel;
        uint8_t color = ledUpdates[i].colour;
        uint16_t delayOffset = ledUpdates[i].offset;
        ledUpdates.erase(ledUpdates.begin() + i);

        uint16_t inputDelay = delayOffset + trinity->getPanelDiodeAmount(inputDirection);
        for (uint16_t i = 0; i < trinity->getPanelDiodeAmount(inputDirection); i++)
        {
          trinity->setPanelDiodeVfx(inputDirection, i, VFXData{EFFECT_STOCK_FLASH, color, inputDelay, 1, false});
          inputDelay--;
        }

        uint16_t outputDelay = delayOffset + trinity->getPanelDiodeAmount(inputDirection);
        for (uint16_t i = 0; i < trinity->getPanelDiodeAmount(outputDirection); i++)
        {
          trinity->setPanelDiodeVfx(outputDirection, i, VFXData{EFFECT_STOCK_FLASH, color, outputDelay, 1, false});
          outputDelay++;
        }
      }

      memoryReservedBy = RESERVATION_NONE;
    }

    


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
  