#include "comms/Comms.h"

Comms::Comms()
{
    
}

void    Comms::reboot(String message)
{
  Serial.print(F("MANUAL REBOOT: "));
  Serial.println(message);
  ESP.restart();
}

void    Comms::transmit(String goalMac, String message)
{
  lastMessageID++;
  retransmit(Transmission{"MASTER", lastMessageID, messageType, message, DIRECTION_NONE});
}

void                    Comms::parseTransmission (String message)
{
    Serial.print("Parsing message: ");
    Serial.println(message);

    if (message.startsWith(MESSAGETYPE_COCL_REQUESTTEMPLATE))
    {
        lastMessageID++;
        retransmit(Transmission{"MASTER", lastMessageID, MESSAGETYPE_CLCO_NEWCLIENTTEMPLATE, macAdress + (char)SELECTEDPRESET, DIRECTION_NONE});
    }

    if (message.startsWith(MESSAGETYPE_COCL_UPDATEREQUEST))
    {
      Serial.println("Update requested, not implemented");
    }

    if (message.startsWith(MESSAGETYPE_COCL_NEWEFFECT))
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
}

void    Comms::receiveAndParse()
{
    if (message.startsWith(MESSAGE_CLCO_NEWCLIENTFULL))//Message contains 8 pieces of data: macAdress, heartPiece, n, e, s, w, u, d
    {
      Serial.println("New client full message received. Not implemented");
    }
    if (message.startsWith(MESSAGE_CLCO_NEWCLIENTTEMPLATE))//Message contains 8 pieces of data: macAdress, heartPiece, n, e, s, w, u, d
    {
      NewClientInfo newClientInfo;
      newClientInfo.macAdress = message.substring(5, 22);
      newClientInfo.ipAdress = udp.remoteIP().toString();
      newClientInfo.templateID = (uint8_t)message[22];

      Serial.print("New client: ");
      Serial.print(newClientInfo.macAdress);
      Serial.print(" with templateID: ");
      Serial.println(newClientInfo.templateID);
      

      newClientBuffer.push_back(newClientInfo);
    }

    if (message.startsWith(MESSAGE_CLCO_CONNECTIONCHANGED)) 
    {
      //Original code: String(char(moduleAdress)+ char(direction) + char(neighborAdress) + char(neighborDirection)

      ModuleChangeInfo moduleChangeInfo;
      moduleChangeInfo.moduleID =           (uint8_t)message[5];
      moduleChangeInfo.direction =          (uint8_t)message[6];
      moduleChangeInfo.neighborID =         (uint8_t)message[7];
      moduleChangeInfo.neighborDirection =  (uint8_t)message[8]; 

      moduleChangeBuffer.push_back(moduleChangeInfo);
    }

    if (message.startsWith(MESSAGE_HOCO_HORNTRIGGERED))
    {
      uint8_t moduleID = (uint8_t)message[5];

      Serial.print("Horn ");
      Serial.print(moduleID);
      Serial.println(" triggered");

      triggeredHornsIDs.push_back(moduleID);
    }

    if (message.startsWith(MESSAGE_DUPL_SESSIONCHECK)) //Session check, respond with current session
    {
      transmit(MESSAGE_DUPL_SESSIONCHECK + String(sessionID), udp.remoteIP().toString());
    }
    // Add else if conditions here for other codephrases
  }
}
void    Comms::connect()
{
  Serial.println("(Re)Connecting to WiFi");

  uint8_t attempts = 0;
  WiFi.setHostname("GLOWMASTER");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (attempts++ > WIFICONNECTATTEMPTS)
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
    Serial.print(WIFICONNECTATTEMPTS);
    Serial.println(")");
  }
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  // Start UDP
  udp.begin(SERVER_UDPPORT);
  Serial.print("Now listening at IP ");
  Serial.print(WiFi.localIP());
  Serial.print(", UDP port ");
  Serial.println(SERVER_UDPPORT);
}
void    Comms::tick()
{
  uint64_t currentMillis = millis();

  // Check if the WiFi is still connected, if not, reconnect
  if (WiFi.status() != WL_CONNECTED) connect();

  //Incoming messages & Parsing
  receiveAndParse();
}

std::vector<ModuleChangeInfo> Comms::getModuleChangeBuffer()
{
  std::vector<ModuleChangeInfo> temp = moduleChangeBuffer;
  moduleChangeBuffer.clear();
  return temp;
}
std::vector<NewClientInfo> Comms::getNewClientBuffer()
{
  std::vector<NewClientInfo> temp = newClientBuffer;
  newClientBuffer.clear();
  return temp;
}
std::vector<uint8_t> Comms::getTriggeredHornsIDs()
{
  std::vector<uint8_t> temp = triggeredHornsIDs;
  triggeredHornsIDs.clear();
  return temp;
}

void Comms::setSessionID
(uint8_t sessionID)
{
  this->sessionID = sessionID;
}