#include "Storage.h"

Storage::Storage()
{
  Serial.println(F("Storage Starting..."));

  Serial.println(F("...Storage Started"));
}

void Storage::store(uint8_t slot, LedManager *LedManager)
{
  if(slot >= STORAGESLOTS)
  {
    Serial.println("SELECTED SLOT IS TOO HIGH");
    return;
  }
  
}