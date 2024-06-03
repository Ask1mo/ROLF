#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>
#include <vector>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "setup.h"

#define WIFICONNECTATTEMPTS 10

#define MESSAGE_CLCO_NEWCLIENTTEMPLATE  "NwClT" //NwClT(macAdress)(TemplateID)
#define MESSAGE_CLCO_NEWCLIENTFULL      "NwClF" //NwClF(macAdress)(heartPiece)(nl,nd)(el,ed)(sl,sd)(wl,wd)(ul,ud)(dl,dd)
#define MESSAGE_COCL_IDASSIGNMENT       "IDAss" //IDAss(moduleAdress)(sessionID)
#define MESSAGE_CLCO_CONNECTIONCHANGED  "ConCh" //ConCh(updateCode)
#define MESSAGE_COCL_UPDATEREQUEST      "UpReq" //UpReq
#define MESSAGE_COCL_NEWEFFECT          "NewFX" //NewFX(effectCode)
#define MESSAGE_DUPL_SESSIONCHECK       "SeChk" //SeChk(sessionID)
#define MESSAGE_HOCO_HORNTRIGGERED      "HorTr" //HorTr

 

class Comms
{
private:

    WiFiUDP udp;
    uint8_t sessionID = 0;
    uint64_t lastMillis_SessionCheck = 0;
    std::vector<ModuleChangeInfo> moduleChangeBuffer;
    std::vector<NewClientInfo> newClientBuffer;
    std::vector<uint8_t> triggeredHornsIDs;
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