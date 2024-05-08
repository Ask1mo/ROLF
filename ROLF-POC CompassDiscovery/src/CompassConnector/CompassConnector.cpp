#include "CompassConnector.h"

CompassConnector::CompassConnector(uint8_t pin, uint8_t direction, String textName, uint8_t *moduleAdress)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating CompassConnector at adress "));
        Serial.println((int)this, DEC);
    }

    this->pin = pin;
    this->direction = direction;
    this->textName = textName;
    this->moduleAdress = moduleAdress;
    this->connectionState = NEIGH_CONNECTSTATE_UNKNOWN;
    this->neighborAdress = NEIGH_ADRESS_UNKNOWN;
    this->neighborDirection = DIRECTION_NONE;
}

void CompassConnector::claimLine(bool enabled)
{
    if (enabled)// Claim line by pulling low
    {   
        Serial.println("Line Claimed");
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    else //Release line back to high
    {
        pinMode(pin, INPUT_PULLUP);
        Serial.println("Line Released");
    }
}
bool CompassConnector::checkLineClaimed()
{
    Serial.print("Checking line: ");
    Serial.println(digitalRead(pin));
    return !digitalRead(pin);
}
bool CompassConnector::tick()
{
    if (connectionState == NEIGH_CONNECTSTATE_BLOCKED) return false;


        if(checkLineClaimed())
        {
            //Check line claim duration
            uint64_t lineClaimMillis = millis();
            while (checkLineClaimed())
            {
                delay(1);
                Serial.print(".");
            }
            Serial.println();
   
            //Measure pulse length
            if (millis() - lineClaimMillis > PULSELENGTH) //If it was a IDENT message
            {
                Serial.println(F("!!!IDENT REQUEST RECEIVED!!!"));
                delay(1000);
                transmit(NEIGH_ADRESS_MASTER);
                return false;
            }
            else //If it was a LED Sync pulse
            {
                Serial.println(F("!!!LED SYNC RECEIVED!!!"));
                delay(1000);
                return true;
            }     
        }
    return false;
}


void CompassConnector::transmit(uint8_t adress)
{
    Serial.print(F("Transmitting "));
    Serial.println(textName);

    softwareSerial = new SoftwareSerial(PIN_DEAD, pin);
    softwareSerial->begin(9600);
    
    softwareSerial->write(adress);
    softwareSerial->write(direction);
    
    softwareSerial->end();

    delete static_cast<SoftwareSerial*>(softwareSerial);
    claimLine(false); //To make sure the data line returns to HIGH, release it.
}


uint8_t CompassConnector::getConnectionState()
{
    return connectionState;
}

void CompassConnector::connect()
{
    claimLine(true);
    //lineClaimMillis = millis();
    delay(60);
    claimLine(false);

    Serial.println(F("Attempting Serial connection"));

    if (readData())
    {
        connectionState = NEIGH_CONNECTSTATE_CONNECTED;
        Serial.println(F("Connected"));
    }
    else
    {
        connectionState = NEIGH_CONNECTSTATE_DISCONNECTED;
        Serial.println(F("No neighbor found"));
    }
}
bool CompassConnector::readData()
{
    Serial.println(F("Reading data"));

    uint8_t readResult = waitAndRead();
    if(readResult == false)
    {
        neighborAdress = NEIGH_ADRESS_UNKNOWN;
        neighborDirection = DIRECTION_NONE;
        return false;
    }
    else neighborAdress = readResult;
    

    readResult = waitAndRead();
    if(readResult == false)
    {
        neighborAdress = NEIGH_ADRESS_UNKNOWN;
        neighborDirection = DIRECTION_NONE;
        return false;
    }
    else neighborDirection = readResult;

    Serial.print(F("Neighbor adress: "));
    Serial.println(neighborAdress);
    Serial.print(F("Neighbor direction: "));
    Serial.println(neighborDirection);

    
    return true;
}
uint8_t CompassConnector::waitAndRead()
{
    softwareSerial = new SoftwareSerial(pin, PIN_DEAD);
    softwareSerial->begin(9600);

    uint8_t timeoutAttempts = 100;
    while (!softwareSerial->available())
    {
        delay(1);
        Serial.print(".");
        timeoutAttempts--;
        if (timeoutAttempts == 0)
        {
            Serial.println(F("Timeout"));
            softwareSerial->end();
            delete static_cast<SoftwareSerial*>(softwareSerial);
            return false;
        }
    }
    uint8_t readResult = softwareSerial->read();
    Serial.print(F("Read: "));
    Serial.println(readResult);
    softwareSerial->end();
    delete static_cast<SoftwareSerial*>(softwareSerial);
    return readResult;
}