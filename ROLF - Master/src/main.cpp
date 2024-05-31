#include "main.h"

void importNewClients()
{
  std::vector<NewClientInfo> newClients = comms.getNewClientBuffer();
  for (int i = 0; i < newClients.size(); i++)
  {
    Serial.print("New client: ");
    Serial.print(newClients[i].macAdress);
    Serial.print(" with templateID: ");
    Serial.println(newClients[i].templateID);

    uint8_t moduleID = moduleManager.addNewModule(newClients[i]);
    if (moduleID == 0) Serial.println("New module could not be added");
    comms.transmit(MESSAGE_COCL_IDASSIGNMENT + String(moduleID) + comms.getSessionID(), newClients[i].ipAdress);
  }
}
void importModuleChanges()
{
  std::vector<ModuleChangeInfo> moduleChanges = comms.getModuleChangeBuffer();
  for (int i = 0; i < moduleChanges.size(); i++)
  {
    moduleManager.updateModuleConnection(moduleChanges[i]);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("---===Setup started===---"));
  //No setup needed. Everything is done in constructors
  Serial.println(F("---===Setup finished===---"));  
}
void loop()
{
  uint64_t currentMillis = millis();

  comms.tick();
  importNewClients();
  importModuleChanges();

  moduleManager.tick();
  String ledTransmission = MESSAGE_COCL_NEWEFFECT;
  String ipAdress;
  if (moduleManager.getLedTransmission(&ledTransmission, &ipAdress))
  {
    Serial.println("Sending LED transmission");
    comms.transmit(ledTransmission, ipAdress);
  }

  /*
  if (currentMillis - lastSystemScanMillis > INTERVAL_MODULECHANGESCAN)
  {
    lastSystemScanMillis = currentMillis;
    Serial.println("Scanning module changes... (Not Implemented)");
  }
  */
} 
