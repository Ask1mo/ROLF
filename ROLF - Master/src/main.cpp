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
    comms.transmit(MESSAGE_COCL_IDASSIGNMENT + String(moduleID) + sessionID, newClients[i].ipAdress);
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
void importHornTriggers()
{
  std::vector<uint8_t> hornIDs = comms.getTriggeredHornsIDs();
  //for (int i = 0; i < hornIDs.size(); i++) FOR LOOP REMOVED IN FAVOR OF ONLY USING THE FIRST HORN. ONLY FOR PROTOTPTYE
  if (hornIDs.size() > 0)
  {
    Serial.print("Horn triggered: ");
    Serial.print(hornIDs[0]);
    Serial.print(" Free horns: ");
    std::vector<uint8_t> freeHorns = moduleManager.getFreeHornIDs(hornIDs[0]);
    for (int j = 0; j < freeHorns.size(); j++)
    {
      Serial.print(freeHorns[j]);
      Serial.print(" ");
    }

    if (freeHorns.size() == 1)
    {
      if (moduleManager.tempPathfindingDemoV2(hornIDs[0], freeHorns[0]))
      {
        Serial.println("Path found");
      }
      else
      {
        Serial.println("Path not found");
      }
    }

    if (freeHorns.size() > 1)
    {
      if (moduleManager.tempPathfindingDemoV2(hornIDs[0], random(0, freeHorns.size() - 1)))
      {
        Serial.println("Path found");
      }
      else
      {
        Serial.println("Path not found");
      }
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("---===Setup started===---"));

  sessionID = (uint8_t)esp_random();
  comms.setSessionID(sessionID);
  Serial.println("Session ID: " + String(sessionID));
  //No setup needed. Everything is done in constructors
  comms.connect();
  Serial.println(F("---===Setup finished===---"));  
}
void loop()
{
  uint64_t currentMillis = millis();

  comms.tick();
  importNewClients();
  importModuleChanges();
  importHornTriggers();

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
