/*
void    Comms::transmit(String message)
{
  Serial.print(F("UDP TRANSMIT: "));
  Serial.println(message);
  udp.beginPacket(SERVER_IP, SERVER_UDPPORT);
  udp.write((uint8_t *)message.c_str(), message.length());
  udp.endPacket();
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

*/