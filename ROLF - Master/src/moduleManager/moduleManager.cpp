#include "moduleManager.h"

//NOTE: ADRESS_NONE is 0, AND MAY NEVER BE USED AS A MODULE ID
//NOTE: MINIMISE MODULEID CHANGING IF SAME MAC ADRESS IS FOUND. UPDATE MODULE INSTEAD (IP ADRESS)

//Constructor
ModuleManager::ModuleManager()
{
    Serial.println("ModuleManager created");
}

//Private
bool ModuleManager::checkModuleID(uint8_t moduleID)
{
    for (int i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getModuleID() == moduleID)
        {
            return true;
        }
    }
    return false;
}
uint8_t ModuleManager::findModule_macAdress(String macAdress)
{
    for (int i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getMacAdress() == macAdress)
        {
            return connectedModules[i]->getModuleID();
        }
    }
}
uint8_t ModuleManager::findModule_ipAdress(String ipAdress)
{
    for (int i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getIpAdress() == ipAdress)
        {
            return connectedModules[i]->getModuleID();
        }
    }
}
bool ModuleManager::updateModule_(uint8_t moduleID, String macAdress, String ipAdress, uint8_t shape)
{
    if (!checkModuleID(moduleID))
    {
        Serial.println("Module not found");
        return false;
    }


    for (int i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getModuleID() == moduleID)
        {
            connectedModules[i].updateIpAdress(ipAdress);
            connectedModules[i].updateMacAdress(macAdress);
            connectedModules[i].updateShape(shape);

            Serial.print("Module replaced with ID: ");
            Serial.println(moduleID);
            Serial.print("MAC address: ");
            Serial.println(macAdress);
            Serial.print("Shape: ");
            Serial.println(shape);
        }
    }
}

//Public
uint8_t ModuleManager::addNewModule(String macAdress, String ipAdress, uint8_t shape)
{
    // Check if MAC adress has already been logged, In which case: Look up existing module with that MAC adress and update it's IP adress
    if (findModule_macAdress(macAdress) != ADDR_NONE)
    {
        Serial.println("MAC adress already logged");
         
        return;
    }

    // Check if IP adress has already been logged
    for (int i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getIpAdress() == ipAdress)
        {
            Serial.println("IP adress already logged");
            connectedModules.;
            return addNewModule(macAdress, ipAdress, shape);
        }

        // Add new module, assign new ID
        uint8_t newModuleID = 0;
        for (int i = 0; i < sizeof(uint8_t); i++)
        {
            bool idExists = false;
            newModuleID++;
            for (int i = 0; i < connectedModules.size(); i++)
            {
                if (connectedModules[i]->getModuleID() == newModuleID) idExists = true;
            }
            if (!idExists)
            {
                break;
            }
        }

        uint16_t newModuleID = connectedModules.size(); //
        connectedModules.push_back(new ConnectedModule(macAdress, ipAdress, newModuleID, shape));
        Serial.print("New module added with ID: ");
        Serial.println(newModuleID);
        Serial.print("MAC address: ");
        Serial.println(macAdress);
        Serial.print("Shape: ");
        Serial.println(shape);

        return newModuleID;
    }
}
