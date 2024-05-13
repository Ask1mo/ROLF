#ifndef COMPASSCONNECTOR_H
#define COMPASSCONNECTOR_H

#include "setup.h"
#include "SoftwareSerial.h"

#define BUTTON_RELEASED             0
#define BUTTON_TAPPED               1
#define BUTTON_HELD                 2
#define BUTTON_ERROR                3 
#define BUTTON_PRESSSTATE_HIGH      1 //This one is only for getPressState()

#define NEIGH_CONNECTSTATE_UNKNOWN              0
#define NEIGH_CONNECTSTATE_DISCONNECTED         1
#define NEIGH_CONNECTSTATE_CONNECTED            2
#define NEIGH_CONNECTSTATE_BLOCKED              3

#define LINESTATE_CLEAR         0
#define LINESTATE_CLAIMED       1
#define LINESTATE_CLAIMED_LEDS  2
#define LINESTATE_CLAIMED_IDENT 3

#define ADRESS_UNKNOWN 0

#define DIRECTIONS 6
#define DIRECTION_NONE  0
#define DIRECTION_NORTH 1
#define DIRECTION_EAST  2
#define DIRECTION_SOUTH 3
#define DIRECTION_WEST  4
#define DIRECTION_UP    5
#define DIRECTION_DOWN  6

#define CONNECTOR_COMMAND_IDENT     1
#define CONNECTOR_COMMAND_LEDSYNC   2

#define SERIALMODE_DISABLED 0
#define SERIALMODE_READ     1
#define SERIALMODE_WRITE    2




class CompassConnector
{
    private:
    uint8_t pin;
    uint8_t direction;
    String textName;
    uint8_t connectionState;
    uint8_t neighborAdress;
    uint8_t neighborDirection;
    uint64_t lineClaimMillis;
    uint8_t *moduleAdress;
    bool serialMode;

    //SoftwareSerial* softwareSerialTransmit;
    //SoftwareSerial* softwareSerialReceive;

    EspSoftwareSerial::UART *softwareSerial;

    void claimLine(bool enabled); //High to pull the line down and claim it.

 
    public:
    CompassConnector(uint8_t pin, uint8_t direction, String textName, uint8_t *moduleAdress);
    void prepareSerial_Read();
    void prepareSerial_Write();
    void transmit();
    void waitAndReceive();
    bool checkLineClaimed();
    bool lineCheckTick();
    uint8_t getConnectionState();
    bool tick();
    void connect();
    bool readData();
    uint8_t waitAndRead();
    void sendSyncSignal();
};



#endif