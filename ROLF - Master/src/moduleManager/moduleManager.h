#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

#include <vector>
#include "connectedModule/connectedModule.h"
#include "setup.h" 

#define ADDR_NONE 0




class ModuleManager
{
    private:
    std::vector<ConnectedModule*> connectedModules;

    


    bool    checkModuleID(uint8_t moduleID);
    uint8_t getModuleID_macAdress(String macAdress);
    uint8_t findModule_ipAdress(String ipAdress);
    bool    updateModule(uint8_t moduleID, String macAdress, String ipAdress);
    ConnectedModule *getModule(uint8_t moduleID);

    public:
    ModuleManager();
    void tick();
    uint8_t addNewModule(String macAdress, String ipAdress, BaseInfo baseInfo);
    void updateModuleConnection(uint8_t moduleID, uint8_t direction, uint8_t neighborID, uint8_t neighborDirection);
};



#endif