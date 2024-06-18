#ifndef CONNECTORMANAGER_H
#define CONNECTORMANAGER_H

#include "setup.h"
#include "CompassConnector/CompassConnector.h"

#define INTERVAL_PINTEST 1000             //Interval at which the next pin test is done

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
//Static
    String                  macAdress;                                                                      //Mac adress of this module
    CompassConnector        **compassConnectors;                                                            //Array of connectors in this module
//Pin tests
    uint64_t                lastMillis_PinTest;                                                             //Last time a pin test was done
    uint8_t                 currentPinTestDirection = 0;                                                    //Current pin test direction
//Retransmission prevention
    uint8_t                 lastMessageID = 0;                                                              //Last transmissionID created for transmessions generated from this module
//Transmission buffers
    std::vector<LedUpdate>  ledUpdates_buffer;                                                              //Quick dump so incoming transmissions are always stored
//Transmissions
    void                    parseTransmission   (String message);                                           //Parse incoming transmissions read from the connectors
    void                    retransmit          (Transmission transmission);                                //Used if a transmission has been received that's not meant for this module. Sends the message to all connectors for transmisison.
    void                    setSystemBusy       (uint8_t directionThatsActuallyDoingSomething, bool busy);  //Set all other pins busy so no transmissions are missed on those pins
//Debug
    void                    printConnectors     ();                                                         //Print all connectors
public:
//Constructor
    ConnectorManager                            (String macAdress);                                         //Constructor
//Loop
    void                    tick                ();                                                         //Loop
//Transmissions
    void                    transmit            (String messageType, String message = "");                  //Generate new transmission and transmit
//Getters
    std::vector<LedUpdate>  getLedUpdates       ();                                                         //Get all led updates            
};
#endif