#ifndef CONNECTORMANAGER_H
#define CONNECTORMANAGER_H

#include "setup.h"
#include "CompassConnector/CompassConnector.h"


#define INTERVAL_SYNCPULSE 1000             // 1 second

#define MESSAGE_CLCO_NEWCLIENTTEMPLATE  "NwClT" //NwClT (macAdress)(TemplateID)
#define MESSAGE_COCL_IDASSIGNMENT       "IDAss" //IDAss (macAdress)(moduleAdress)(sessionID)
#define MESSAGE_CLCO_CONNECTIONCHANGED  "ConCh" //ConCh (updateCode)
#define MESSAGE_COCL_UPDATEREQUEST      "UpReq" //UpReq 
#define MESSAGE_COCL_NEWEFFECT          "NewFX" //NewFX (effectCode)
#define MESSAGE_DUPL_SESSIONCHECK       "SeChk" //SeChk (sessionID)
#define MESSAGE_HOCO_HORNTRIGGERED      "HorTr" //HorTr 

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