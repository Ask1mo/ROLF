#ifndef COMPASSCONNECTOR_H
#define COMPASSCONNECTOR_H

#include <vector>
#include "setup.h"
#include "SoftwareSerial.h"

#define SOFTSERIALTIMOUTTIME 10             //Time to wait for new characters before assuming the transmission timed out. Used in waitAndRead()
#define MAXINTERVALMILLIS_PINTEST 10000     // If no new pintest is successful in x millis. It is assumed the neighbor is gone.
#define CHARFREETIME 15                     //Time to wait for a new char to arrive before assuming the transmission is done. Only used in checkLineClaimed to wait for a bad transmission to finish.

#define NEIGH_CONNECTSTATE_UNKNOWN              0 //No neighbor is known
#define NEIGH_CONNECTSTATE_DISCONNECTED         1 //Neighbor is known to be disconnected
#define NEIGH_CONNECTSTATE_CONNECTED            2 //Neighbor is known to be connected
#define NEIGH_CONNECTSTATE_BLOCKED              3 //This is selected from the master. Or manually. This is to block broken modules or otherwise unreliable connections. This is not implemented.

#define ADRESS_UNKNOWN "000000"

#define TRANSMISSIONTYPE_BUSY           'B'//This char never gets transmitted or received. It is used to indicate that the neighbor is busy in other functions.
#define TRANSMISSIONTYPE_PINTEST        'P' //Means a pintest transmission is coming
#define TRANSMISSIONTYPE_MESSAGE        'M' //Means a message transmission is coming

#define LINEMODE_IDLE         0 //DigialReading for neighbor to claim the line
#define LINEMODE_PULLCLAIMED  1 //Claiming the line low in preparation for sending a transmission
#define LINEMODE_BUSY         2 //Same as pullClaimed, but the neighbor is busy. So with more checks from self to keep us from sending new transmissions.
#define LINEMODE_SERIAL_READ  3 //Reading serial data of transmission
#define LINEMODE_SERIAL_WRITE 4 //Writing serial data of transmission

#define MESSAGELENGTH_PINTEST 7 //Test to see if a transmission was the correct length to be a pintest transmission


class CommsConnector
{
private:
//Static
    uint8_t                 pin_comms;                                              //Pin on which the communication is done
    uint8_t                 pin_ledSync;                                            //Pin to sync leds with
    uint8_t                 direction;                                              //Direction of the connector relative to the esp/core/middle of the module.
    String                  macAdress;                                              //Mac adress of this module
//Neighbor data
    uint8_t                 connectionState         = NEIGH_CONNECTSTATE_UNKNOWN;   //State of the connection with the neighbor
    String                  neighborMacAdress       = ADRESS_UNKNOWN;               //Mac adress of the neighbor
    uint8_t                 neighborDirection       = DIRECTION_NONE;               //The direction on which this module is connected to on the NEIGHBORS side.
    bool                    neighborBusy            = false;                        //If the neighbor is busy, it will not start any new transmissions. In or out.
//Comms pin state
    uint8_t                 lineMode                = LINEMODE_IDLE;                //Mode of the line. (Idle, PullClaimed, busy, SerialRead, SerialWrite). See defines above for uses.
    uint64_t                lastMillis_CharReceived = 0;                            //Last time the line mode was changed
//Buffers
    std::vector<String>     updateCodes;                                            //Buffer: Update codes contain a pintest update
//Misc
    uint64_t                lastMillis_PinTest      = 0;                            //Last time a pinTest was succesfully received
    EspSoftwareSerial::UART *softwareSerial;                                        //Software serial object
//Transmissions
    bool          waitAndRead           (uint8_t *data);        //Returns false if unsuccessful (timout)
    void          setLineMode           (uint8_t lineMode);     //Manages: Claiming line. Releasing line. Setting line to busy. Setting line to read/write
    void          saveNeighborData      (String newNeighborMacAdress, uint8_t newNeighborDirection); //Saves the neighbor data
    void          pinTransmitV2         (String data);          //Transmit DATA over the comms pin. Only one to do so.
public:
//Constructor
    CommsConnector                    (uint8_t pin_comms, uint8_t pin_ledSync, uint8_t direction, String macAdress);
//Loop
    void          tick                  (); //Loop
//Transmissions
    uint8_t       checkLineClaimed      (); //Check if neighbor is prepping to send a message. (Or if it is busy)
    void          handlePinTest         (); //Handle(read) an incoming pin test. Neighbor is informing us of it's mac and direction
    Transmission  handleMessageRead     (); //Handle(read) an incoming message. Neighbor is sending us a message
    void          transmit_pinTest      (); //Transmit a pin test. Inform the neighbor of our mac and direction
    void          setBusy               (bool busy); //Pull pin to low to timeout on neighbor side. This means pin is busy.
//Misc
    void          forgetNeighbor        (); //Forget the neighbor. It must have exceeded it's timeout (Not to be confused with waitAndRead timeout)
//Getters
    uint64_t      getLastMillis_PinTest (); //Get the last time a pinTest was succesfully received
    String        getConnectionUpdate   (); //Get the update code for the master. UpdateCodes contain a pintest update
    String        getNeighborMacAdress  (); //Get the mac adress of the neighbor
    uint8_t       getDirection          (); //Get the direction of this connector
//Debug
    void          printConnector        (); //Print the connector's data
};

#endif