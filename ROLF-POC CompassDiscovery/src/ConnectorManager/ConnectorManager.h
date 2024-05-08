#ifndef CONNECTORMANAGER_H
#define CONNECTORMANAGER_H

#include "setup.h"
#include "CompassConnector/CompassConnector.h"

class ConnectorManager
{
    private:
    CompassConnector **compassConnectors;
    CompassConnector *targetConnector;
    bool connectComplete;


    public:
    ConnectorManager(uint8_t *moduleAdress);
    bool tick();
    void connect();
    void sendSyncSignal();
};



#endif