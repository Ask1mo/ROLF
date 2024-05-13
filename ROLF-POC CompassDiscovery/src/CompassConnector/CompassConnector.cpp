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
bool CompassConnector::readData()
{
    uint8_t readResult = waitAndRead();
    if(readResult == false)
    {
        neighborAdress = ADRESS_UNKNOWN;
        neighborDirection = DIRECTION_NONE;
        return false;
    }
    else neighborAdress = readResult;
    
    Serial.println();

    readResult = waitAndRead();
    if(readResult == false)
    {
        neighborAdress = ADRESS_UNKNOWN;
        neighborDirection = DIRECTION_NONE;
        return false;
    }
    else neighborDirection = readResult;

        
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

//Public
void CompassConnector::tick()
{
    if (connectionState == NEIGH_CONNECTSTATE_BLOCKED) return;

    if(checkLineClaimed())
    {
        //Check line claim duration
        uint64_t lineClaimMillis = millis();
        
        if (readData())
        {
            connectionState = NEIGH_CONNECTSTATE_CONNECTED;
            
            Serial.println();
            Serial.print(F("Neirbor "));
            Serial.print(neighborAdress);
            Serial.print(F(" connected on side "));
            Serial.print(textName);
            Serial.print(F(" - Neigbor's side: "));
            Serial.println(directionToString(neighborDirection));
        }
        else
        {
            connectionState = NEIGH_CONNECTSTATE_DISCONNECTED;
            Serial.println(F("No neighbor found"));
        }
    }
}
void CompassConnector::sendPulse_Ident()
{
    claimLine(true);
    delay(PULSELENGTH_SYNC);
    transmit();
    Serial.println(F("IDPulse sent"));
}
void CompassConnector::sendPulse_Sync()
{
    pinMode(pin_sync, OUTPUT);
    digitalWrite(pin_sync, LOW);
    delay(PULSELENGTH_SYNC);
    pinMode(pin_sync, INPUT_PULLUP);
}