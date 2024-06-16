#ifndef COMPASSCONNECTOR_H
#define COMPASSCONNECTOR_H

#include <vector>
#include "setup.h"
#include "SoftwareSerial.h"
#include "pipePieces.h"

#define SOFTSERIALTIMOUTTIME 10 //10ms

#define NEIGH_CONNECTSTATE_UNKNOWN              0
#define NEIGH_CONNECTSTATE_DISCONNECTED         1
#define NEIGH_CONNECTSTATE_CONNECTED            2
#define NEIGH_CONNECTSTATE_BLOCKED              3


#define ADRESS_UNKNOWN "000000"

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

struct LedUpdate
{
  uint8_t inputPanel;
  uint8_t outputPanel;
  uint8_t colour;
  uint16_t offset;
};


struct Transmission
{
    //Message identificaiton.
    String goalMac;         //Mac adress of the module that needs to receive this message. "MASTER" for master
    uint8_t messageID;      //Unique ID for this message, so that no messages are retransmitted twice.
    String messageType;     //Type of message that is contained in "message"
    //Message content
    String message;
    //Local
    uint8_t connectorID;    //Directional Pin/port on which this message was received
};

class CompassConnector
{
    private:
    uint8_t pin_ident;
    uint8_t pin_sync;
    uint8_t direction;
    uint8_t connectionState;
    String neighborMacAdress;
    uint8_t neighborDirection;
    uint64_t lineClaimMillis;
    String macAdress;
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
    void transmit(); //transmit_pinInfo
    bool readData(uint8_t *newNeighborAdress, uint8_t *newNeighborDirection); //read_pinInfo
    uint8_t waitAndRead(bool *timedOut, uint16_t timeoutMillis);
    void transmitAck(); //transmit_ack
    

    void saveNeighborData(String newNeighborMacAdress, uint8_t newNeighborDirection);

    

    public:
    CompassConnector(uint8_t pin_ident, uint8_t pin_sync, uint8_t direction, String macAdress);
    void tick();
    
    void handlePinTest();
    Transmission handleMessageRead();

    void sendPulse_Ident();
    void sendPulse_Sync();
    String getUpdateCode();
    void printConnector();
    void forgetNeighbor();
    uint64_t getLastPulseTime();
    String getNeighborMacAdress();
    uint8_t checkLineClaimed();
    uint8_t getDirection();
    void queueTransmission(Transmission transmission);

    void transmit_busy();
};

String directionToString(uint8_t compassDirection);



#endif