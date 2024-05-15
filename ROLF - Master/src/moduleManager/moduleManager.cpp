#include "moduleManager.h"

//NOTE: ADRESS_NONE is 0, AND MAY NEVER BE USED AS A MODULE ID
//NOTE: MINIMISE MODULEID CHANGING IF SAME MAC ADRESS IS FOUND. UPDATE MODULE INSTEAD (IP ADRESS)
//MAX MODULES: 255 (This is okay, system will probably only have around 100 modules)





//Constructor
ModuleManager::ModuleManager()
{
    Serial.println("ModuleManager created");
}

//Private
bool ModuleManager::checkModuleID(uint8_t moduleID)
{
    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getModuleID() == moduleID)
        {
            return true;
        }
    }
    return false;
}
uint8_t ModuleManager::getModuleID_macAdress(String macAdress)
{
    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getMacAdress() == macAdress)
        {
            return connectedModules[i]->getModuleID();
        }
    }
    return 0; //Return 0 if no module is found with the given mac adress
}
uint8_t ModuleManager::findModule_ipAdress(String ipAdress)
{
    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getIpAdress() == ipAdress)
        {
            return connectedModules[i]->getModuleID();
        }
    }
    return 0; //Return 0 if no module is found with the given ip adress
}
bool ModuleManager::updateModule(uint8_t moduleID, String macAdress, String ipAdress)
{
    ConnectedModule *connectedModule = getModule(moduleID);
    if(connectedModule == NULL) 
    {
        Serial.println("UpdateModule: Module not found");
        return false;
    }
    if (connectedModule->getMacAdress() != macAdress)
    {
        Serial.println("UpdateModule: MAC adress mismatch");
        return false;
    }

    connectedModule->setIpAdress(ipAdress);
    return true;
}
ConnectedModule *ModuleManager::getModule(uint8_t moduleID)
{
    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getModuleID() == moduleID)
        {
            return connectedModules[i];
        }
    }
    return NULL;
}

//Public
uint8_t ModuleManager::addNewModule(String macAdress, String ipAdress, BaseInfo baseInfo)
{
    // Check if MAC adress has already been logged, In which case: Look up existing module with that MAC adress and update it's IP adress
    // Also check if the IP adress is already logged. In which case: Remove existing module with that IP adress and add new module. (The device with the old IP adress is probably disconnected)
    // If no module is found with the given MAC adress or IP adress, add new module and assign new ID to new module

    Serial.print("Adding new module: ");
    Serial.print("MAC: ");
    Serial.print(macAdress);
    Serial.print(" IP: ");
    Serial.println(ipAdress);
    Serial.println("BaseInfo:");
    Serial.print("HeartPiece: ");
    Serial.println(baseInfo.heartPiece);
    Serial.print("NorthPipe: ");
    Serial.println(baseInfo.northPipe);
    Serial.print("EastPipe: ");
    Serial.println(baseInfo.eastPipe);
    Serial.print("SouthPipe: ");
    Serial.println(baseInfo.southPipe);
    Serial.print("WestPipe: ");
    Serial.println(baseInfo.westPipe);
    Serial.print("UpPipe: ");
    Serial.println(baseInfo.upPipe);
    Serial.print("DownPipe: ");
    Serial.println(baseInfo.downPipe);


    uint8_t moduleID = getModuleID_macAdress(macAdress);// stays 0 if no module is found

    //If moduleID is not 0, a module with the given MAC adress is found. If it is 0, no module is found with the given MAC adress, thus a new module should be added
    if (moduleID) 
    {
        Serial.println("MAC adress already logged, updating IP adress");
        if (!updateModule(moduleID, macAdress, ipAdress)) return 0; //If the module failed to update, return 0
    }
    

    // Check if IP adress has already been logged. In which case: Remove existing module with that IP adress and add new module. (The device with the old IP adress is probably disconnected)
    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getIpAdress() == ipAdress && connectedModules[i]->getMacAdress() != macAdress) //If the IP adress is found, but the MAC adress is different
        {
            Serial.println("IP adress duplicate found, removing old module with IP adress:");
            //Free up memory
            delete connectedModules[i];
            //Remove from vector
            connectedModules.erase(connectedModules.begin() + i);
        }
    }

    //If moduleID is not 0, a module with the given MAC adress is found and updated. If it is 0, no module is found with the given MAC adress, thus a new module should be created and added
    if (moduleID) return moduleID; 


    // Add new module, assign new ID: Get lowest available ID (This ID is not the size of the vector, because disconnected modules may have left gaps in the ID sequence)
    uint8_t newModuleID = 1;
    while (checkModuleID(newModuleID))
    {
        if (newModuleID == 255)
        {
            Serial.println("All module ID's are taken");
            return 0; //If all module ID's are taken, return 0
        }
        newModuleID++;
    }
    connectedModules.push_back(new ConnectedModule(macAdress, ipAdress, newModuleID, baseInfo));
    return newModuleID;
}
