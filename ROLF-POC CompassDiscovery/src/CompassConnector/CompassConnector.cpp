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
    this->neighborAdress = ADRESS_UNKNOWN;
    this->neighborDirection = DIRECTION_NONE;
    //this->softwareSerialTransmit = new SoftwareSerial(PIN_DEAD, pin);
    //this->softwareSerialReceive = new SoftwareSerial(pin, PIN_DEAD);

    Serial.print(F("CompassConnector created: "));
    Serial.println(*moduleAdress);

    serialMode = SERIALMODE_DISABLED;

    claimLine(false);
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
    softwareSerial->begin(BAUDRATE_SYSTEM, EspSoftwareSerial::SWSERIAL_8N1, pin, PIN_DEAD, false, 256);
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
    softwareSerial->begin(BAUDRATE_SYSTEM, EspSoftwareSerial::SWSERIAL_8N1, PIN_DEAD, pin, false, 256);
    Serial.println(F("Serial started"));
}

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
        Serial.println(pin);
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    else //Release line back to high
    {
        Serial.print(F("Releasing Line "));
        Serial.println(pin);

        pinMode(pin, INPUT_PULLUP);
    }

    Serial.println("Line done");
}
bool CompassConnector::checkLineClaimed()
{
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
                Serial.print(".");
            }
            Serial.println();
   
            //Measure pulse length
            if (millis() - lineClaimMillis > PULSELENGTH) //If it was a IDENT message
            {
                Serial.println(F("!!!IDENT REQUEST RECEIVED!!!"));
                transmit();
                return false;
            }
            else //If it was a LED Sync pulse
            {
                Serial.println(F("!!!LED SYNC RECEIVED!!!"));
                return true;
            }     
        }
    return false;
}


void CompassConnector::transmit()
{
    Serial.print(F("Transmitting "));
    Serial.print(textName);
    Serial.print(F(": Adress "));
    Serial.print(*moduleAdress);
    Serial.print(F(" Direction "));
    Serial.println(direction);


    



    /*
    softwareSerialTransmit->begin(9600);
    softwareSerialTransmit->write(adress);
    softwareSerialTransmit->write(direction);
    softwareSerialTransmit->end();
    */

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

    softwareSerial->write('S'); //Dummy data

    Serial.println(F("Dummy sent"));
    
    softwareSerial->write(*moduleAdress);
    softwareSerial->write(direction);

    Serial.println(F("Data sent"));


    claimLine(false); //To make sure the data line returns to HIGH, release it.
    Serial.println(F("Line released"));
}


uint8_t CompassConnector::getConnectionState()
{
    return connectionState;
}

void CompassConnector::connect()
{
    claimLine(true);
    //lineClaimMillis = millis();
    delay(PULSELENGTH*1.2);
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

    uint8_t timeoutAttempts = 100;
    //while (!softwareSerialReceive->available())
    while (!softwareSerial->available())
    {
        Serial.print(".");
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