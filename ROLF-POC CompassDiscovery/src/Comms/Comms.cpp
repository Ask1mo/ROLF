#include "comms/Comms.h"

Comms::Comms()
{
    //Have comms.connect() here?
}

void    Comms::reboot(String message)
{
  Serial.print(F("MANUAL REBOOT: "));
  Serial.println(message);
  ESP.restart();
}

void    Comms::transmit(String message)
{
  Serial.print(F("UDP TRANSMIT: "));
  Serial.println(message);
  udp.beginPacket(SERVER_IP, SERVER_UDPPORT);
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
    //Serial.printf("UDP RECEIVED:  %s\n", incomingPacket);


    //Message handling
    String message = String(incomingPacket);
    message.trim();

    if (message.startsWith(MESSAGE_COCL_IDASSIGNMENT))//Message contains 2 bytes: moduleAdress and sessionID
    {
      moduleID = (uint8_t)message.substring(strlen(MESSAGE_COCL_IDASSIGNMENT), strlen(MESSAGE_COCL_IDASSIGNMENT)+1).toInt();
      sessionID = (uint8_t)message.substring(strlen(MESSAGE_COCL_IDASSIGNMENT)+1).toInt();
      Serial.print("ModuleID: ");
      Serial.println(moduleID);
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
void    Comms::connect()
{
  Serial.println("(Re)Connecting to WiFi");

  uint8_t attempts = 0;
  WiFi.begin(WIFI_SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (attempts++ > TIMEOUTATTEMPTS)
    {
      Serial.println("WIFI connection failed. Restarting...");
      ESP.restart();
    }
    delay(1000);
    Serial.print("Attempting connection to ");
    Serial.print(WIFI_SSID);
    Serial.print(" (");
    Serial.print(attempts);
    Serial.print("/");
    Serial.print(TIMEOUTATTEMPTS);
    Serial.println(")");
  }
  Serial.println("WiFi Connected");


  udp.begin(SERVER_UDPPORT);
  String identMessage = MESSAGE_CLCO_NEWCLIENTTEMPLATE + WiFi.macAddress() + (char)SELECTEDPRESET;
  transmit(identMessage);

  uint16_t timeout = 10000;
  while (moduleID == 0 || sessionID == 0) 
  {
    if (timeout-- == 0) reboot("Server connection failed. Restarting...");
    delay(1);
    receiveAndParse();
  }
  Serial.println("Server Connected");
}
void    Comms::tick()
{
  uint64_t currentMillis = millis();

  // Check if the WiFi is still connected, if not, reconnect
  if (WiFi.status() != WL_CONNECTED) connect();

  //Incoming messages & Parsing
  receiveAndParse();

  //Session check
  if (currentMillis - lastMillis_SessionCheck > INTERVAL_SESSIONCHECK)
  {
    lastMillis_SessionCheck = currentMillis;
    transmit(MESSAGE_DUPL_SESSIONCHECK+String(sessionID));
  }
    
  //Putting received messages in memory accessable for main_leds
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

}
uint8_t *Comms::getModuleID()
{
    return &moduleID;
}
std::vector<LedUpdate> Comms::getLedUpdates()
{
  std::vector<LedUpdate> ledUpdatesToReturn;

  if (memoryReservedBy == RESERVATION_NONE)
  {
    memoryReservedBy = RESERVATION_LEDS;
    ledUpdatesToReturn = ledUpdates;
    ledUpdates.clear();
    memoryReservedBy = RESERVATION_NONE;
    return ledUpdatesToReturn;
  }
  return ledUpdatesToReturn;
}