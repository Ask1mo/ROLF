#ifndef COMPASSCONNECTOR_H
#define COMPASSCONNECTOR_H

#include <vector>
#include "setup.h"
#include "SoftwareSerial.h"
#include "pipePieces.h"

#define NEIGH_CONNECTSTATE_UNKNOWN              0
#define NEIGH_CONNECTSTATE_DISCONNECTED         1
#define NEIGH_CONNECTSTATE_CONNECTED            2
#define NEIGH_CONNECTSTATE_BLOCKED              3


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

#define PULSELENGTH_SYNC 5
#define INTERVAL_SYNCPULSE_MAXABSENCE 10000 // 10 seconds


#define TRANSMISSIONTYPE_BUSY           'B'
#define TRANSMISSIONTYPE_PINTEST        'P'
#define TRANSMISSIONTYPE_MESSAGE        'M'

struct Transmission
{
    bool isIdentMessage;      //True if the message is an ident message, ident messages assign the moduleID to the module. If true goalID will be 255 in which case the MAC adress in the message will have to be parsed to kinda get the goal module (Instead of goalID)
    uint8_t goalID;         //0 for master, 255 for unknown module (broadcast)
    uint8_t senderID;       //0 for master, 255 for unknown module (broadcast).     Sender ID will be the last digit of the macAdress of the sender if IdentMessage is true. To avoid message being lost.
    uint8_t connectorID;    //Directional Pin/port on which this message was received
    uint64_t messageID;
    String message;
};

class CompassConnector
{
    private:
    uint8_t pin_ident;
    uint8_t pin_sync;
    uint8_t direction;
    uint8_t connectionState;
    uint8_t neighborAdress;
    uint8_t neighborDirection;
    uint64_t lineClaimMillis;
    uint8_t *moduleAdress;
    bool serialMode;
    std::vector<String> updateCodes;
    uint64_t lastMillis_SyncPulse;
    bool neighborIsBusy = false;
    void (*lockSystemOccupied)();
    void (*releaseSystemOccupied)();
    EspSoftwareSerial::UART *softwareSerial;
    std::vector<Transmission> transmissions;

    void claimLine(bool enabled); //High to pull the line down and claim it.
    

    void prepareSerial_Read();
    void prepareSerial_Write();
    void transmit();
    bool readData(uint8_t *newNeighborAdress, uint8_t *newNeighborDirection);
    uint8_t waitAndRead(bool *timedOut, uint16_t timeoutMillis);
    void transmitAck();
    

    void saveNeighborData(uint8_t newNeighborAdress, uint8_t newNeighborDirection);

    

    public:
    CompassConnector(uint8_t pin_ident, uint8_t pin_sync, uint8_t direction, uint8_t *moduleAdress);
    void tick();
    
    void handlePinTest();
    Transmission handleMessageRead();

    void sendPulse_Ident();
    void sendPulse_Sync();
    String getUpdateCode();
    void printConnector();
    void forgetNeighbor();
    uint64_t getLastPulseTime();
    uint8_t getNeighborAdress();
    uint8_t checkLineClaimed();
    uint8_t getDirection();
    void queueTransmission(Transmission transmission);
};

String directionToString(uint8_t compassDirection);



#endif