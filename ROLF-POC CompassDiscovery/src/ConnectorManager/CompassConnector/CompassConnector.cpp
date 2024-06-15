#include "CompassConnector.h"


//Constructor
CompassConnector::CompassConnector(uint8_t pin_ident, uint8_t pin_sync, uint8_t direction, uint8_t *moduleAdress)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating CompassConnector at adress "));
        Serial.println((int)this, DEC);
    }

    this->pin_ident = pin_ident;
    this->pin_sync = pin_sync;
    this->direction = direction;
    this->moduleAdress = moduleAdress;
    connectionState = NEIGH_CONNECTSTATE_UNKNOWN;
    neighborAdress = ADRESS_UNKNOWN;
    neighborDirection = DIRECTION_NONE;
    lastMillis_SyncPulse = 0;
    serialMode = SERIALMODE_DISABLED;

    this->lockSystemOccupied = lockSystemOccupied;
    this->releaseSystemOccupied = releaseSystemOccupied;

    claimLine(false);

    pinMode(pin_sync, INPUT_PULLUP);
}

//Private
void CompassConnector::claimLine(bool enabled)
{
    if (serialMode)
    {
        softwareSerial->end();
        delete softwareSerial;
        serialMode = SERIALMODE_DISABLED;
    }

    if (enabled)// Claim line by pulling low
    {   
        pinMode(pin_ident, OUTPUT);
        digitalWrite(pin_ident, LOW);
    }
    else //Release line back to high
    {
        pinMode(pin_ident, INPUT_PULLUP);
    }
}
uint8_t CompassConnector::checkLineClaimed()
{
    if (connectionState == NEIGH_CONNECTSTATE_BLOCKED) return false; //Early return if module is chosen to be unreliable

    bool lineClaimed = !digitalRead(pin_ident);
    if (neighborIsBusy) //If the neighbor is already known to be busy:
    {
        if (lineClaimed) return TRANSMISSIONTYPE_BUSY; //If the line is claimed, return 'B' to indicate that the neighbor is still busy.
        else  //If line voltage is HIGH, return false. Line is not claimed. Neigbor is available again
        {
            neighborIsBusy = false;
            return false; //Line is no longer claimed
        }
    }
    else if (!lineClaimed) return false; //If the connector is known to be not busy. And if line is not claimed. Return false. Line is not claimed.

    //If this code is reached the line has been newly claimed. Figure out what kind of transmission is to be expected.
    bool timedOut = false; //This var is not really doing anything. If waitAndRead() times out it will return 0 which is acceptable. (The timeout Bool can be ignored in this case )
    uint8_t identifier = waitAndRead(&timedOut, 25);  //When a modules claims a line it will send out an identifier to know what kind of transmisison is to be expected.
    if (identifier == TRANSMISSIONTYPE_BUSY) neighborIsBusy = true; //If the identifier is 'B' the neighbor is busy and we should not send any messages to it.
    return identifier; //After the identier is read, the message will immediately follow (Blocking code until message is received.). This message will have to be read elsewhere.
}

void CompassConnector::prepareSerial_Read()
{
    if (serialMode != SERIALMODE_DISABLED)
    {
        softwareSerial->end();
        delete softwareSerial;
    }
    serialMode = SERIALMODE_READ;
    softwareSerial = new EspSoftwareSerial::UART();
    softwareSerial->begin(BAUDRATE_SYSTEM, EspSoftwareSerial::SWSERIAL_8N1, pin_ident, PIN_DEAD, false, 256);
}
void CompassConnector::prepareSerial_Write()
{
    if (serialMode != SERIALMODE_DISABLED)
    {
        softwareSerial->end();
        delete softwareSerial;
    }
    serialMode = SERIALMODE_WRITE;
    softwareSerial = new EspSoftwareSerial::UART();
    softwareSerial->begin(BAUDRATE_SYSTEM, EspSoftwareSerial::SWSERIAL_8N1, PIN_DEAD, pin_ident, false, 256);
}
void CompassConnector::transmit()
{
    prepareSerial_Write();
    softwareSerial->write(*moduleAdress);
    delay(PULSELENGTH_SYNC);
    softwareSerial->write(direction);
    claimLine(false); //To make sure the data line returns to HIGH, release it.
}
void CompassConnector::transmitAck()
{
    prepareSerial_Write();
    softwareSerial->write('Ack');
    claimLine(false);
}
bool CompassConnector::readData(uint8_t *newNeighborAdress, uint8_t *newNeighborDirection)
{
    bool timedOut = false; //Todo: Implement this timeout variable instead of the readResult == false code.
    

    uint8_t readResult = waitAndRead(&timedOut, 100);
    if(readResult == false)
    {
        connectionState = NEIGH_CONNECTSTATE_UNKNOWN;
        *newNeighborAdress = ADRESS_UNKNOWN;
        *newNeighborDirection = DIRECTION_NONE;
        return false;
    }
    else *newNeighborAdress = readResult;

    readResult = waitAndRead(&timedOut, 100);
    if(readResult == false)
    {
        connectionState = NEIGH_CONNECTSTATE_UNKNOWN;
        *newNeighborAdress = ADRESS_UNKNOWN;
        *newNeighborDirection = DIRECTION_NONE;
        return false;
    }
    else *newNeighborDirection = readResult;

    connectionState = NEIGH_CONNECTSTATE_CONNECTED;
    return true;
}
uint8_t CompassConnector::waitAndRead(bool *timedOut, uint16_t timeoutMillis)
{
    //TimedOut will start as false, but if the function times out it will be set to true.
    if (*timedOut) 
    {
        Serial.println(F("Timed out, early return in CompassConnector::waitAndRead();"));
        return false; //If the function has already timed out, return false. It means it timed out in an earlier read.
    }
    prepareSerial_Read();

    uint64_t lastMillis_ReadStart = millis();

    while (!softwareSerial->available())
    {
        if ( millis() - lastMillis_ReadStart > timeoutMillis)
        {
            Serial.println(F("Timeout"));
            claimLine(false);
            *timedOut = true;
            return false;
        }
    }
    uint8_t readResult = softwareSerial->read();
    claimLine(false);
    return readResult;
}
String directionToString(uint8_t compassDirection)
{
    switch (compassDirection)
    {
        case DIRECTION_NONE:
            return "DIRECTION_NONE";
        break;

        case DIRECTION_NORTH:
            return "North";
        break;

        case DIRECTION_EAST:
            return "East ";
        break;

        case DIRECTION_SOUTH:
            return "South";
        break;

        case DIRECTION_WEST:
            return "West ";
        break;

        case DIRECTION_UP:
            return "Up   ";
        break;

        case DIRECTION_DOWN:
            return "Down ";
        break;
    }
    return "ERROR_DIRECTIONTOSTRING";
}
void CompassConnector::saveNeighborData(uint8_t newNeighborAdress, uint8_t newNeighborDirection)
{
    if (newNeighborAdress != neighborAdress || newNeighborDirection != neighborDirection)
    {
        neighborAdress = newNeighborAdress;
        neighborDirection = newNeighborDirection;
        String updateCode = "";
        updateCode += char(*moduleAdress);
        updateCode += char(direction);
        updateCode += char(neighborAdress);
        updateCode += char(neighborDirection);
        updateCodes.push_back(updateCode);
        //updateCodes.push_back(String(char(*moduleAdress)+ char(direction) + char(neighborAdress) + char(neighborDirection)));

        
        Serial.println();
        Serial.print(F("ModuleID "));
        Serial.print(*moduleAdress);
        Serial.print(F(" - Neighbor "));
        Serial.print(neighborAdress);
        Serial.print(F(" connected on side "));
        Serial.print(directionToString(direction));
        Serial.print(F(" - Neigbor's side: "));
        Serial.println(directionToString(neighborDirection));

        Serial.print("ReconvertTest: ");
        Serial.print((uint8_t)updateCode[0]);
        Serial.print(" ");
        Serial.print((uint8_t)updateCode[1]);
        Serial.print(" ");
        Serial.print((uint8_t)updateCode[2]);
        Serial.print(" ");
        Serial.print((uint8_t)updateCode[3]);
        Serial.println();
            
    }
}

//Public
void CompassConnector::tick()
{
    //Check if the connection is still alive. (AKA: Did we receive a pulse in the last X seconds? If not, say that we are disconnected.)
    if (lastMillis_SyncPulse + INTERVAL_SYNCPULSE_MAXABSENCE < millis() && connectionState != NEIGH_CONNECTSTATE_DISCONNECTED)
    {
        saveNeighborData(ADRESS_UNKNOWN, DIRECTION_NONE);
        connectionState = NEIGH_CONNECTSTATE_DISCONNECTED;
    }
}

void CompassConnector::handlePinTest()
{
    lastMillis_SyncPulse = millis();
    uint8_t newNeighborAdress;
    uint8_t newNeighborDirection;
    readData(&newNeighborAdress, &newNeighborDirection);
    saveNeighborData(newNeighborAdress, newNeighborDirection);
}

Transmission CompassConnector::handleMessageRead()
{
    bool timedOut = false; 
    Transmission incomingTransmission = {false, 0, 0, direction, 0, ""};

    incomingTransmission.isIdentMessage = waitAndRead(&timedOut, 10);
    incomingTransmission.goalID         = waitAndRead(&timedOut, 10);
    incomingTransmission.senderID       = waitAndRead(&timedOut, 10);
    incomingTransmission.connectorID    = direction;
    incomingTransmission.messageID      = waitAndRead(&timedOut, 10);

    

    bool fistStringRead = true;
    while (!timedOut)
    {
        incomingTransmission.message += char(waitAndRead(&timedOut, 10));

        if (fistStringRead)
        {
            fistStringRead = false;
            if (timedOut) return Transmission{false, 0, 0, 0, 0, ""}; //If the transmisison timed out before any characters could be received, the message is empty, in which case transmission has failed.
        }
    }
    transmitAck();
}



void CompassConnector::sendPulse_Ident()
{
    claimLine(true);
    delay(PULSELENGTH_SYNC);
    transmit();
}
void CompassConnector::sendPulse_Sync()
{
    pinMode(pin_sync, OUTPUT);
    digitalWrite(pin_sync, LOW);
    delay(PULSELENGTH_SYNC);
    pinMode(pin_sync, INPUT_PULLUP);
}

String CompassConnector::getUpdateCode()
{
    if (updateCodes.size() == 0) return "";
    
    String updateCodeToSend = updateCodes[0];
    updateCodes.erase(updateCodes.begin());
    return updateCodeToSend;
}
void CompassConnector::printConnector()

{
    Serial.print(F("Connector: "));
    Serial.print(directionToString(direction));

    if(connectionState == NEIGH_CONNECTSTATE_CONNECTED)
    {
        Serial.print(F(" - Connected to "));
        Serial.print(neighborAdress);
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

void CompassConnector::forgetNeighbor()
{
    /*
    Serial.print(F("Forced to forget neighbor "));
    Serial.print(neighborAdress);
    Serial.print(F(" on side "));
    Serial.print(directionToString(direction));
    Serial.print(F(" - Neigbor's side: "));
    Serial.println(directionToString(neighborDirection));
    */

    saveNeighborData(ADRESS_UNKNOWN, DIRECTION_NONE);
    connectionState = NEIGH_CONNECTSTATE_DISCONNECTED;
}
uint64_t CompassConnector::getLastPulseTime()
{
    return lastMillis_SyncPulse;
}
uint8_t CompassConnector::getNeighborAdress()
{
    return neighborAdress;
}

uint8_t CompassConnector::getDirection()
{
    return direction;
}
