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

void    Comms::transmit(String message, String clientIP)
{
  if (clientIP == "192.168.137.105") return; //Ignore myself
  Serial.print(F("UDP TRANSMIT: "));
  Serial.print(clientIP);
  Serial.print(F(" - "));
  Serial.println(message);
  udp.beginPacket(clientIP.c_str(), SERVER_UDPPORT);
  udp.write((uint8_t *)message.c_str(), message.length());
  udp.endPacket();
} 
void    Comms::receiveAndParse()
{
  //Message Receiving
  if (udp.parsePacket())
  { 
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = 0;
    

    //Message handling
    String message = String(incomingPacket);
    message.trim();
    
    Serial.print(F("UDP RECEIVED: "));
    Serial.println(message);
    

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