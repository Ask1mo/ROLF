#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>
#include <vector>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "setup.h"

#include "CompassConnector/CompassConnector.h"



//Message structure: "goalMac, messageID, messageType, message"
//messageTypes and what message they contain:
#define MESSAGETYPE_COCL_REQUESTTEMPLATE    "TeReq" //TeReq N/A                     If master recevies command from module it does not know. Ask the module to send its template.
#define MESSAGETYPE_CLCO_NEWCLIENTTEMPLATE  "NwClT" //NwClT (originMac)(TemplateID) When client comes online. Inform master of it's template. (Can also be sent if master asks for it "TeReq")
#define MESSAGETYPE_CLCO_CONNECTIONCHANGED  "ConCh" //ConCh (updateCode)            When a connection on a pin on a module changes. Inform the master of the change. UpdateCode contains mac adresses
#define MESSAGETYPE_COCL_UPDATEREQUEST      "UpReq" //UpReq N/A                     Master asks module to send all it's connections. Just in case it missed one.
#define MESSAGETYPE_COCL_NEWEFFECT          "NewFX" //NewFX (effectCode)            Master sends a new effect to the module.
#define MESSAGETYPE_HOCO_HORNTRIGGERED      "HorTr" //HorTr (originMac)             When someone talks into a horn. Inform the master.


 

class Comms
{
private:

    WiFiUDP udp;
    uint8_t sessionID = 0;
    uint64_t lastMillis_SessionCheck = 0;
    CompassConnector *connector;
    std::vector<ModuleChangeInfo> moduleChangeBuffer;
    std::vector<NewClientInfo> newClientBuffer;
    std::vector<uint8_t> triggeredHornsIDs;

    uint8_t lastMessageID = 0;
    void reboot(String message);
    
    void receiveAndParse();

public:
    Comms();
    void tick();
    void connect();
    void transmit(String message, String clientIP);
    std::vector<ModuleChangeInfo> getModuleChangeBuffer();
    std::vector<NewClientInfo> getNewClientBuffer();
    std::vector<uint8_t> getTriggeredHornsIDs();
    void setSessionID(uint8_t sessionID);
};

#endif