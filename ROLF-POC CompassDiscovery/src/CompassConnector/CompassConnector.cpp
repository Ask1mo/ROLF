#include "CompassConnector.h"


//Constructor
CompassConnector::CompassConnector(uint8_t pin_ident, uint8_t pin_sync, uint8_t direction, String textName, uint8_t *moduleAdress)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating CompassConnector at adress "));
        Serial.println((int)this, DEC);
    }

    this->pin_ident = pin_ident;
    this->pin_sync = pin_sync;
    this->direction = direction;
    this->textName = textName;
    this->moduleAdress = moduleAdress;
    this->connectionState = NEIGH_CONNECTSTATE_UNKNOWN;
    this->neighborAdress = ADRESS_UNKNOWN;
    this->neighborDirection = DIRECTION_NONE;
    //this->softwareSerialTransmit = new SoftwareSerial(PIN_DEAD, pin_ident);
    //this->softwareSerialReceive = new SoftwareSerial(pin_ident, PIN_DEAD);

    Serial.print(F("CompassConnector created: "));
    Serial.println(*moduleAdress);

    serialMode = SERIALMODE_DISABLED;

    pinMode(pin_ident, INPUT);
    int readResult = analogRead(pin_ident);
    if (readResult > 3800)
    {
        Serial.print(F("HIGH, MASTER DETECTED ON LINE "));
    }
    else
    {
        Serial.print(F("Pin "));
        Serial.print(pin_ident);
        Serial.print(F(" is low: "));
        Serial.println(readResult);
    }

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
        *newNeighborAdress = ADRESS_UNKNOWN;
        *newNeighborDirection = DIRECTION_NONE;
        return false;
    }
    else *newNeighborAdress = readResult;
    
    Serial.println();

    readResult = waitAndRead();
    if(readResult == false)
    {
        *newNeighborAdress = ADRESS_UNKNOWN;
        *newNeighborDirection = DIRECTION_NONE;
        return false;
    }
    else *newNeighborDirection = readResult;

        
    return true;
}
uint8_t CompassConnector::waitAndRead()
{
    //softwareSerialReceive->begin(9600);
    prepareSerial_Read();

    uint16_t timeoutAttempts = 1000;
    //while (!softwareSerialReceive->available())
    while (!softwareSerial->available())
    {
        Serial.print(".");
        delay(1);
        timeoutAttempts--;
        if (timeoutAttempts == 0)
        {
            Serial.println(F("Timeout"));
            claimLine(false);
            return false;
        }
    }
    //uint8_t readResult = softwareSerialReceive->read();
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
            return "East";
        break;

        case DIRECTION_SOUTH:
            return "South";
        break;

        case DIRECTION_WEST:
            return "West";
        break;

        case DIRECTION_UP:
            return "Up";
        break;

        case DIRECTION_DOWN:
            return "Down";
        break;
    }
    return "ERROR_DIRECTIONTOSTRING";
}
void CompassConnector::saveNeighborData(uint8_t newNeighborAdress, uint8_t newNeighborDirection)
{
    if (newNeighborAdress != neighborAdress || newNeighborDirection != neighborDirection)
    {
        Serial.print(F("Neighbor data changed: "));
        Serial.print(neighborAdress);
        Serial.print(F(" -> "));
        Serial.print(newNeighborAdress);
        Serial.print(F(" - "));
        Serial.print(neighborDirection);
        Serial.print(F(" -> "));
        Serial.println(newNeighborDirection);

        neighborAdress = newNeighborAdress;
        neighborDirection = newNeighborDirection;

        Serial.println();
        Serial.print(F("Neirbor "));
        Serial.print(neighborAdress);
        Serial.print(F(" connected on side "));
        Serial.print(textName);
        Serial.print(F(" - Neigbor's side: "));
        Serial.println(directionToString(neighborDirection));

        updateCode = String(direction) + String(neighborAdress) + String(neighborDirection);

        
    }
}

//Public
void CompassConnector::tick()
{
    uint64_t currentMillis = millis();

    if (connectionState == NEIGH_CONNECTSTATE_BLOCKED) return;

    if(checkLineClaimed())
    {
        lastMillis_SyncPulse = currentMillis;
        uint8_t newNeighborAdress;
        uint8_t newNeighborDirection;
        if (readData(&newNeighborAdress, &newNeighborDirection)) connectionState = NEIGH_CONNECTSTATE_CONNECTED;
        else connectionState = NEIGH_CONNECTSTATE_DISCONNECTED;
        saveNeighborData(newNeighborAdress, newNeighborDirection);
    }
    if (lastMillis_SyncPulse + INTERVAL_SYNCPULSE_MAXABSENCE < currentMillis && connectionState == NEIGH_CONNECTSTATE_CONNECTED)
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
    String updateCodeToSend = updateCode;
    updateCode = "";
    return updateCodeToSend;
}
void CompassConnector::printConnector()
{
    Serial.print(F("Connector: "));
    Serial.print(textName);

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