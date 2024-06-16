#ifndef CONNECTORMANAGER_H
#define CONNECTORMANAGER_H

#include "setup.h"
#include "CompassConnector/CompassConnector.h"


#define INTERVAL_SYNCPULSE 1000             // 1 second

//Message structure: "goalMac, messageID, messageType, message"
//messageTypes and what message they contain:
#define MESSAGETYPE_COCL_REQUESTTEMPLATE    "TeReq" //TeReq N/A                     If master recevies command from module it does not know. Ask the module to send its template.
#define MESSAGETYPE_CLCO_NEWCLIENTTEMPLATE  "NwClT" //NwClT (originMac)(TemplateID) When client comes online. Inform master of it's template. (Can also be sent if master asks for it "TeReq")
#define MESSAGETYPE_CLCO_CONNECTIONCHANGED  "ConCh" //ConCh (updateCode)            When a connection on a pin on a module changes. Inform the master of the change. UpdateCode contains mac adresses
#define MESSAGETYPE_COCL_UPDATEREQUEST      "UpReq" //UpReq N/A                     Master asks module to send all it's connections. Just in case it missed one.
#define MESSAGETYPE_COCL_NEWEFFECT          "NewFX" //NewFX (effectCode)            Master sends a new effect to the module.
#define MESSAGETYPE_HOCO_HORNTRIGGERED      "HorTr" //HorTr (originMac)             When someone talks into a horn. Inform the master.

class ConnectorManager
{
    private:
    CompassConnector **compassConnectors;
    bool connectComplete;

    uint64_t lastIdentPulseMillis;

    uint8_t directionsTurn;

    uint8_t moduleID = 0;
    uint8_t sessionID = 0;
    uint8_t lastMessageID = 0;

    void setConnectorsToBusy(uint8_t directionToIgnore);
    void setConnectorsToFree();
    void reboot(String message);//Odd place to have this here.

    



    public:
    ConnectorManager(String macAdress);
    void tick();
    void sendSyncSignal();
    String getUpdateCode();
    void printConnectors();
    void addLedPipe(uint8_t direction, uint8_t basePipeType);
    void setSystemOccupied();
    void setSystemFree();
    void lockSystemOccupied();
    void releaseSystemOccupied();
    void parseTransmission(String message);
    void transmit(Transmission transmission); 
};



#endif