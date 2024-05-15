#ifndef CONNECTORMANAGER_H
#define CONNECTORMANAGER_H

#include "setup.h"
#include "CompassConnector/CompassConnector.h"


#define IDENTPULSEINTERVAL 1000

class ConnectorManager
{
    private:
    CompassConnector **compassConnectors;
    CompassConnector *targetConnector;
    bool connectComplete;

    uint64_t lastIdentPulseMillis;

    uint8_t directionsTurn;


    public:
    ConnectorManager(uint8_t *moduleAdress);
    void tick();
    void sendSyncSignal();
    String getUpdateCode();
    void printConnectors();
};



#endif