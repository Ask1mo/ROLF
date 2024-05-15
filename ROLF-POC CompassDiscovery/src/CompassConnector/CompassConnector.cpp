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
bool CompassConnector::checkLineClaimed()
{
    return !digitalRead(pin_ident);
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
bool CompassConnector::readData(uint8_t *newNeighborAdress, uint8_t *newNeighborDirection)
{
    uint8_t readResult = waitAndRead();
    if(readResult == false)
    {
        connectionState = NEIGH_CONNECTSTATE_UNKNOWN;
        *newNeighborAdress = ADRESS_UNKNOWN;
        *newNeighborDirection = DIRECTION_NONE;
        return false;
    }
    else *newNeighborAdress = readResult;

    readResult = waitAndRead();
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
uint8_t CompassConnector::waitAndRead()
{
    prepareSerial_Read();

    uint16_t timeoutAttempts = 1000;
    while (!softwareSerial->available())
    {
        delay(1);
        timeoutAttempts--;
        if (timeoutAttempts == 0)
        {
            Serial.println(F("Timeout"));
            claimLine(false);
            return false;
        }
    }
    uint8_t readResult = softwareSerial->read();
    claimLine(false);
    return readResult;
}
String CompassConnector::directionToString(uint8_t compassDirection)
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
        updateCodes.push_back(String(char(*moduleAdress)+ char(direction) + char(neighborAdress) + char(neighborDirection)));

        /*
        Serial.println();
        Serial.print(F("Neirbor "));
        Serial.print(neighborAdress);
        Serial.print(F(" connected on side "));
        Serial.print(directionToString(direction));
        Serial.print(F(" - Neigbor's side: "));
        Serial.println(directionToString(neighborDirection));
        */      
    }
}

//Public
void CompassConnector::tick()
{
    if (connectionState == NEIGH_CONNECTSTATE_BLOCKED)return; //Early return if module is chosen to be unreliable

    uint64_t currentMillis = millis();

    if(checkLineClaimed())
    {
        lastMillis_SyncPulse = currentMillis;
        uint8_t newNeighborAdress;
        uint8_t newNeighborDirection;
        readData(&newNeighborAdress, &newNeighborDirection);
        saveNeighborData(newNeighborAdress, newNeighborDirection);
    }
    if (lastMillis_SyncPulse + INTERVAL_SYNCPULSE_MAXABSENCE < currentMillis && connectionState != NEIGH_CONNECTSTATE_DISCONNECTED)
    {
        saveNeighborData(ADRESS_UNKNOWN, DIRECTION_NONE);
        connectionState = NEIGH_CONNECTSTATE_DISCONNECTED;
    }
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
