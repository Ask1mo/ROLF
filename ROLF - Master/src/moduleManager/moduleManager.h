#ifndef CONNECTEDMODULE_H
#define CONNECTEDMODULE_H

#include <vector>
#include "connectedModule/connectedModule.h"
#include "setup.h"

#define ADDR_NONE 0


class ModuleManager
{
    private:
    std::vector<ConnectedModule*> connectedModules;
    bool checkModuleID(uint8_t moduleID);
    uint8_t findModule_macAdress(String macAdress);
    uint8_t findModule_ipAdress(String ipAdress);
    void updateModule(uint8_t moduleID, String macAdress, String ipAdress, uint8_t shape);


    public:
    ModuleManager();
    uint8_t addNewModule(String macAdress, String ipAdress, uint8_t shape);
};



#endif