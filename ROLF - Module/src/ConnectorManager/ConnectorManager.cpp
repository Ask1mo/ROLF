#include "ConnectorManager.h"
//private
void                    ConnectorManager::parseTransmission (String message)
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
void                    ConnectorManager::retransmit        (Transmission transmission) //Retransmit is a bad name for this func.
{
    for (uint8_t i = 0; i < DIRECTIONS; i++)
    {
        if (transmission.connectorID == compassConnectors[i]->getDirection()) continue; //You don't want to retransmit the message to the connector from which you received it
        //compassConnectors[i]->queueTransmission(transmission);
    }
}
void                    ConnectorManager::setSystemBusy     (uint8_t directionThatsActuallyDoingSomething, bool busy)
{
    //Transmit busy on all other pins so no transmissions are missed since we're busy with the pin on directionThatsActuallyDoingSomething
    for (int i = 0; i < DIRECTIONS; i++)
    {
        if (compassConnectors[i]->getDirection() != directionThatsActuallyDoingSomething) compassConnectors[i]->setBusy(busy);
    }
}
void                    ConnectorManager::printConnectors   ()
{
    Serial.println(F("Printing connectors:"));
    for (int i = 0; i < DIRECTIONS; i++)
    {
        compassConnectors[i]->printConnector();
    }
    Serial.println();
}
//Constructor
ConnectorManager::ConnectorManager                          (String macAdress)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating ConnectorManager at adress "));
        Serial.println((int)this, DEC);
    }

    if (macAdress.length() != 6)
    {
        Serial.print(F("Error:ConnectorManager - Mac adress is not 6 characters long: "));
        Serial.println(macAdress);
        return;
    }

    this->compassConnectors = new CompassConnector*[DIRECTIONS];
    this->compassConnectors[0] = new CompassConnector(PIN_IDENT_NORTH,  PIN_SYNC_NORTH,   DIRECTION_NORTH,    macAdress);
    this->compassConnectors[1] = new CompassConnector(PIN_IDENT_EAST,   PIN_SYNC_EAST,    DIRECTION_EAST,     macAdress);
    this->compassConnectors[2] = new CompassConnector(PIN_IDENT_SOUTH,  PIN_SYNC_SOUTH,   DIRECTION_SOUTH,    macAdress);
    this->compassConnectors[3] = new CompassConnector(PIN_IDENT_WEST,   PIN_SYNC_WEST,    DIRECTION_WEST,     macAdress);
    this->compassConnectors[4] = new CompassConnector(PIN_IDENT_UP,     PIN_SYNC_UP,      DIRECTION_UP,       macAdress);
    this->compassConnectors[5] = new CompassConnector(PIN_IDENT_DOWN,   PIN_SYNC_DOWN,    DIRECTION_DOWN,     macAdress);
    currentPinTestDirection = 0;
    this->macAdress = macAdress;

    String identMessage = macAdress + (char)SELECTEDPRESET;
    lastMessageID++;
    retransmit(Transmission{"MASTER", lastMessageID, MESSAGETYPE_CLCO_NEWCLIENTTEMPLATE, identMessage, DIRECTION_NONE});
}
//Public
void                    ConnectorManager::tick              ()
{
    uint64_t currentMillis = millis();

    if (currentMillis - lastMillis_PinTest > INTERVAL_PINTEST)
    {   
        lastMillis_PinTest = currentMillis;
        compassConnectors[currentPinTestDirection]->transmit_pinTest();

        currentPinTestDirection++;
        if (currentPinTestDirection >= DIRECTIONS)currentPinTestDirection = 0;
    }


    for (int i = 0; i < DIRECTIONS;  i++)
    {
        compassConnectors[i]->tick();

        char newTransmissionType = (char)compassConnectors[i]->checkLineClaimed();
        if (newTransmissionType != 0)
        {

            switch (newTransmissionType)
            {
                case TRANSMISSIONTYPE_BUSY:
                {
                    Serial.print(F("Connector on pin "));
                    Serial.print(directionToString(compassConnectors[i]->getDirection()));
                    Serial.println(F(" is busy"));
                    //The connector class already notes down itself as busy, so no need to do anything here
                }
                break;

                case TRANSMISSIONTYPE_PINTEST:
                {
                    setSystemBusy(compassConnectors[i]->getDirection(), true);
                    compassConnectors[i]->handlePinTest();
                    setSystemBusy(compassConnectors[i]->getDirection(), false);

                    Serial.print(F("Pin test on pin "));
                    Serial.print(directionToString(compassConnectors[i]->getDirection()));
                    Serial.println(F(" completed"));
                }   
                break;

                case TRANSMISSIONTYPE_MESSAGE:
                {
                    setSystemBusy(compassConnectors[i]->getDirection(), true);
                    Transmission incomingTransmission = compassConnectors[i]->handleMessageRead();
                    if (incomingTransmission.goalMac == macAdress) parseTransmission(incomingTransmission.message);
                    else retransmit(incomingTransmission); //There should be double retransmit prevention in this function. This function only queues transmissions.
                    setSystemBusy(compassConnectors[i]->getDirection(), false);

                    Serial.print(F("Message on pin "));
                    Serial.print(directionToString(compassConnectors[i]->getDirection()));
                    Serial.println(F(" completed"));
                }
                break;

                default:
                {
                    Serial.print(F("Unknown transmission type detected on pin: "));
                    Serial.print(directionToString(compassConnectors[i]->getDirection()));
                    Serial.print(F(" Type: "));
                    Serial.println(newTransmissionType);
                }
                break;
            }
        }

        //Make sure no duplicates connections have been made
        for (int j = 0; j < DIRECTIONS; j++)
        {
            //If two connectors have the same neighbor, delete the oldest one. If any of the connectors have an unknown neighbor, ignore them
            if(compassConnectors[i]->getNeighborMacAdress() == compassConnectors[j]->getNeighborMacAdress() && i != j && compassConnectors[i]->getNeighborMacAdress() != ADRESS_UNKNOWN && compassConnectors[j]->getNeighborMacAdress() != ADRESS_UNKNOWN)
            {
                //If i is the newest, delete j. Else delete i
                if(compassConnectors[i]->getLastMillis_PinTest() > compassConnectors[j]->getLastMillis_PinTest()) compassConnectors[j]->forgetNeighbor();
                else compassConnectors[i]->forgetNeighbor();
                
            }
        }

        //Detecing pin changes and sending them to the master
        String updateCode = "";
        for (int i = 0; i < DIRECTIONS; i++)
        {
            updateCode = compassConnectors[i]->getConnectionUpdate();
            if (updateCode != "") transmit(MESSAGETYPE_CLCO_CONNECTIONCHANGED, updateCode);
        }
    }
}
void                    ConnectorManager::transmit          (String messageType, String message)
{
    lastMessageID++;
    retransmit(Transmission{"MASTER", lastMessageID, messageType, message, DIRECTION_NONE});
}
std::vector<LedUpdate>  ConnectorManager::getLedUpdates     ()
{
  std::vector<LedUpdate> ledUpdatesToReturn = ledUpdates_buffer;
  ledUpdates_buffer.clear();
  return ledUpdatesToReturn;
}