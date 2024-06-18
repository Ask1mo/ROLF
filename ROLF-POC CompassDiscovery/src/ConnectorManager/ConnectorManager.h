#ifndef CONNECTORMANAGER_H
#define CONNECTORMANAGER_H

#include "setup.h"
#include "CompassConnector/CompassConnector.h"


#define INTERVAL_SYNCPULSE 1000             // 1 second


class ConnectorManager
{
    private:
    CompassConnector **compassConnectors;
    bool connectComplete;

    uint64_t lastIdentPulseMillis;

    uint8_t directionsTurn;


    public:
    ConnectorManager(uint8_t *moduleAdress);
    void tick();
    void sendSyncSignal();
    String getUpdateCode();
    void printConnectors();
    void addLedPipe(uint8_t direction, uint8_t basePipeType);
};



#endif