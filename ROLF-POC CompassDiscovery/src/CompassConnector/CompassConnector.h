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

    //SoftwareSerial* softwareSerialTransmit;
    //SoftwareSerial* softwareSerialReceive;

    EspSoftwareSerial::UART *softwareSerial;

    void claimLine(bool enabled); //High to pull the line down and claim it.
    bool checkLineClaimed();

    void prepareSerial_Read();
    void prepareSerial_Write();
    void transmit();
    bool readData(uint8_t *newNeighborAdress, uint8_t *newNeighborDirection);
    uint8_t waitAndRead();
    String directionToString(uint8_t compassDirection);

    void saveNeighborData(uint8_t newNeighborAdress, uint8_t newNeighborDirection);

    

    public:
    CompassConnector(uint8_t pin_ident, uint8_t pin_sync, uint8_t direction, uint8_t *moduleAdress);
    void tick();
    void sendPulse_Ident();
    void sendPulse_Sync();
    String getUpdateCode();
    void printConnector();
    void forgetNeighbor();
    uint64_t getLastPulseTime();
    uint8_t getNeighborAdress();
    
};



#endif