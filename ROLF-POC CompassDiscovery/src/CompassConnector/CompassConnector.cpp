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
        Serial.println(F("Ending Serial"));
        softwareSerial->end();
        delete softwareSerial;
        serialMode = SERIALMODE_DISABLED;
    }
    else
    {
        Serial.println(F("Serial not active"));
    }

    if (enabled)// Claim line by pulling low
    {   
        Serial.print(F("Claiming Line "));
        Serial.println(pin_ident);
        pinMode(pin_ident, OUTPUT);
        digitalWrite(pin_ident, LOW);
    }
    else //Release line back to high
    {
        Serial.print(F("Releasing Line "));
        Serial.println(pin_ident);

        pinMode(pin_ident, INPUT_PULLUP);
    }

    Serial.println("Line done");
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
    Serial.println(F("Starting Serial for Read"));
    serialMode = SERIALMODE_READ;
    softwareSerial = new EspSoftwareSerial::UART();
    softwareSerial->begin(BAUDRATE_SYSTEM, EspSoftwareSerial::SWSERIAL_8N1, pin_ident, PIN_DEAD, false, 256);
    Serial.println(F("Serial started"));
}
void CompassConnector::prepareSerial_Write()
{
    if (serialMode != SERIALMODE_DISABLED)
    {
        softwareSerial->end();
        delete softwareSerial;
    }
    Serial.println(F("Starting Serial for Write"));
    serialMode = SERIALMODE_WRITE;
    softwareSerial = new EspSoftwareSerial::UART();
    softwareSerial->begin(BAUDRATE_SYSTEM, EspSoftwareSerial::SWSERIAL_8N1, PIN_DEAD, pin_ident, false, 256);
    Serial.println(F("Serial started"));
}
void CompassConnector::transmit()
{
    Serial.print(F("Transmitting "));
    Serial.print(textName);
    Serial.print(F(": Adress "));
    Serial.print(*moduleAdress);
    Serial.print(F(" Direction "));
    Serial.println(direction);


    prepareSerial_Write();

    if(moduleAdress == nullptr)
    {
        Serial.println(F("!!!!!!!!!!!!!!!No module adress!!!!!!!!!!!!!!!"));
        return;
    }
    else
    {
        Serial.print(F("Module adress: "));
        Serial.println(*moduleAdress);
    }

    if(&softwareSerial == nullptr) {
    Serial.println(F("softwareSerial is null"));
    } else {
        Serial.println(F("softwareSerial is Initialised"));
    }

    Serial.println(F("Sending data"));

    softwareSerial->write(*moduleAdress);
    delay(100);
    softwareSerial->write(direction);

    Serial.println(F("Data sent"));


    claimLine(false); //To make sure the data line returns to HIGH, release it.
    Serial.println(F("Line released"));
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
    

    readResult = waitAndRead();
    if(readResult == false)
    {
        neighborAdress = ADRESS_UNKNOWN;
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
    Serial.print(F("Read: "));
    Serial.println(readResult);
    claimLine(false);
    return readResult;
}

//Public
void CompassConnector::tick()
{
    if (connectionState == NEIGH_CONNECTSTATE_BLOCKED) return;

    if(checkLineClaimed())
    {
        //Check line claim duration
        uint64_t lineClaimMillis = millis();
        Serial.println(F("!!!IDENT REQUEST RECEIVED!!!"));
        
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
}
void CompassConnector::sendPulse_Ident()
{
    claimLine(true);
    delay(10);
    claimLine(false);
    transmit();
    Serial.println(F("IDPulse sent"));
}
void CompassConnector::sendPulse_Sync()
{
    pinMode(pin_sync, OUTPUT);
    digitalWrite(pin_sync, LOW);
    delay(PULSELENGTH);
    pinMode(pin_sync, INPUT_PULLUP);
}