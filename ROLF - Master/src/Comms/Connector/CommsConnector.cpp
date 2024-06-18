#include "CommsConnector.h"

//Private
bool            CommsConnector::waitAndRead           (uint8_t *data)
{
    *data = 0; //Set data to 0

    if (lineMode != LINEMODE_SERIAL_READ)
    {
        Serial.println(F("Error: CommsConnector::waitndRead() called while not in read mode"));
        return false;
    }
    
    uint64_t lastMillis_ReadStart = millis();

    while (!softwareSerial->available())
    {
        if (millis() - lastMillis_ReadStart > SOFTSERIALTIMOUTTIME)
        {
            Serial.println(F("Timeout"));
            return false;
        }
    }

    *data = softwareSerial->read();
    return true;
}
void            CommsConnector::setLineMode           (uint8_t newLineMode)
{
    if (lineMode == newLineMode) return;

    if (lineMode == LINEMODE_BUSY && newLineMode != LINEMODE_IDLE)
    {
        Serial.print(F("Error: CommsConnector::setLineMode() - Trying to change from busy to something other than idle: "));
        Serial.println(newLineMode);
        return;
    }

    switch (newLineMode)
    {
        case LINEMODE_IDLE:
        {
            if (lineMode == LINEMODE_SERIAL_READ || lineMode == LINEMODE_SERIAL_WRITE)
            {
                softwareSerial->end();
                delete softwareSerial;
            }
            pinMode(pin_comms, INPUT_PULLUP);
        }
        break;

        case LINEMODE_PULLCLAIMED:
        {
            if (lineMode == LINEMODE_SERIAL_READ || lineMode == LINEMODE_SERIAL_WRITE)
            {
                softwareSerial->end();
                delete softwareSerial;
            }
            pinMode(pin_comms, OUTPUT);
            digitalWrite(pin_comms, LOW);
        }
        break;

        case LINEMODE_BUSY:
        {
            if (lineMode == LINEMODE_SERIAL_READ || lineMode == LINEMODE_SERIAL_WRITE)
            {
                softwareSerial->end();
                delete softwareSerial;
            }
            pinMode(pin_comms, OUTPUT);
            digitalWrite(pin_comms, HIGH);
        }
        break;

        case LINEMODE_SERIAL_READ:
        {
            if (lineMode == LINEMODE_SERIAL_WRITE)
            {
                softwareSerial->end();
                delete softwareSerial;
            }
            softwareSerial = new EspSoftwareSerial::UART();
            softwareSerial->begin(BAUDRATE_SYSTEM, EspSoftwareSerial::SWSERIAL_8N2, pin_comms, PIN_DEAD, false, 256);
        }
        break;

        case LINEMODE_SERIAL_WRITE:
        {
            if (lineMode == LINEMODE_SERIAL_READ) 
            {
                softwareSerial->end();
                delete softwareSerial;
            }
            pinMode(pin_comms, OUTPUT);     //Dunno if this is a good idea
            digitalWrite(pin_comms, LOW);   //Dunno if this is a good idea
            softwareSerial = new EspSoftwareSerial::UART();
            softwareSerial->begin(BAUDRATE_SYSTEM, EspSoftwareSerial::SWSERIAL_8N2, PIN_DEAD, pin_comms, false, 256);
        }
        break;
    }
    lineMode = newLineMode;
}
void            CommsConnector::saveNeighborData      (String newNeighborMacAdress, uint8_t newNeighborDirection)
{
    Serial.println(F("Saving neighbor data: "));


    if (newNeighborMacAdress != neighborMacAdress || newNeighborDirection != neighborDirection)
    {
        neighborMacAdress = newNeighborMacAdress;
        neighborDirection = newNeighborDirection;
        String updateCode = "";
        updateCode += macAdress;
        updateCode += char(direction);
        updateCode += neighborMacAdress;
        updateCode += char(neighborDirection);
        updateCodes.push_back(updateCode);
        
        Serial.println();
        Serial.print(F("Mac: "));
        Serial.print(sixCharMacToColonMac(macAdress));
        Serial.print(F(" - NeighborMac: "));
        Serial.print(sixCharMacToColonMac(neighborMacAdress));
        Serial.print(F(" connected on side "));
        Serial.print(directionToString(direction));
        Serial.print(F(" - Neigbor's side: "));
        Serial.println(directionToString(neighborDirection));
    }
    else
    {
        Serial.print(F("Neighbor already known: "));
        Serial.print(F("Mac: "));
        Serial.print(sixCharMacToColonMac(macAdress));
        Serial.print(F(" - NeighborMac: "));
        Serial.print(sixCharMacToColonMac(neighborMacAdress));
        Serial.print(F(" connected on side "));
        Serial.print(directionToString(direction));
        Serial.print(F(" - Neigbor's side: "));
        Serial.println(directionToString(neighborDirection));
    }
}
void            CommsConnector::pinTransmitV2         (String data)// Replaces all the other outbound transissions
{
    setLineMode(LINEMODE_PULLCLAIMED);
    delay(5); //REMOVE THIS DELAY
    setLineMode(LINEMODE_SERIAL_WRITE);
    for (int i = 0; i < data.length(); i++)
    {
        softwareSerial->write(data[i]);
        delay(5); //Extra delay for stability during debugging. (Remove this delay)
    }
    setLineMode(LINEMODE_IDLE);
}
//Constructor
CommsConnector::CommsConnector                      (uint8_t pin_comms, uint8_t pin_ledSync, uint8_t direction, String macAdress)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating CommsConnector at adress "));
        Serial.println((int)this, DEC);
    }

    this->pin_comms = pin_comms;
    this->pin_ledSync = pin_ledSync;
    this->direction = direction;
    this->macAdress = macAdress;

    pinMode(pin_ledSync, INPUT_PULLUP);
    setLineMode(LINEMODE_IDLE);
}
//Public
void            CommsConnector::tick                  ()
{
    //Check if the connection is still alive. (AKA: Did we receive a pulse in the last X seconds? If not, say that we are disconnected.)
    if (lastMillis_PinTest + MAXINTERVALMILLIS_PINTEST < millis() && connectionState != NEIGH_CONNECTSTATE_DISCONNECTED)
    {
        forgetNeighbor();
    }
}
uint8_t         CommsConnector::checkLineClaimed      ()
{
    if (connectionState == NEIGH_CONNECTSTATE_BLOCKED) return false; //Early return if module is chosen to be unreliable

    if (lineMode == LINEMODE_BUSY) 
    {
        Serial.println(F("Error: CommsConnector::checkLineClaimed() - Trying to check line while in busy mode"));
        return false; //Early return if the module is  busy. Module should never be running code while busy.
    }

    bool lineClaimed = !digitalRead(pin_comms); //Check if the line is claimed by the neighbor
    if (neighborBusy) //If it's already known the neighbor is busy.
    {
        if (lineClaimed) lastMillis_CharReceived = millis(); //If the line is already known to be busy. Don't inform.  (Now just wait untill the line is free again. Then we're allowed to inform again)
        else if (millis() - lastMillis_CharReceived > CHARFREETIME) neighborBusy = false; //If line is released/ Check if enough time has passed since last character. More transmissions may be coming. (If not, the line is free and we can listen for identifiers again.)
        return false; //No new transmission is to be expected (because we secretly already know the line has already been claimed because neighbor is busy.)
    }
    
    if (!lineClaimed) return false; //If line is not claimed. Return false. No new transmissions are to be expected.

    //If this code is reached the line has been newly claimed. Figure out what kind of transmission is to be expected.
    setLineMode(LINEMODE_SERIAL_READ);
    
    uint8_t identifier = 0;
    if (!waitAndRead(&identifier)) //When a modules claims a line it will send out an identifier to know what kind of transmisison is to be expected.
    {
        neighborBusy = true;
        return TRANSMISSIONTYPE_BUSY; //If the identifier is not received in time it means the neighbor sent out a busy signal. (busy signal keeps the line low)
    }

    setLineMode(LINEMODE_IDLE);
    return identifier; //After the identier is read, the message will immediately follow (Blocking code until message is received.). This message will have to be read elsewhere.
}
void            CommsConnector::handlePinTest         ()
{
    Serial.println(F("Pt"));
    lastMillis_PinTest = millis(); //Obsolete?

    setLineMode(LINEMODE_SERIAL_READ);
    String incomingPinTestData = "";
    while (true)
    {
        uint8_t incomingChar = 0;
        if(!waitAndRead(&incomingChar)) break;
        incomingPinTestData += char(incomingChar);
    }

    if (incomingPinTestData.length() != MESSAGELENGTH_PINTEST) //If the incoming data is too short, return false. Data went missing during transmit
    {
        Serial.print(F("PinTest data too short. Length was: "));
        Serial.print(incomingPinTestData.length());
        Serial.print(F(" - Data: "));
        printStringAsHex(incomingPinTestData);
        connectionState = NEIGH_CONNECTSTATE_UNKNOWN;
        saveNeighborData(ADRESS_UNKNOWN, DIRECTION_NONE);
        return;
    }
    connectionState = NEIGH_CONNECTSTATE_CONNECTED;
    saveNeighborData(incomingPinTestData.substring(0, 6), (uint8_t)incomingPinTestData[6]);
}
Transmission    CommsConnector::handleMessageRead     ()
{
    //Read transmission
    setLineMode(LINEMODE_SERIAL_READ);
    String transmissionString = "";
    while (true)
    {
        uint8_t incomingChar;
        if(!waitAndRead(&incomingChar)) break;
        transmissionString += char(incomingChar);
    }

    //Check if message is long enough to be a transmission
    if (transmissionString.length() < 12)
    {
        Serial.print(F("Transmission too short. Length was: "));
        Serial.print(transmissionString.length());
        Serial.print(F(" - Message: "));
        Serial.println(transmissionString);
        return Transmission{"", 0, "", "", 0};
    }

    //Parse transmission
    Transmission incomingTransmission;
    incomingTransmission.connectorID    = direction;
    incomingTransmission.goalMac        = transmissionString.substring(0, 6);   //6 byte
    incomingTransmission.messageID      = (char) transmissionString[6];         //1 byte
    incomingTransmission.messageType    = transmissionString.substring(7, 12);  //5 byte
    incomingTransmission.message        = transmissionString.substring(12);     //Rest of the message

    return incomingTransmission;
}
void            CommsConnector::transmit_pinTest      ()
{
    String data = "";
    data += char(TRANSMISSIONTYPE_PINTEST);
    data += macAdress;
    data += char(direction);
    pinTransmitV2(data);
}
void            CommsConnector::setBusy               (bool busy)
{
    if (busy)
    {
        if (lineMode == LINEMODE_BUSY)
        {
            Serial.println(F("Error: CommsConnector::setBusy() - Trying to set busy while it already was busy. This is not allowed"));
        }
        else
        {
            setLineMode(LINEMODE_BUSY);
        }
    }
    else
    {
        if (lineMode != LINEMODE_BUSY)
        {
            Serial.println(F("Error: CommsConnector::setBusy() - Trying to set not busy while it was not busy. This is not allowed"));
        }
        else
        {
            setLineMode(LINEMODE_IDLE);
        }
    }
}
void            CommsConnector::forgetNeighbor        ()
{
    //DOnt do anything if neighbor isnt there
    if (connectionState == NEIGH_CONNECTSTATE_DISCONNECTED || connectionState == NEIGH_CONNECTSTATE_UNKNOWN) return;


    Serial.print(F("Forced to forget neighbor "));
    Serial.print(neighborMacAdress);
    Serial.print(F(" on side "));
    Serial.print(directionToString(direction));
    Serial.print(F(" - Neigbor's side: "));
    Serial.println(directionToString(neighborDirection));
    
    saveNeighborData(ADRESS_UNKNOWN, DIRECTION_NONE);
    connectionState = NEIGH_CONNECTSTATE_DISCONNECTED;
}
uint64_t        CommsConnector::getLastMillis_PinTest ()
{
    return lastMillis_PinTest;
}
String          CommsConnector::getConnectionUpdate   ()
{
    if (updateCodes.size() == 0) return "";
    
    String updateCodeToSend = updateCodes[0];
    updateCodes.erase(updateCodes.begin());
    return updateCodeToSend;
}
String          CommsConnector::getNeighborMacAdress  ()
{
    return neighborMacAdress;
}
uint8_t         CommsConnector::getDirection          ()
{
    return direction;
}
void            CommsConnector::printConnector        ()
{
    Serial.print(F("Connector: "));
    Serial.print(directionToString(direction));

    if(connectionState == NEIGH_CONNECTSTATE_CONNECTED)
    {
        Serial.print(F(" - Connected to "));
        Serial.print(sixCharMacToColonMac(neighborMacAdress));
        Serial.print(F(" on side "));
        Serial.println(directionToString(neighborDirection));
    }
    else if(connectionState == NEIGH_CONNECTSTATE_DISCONNECTED) Serial.println(F(" - Disconnected"));
    else if(connectionState == NEIGH_CONNECTSTATE_BLOCKED) Serial.println(F(" - Blocked"));
    else
    {
        Serial.print(F(" - Unknown state: "));
        Serial.println(connectionState);
    }

}
