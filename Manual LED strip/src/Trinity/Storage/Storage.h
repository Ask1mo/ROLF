#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include "Trinity/LedManager/ledManager.h"

#define STORAGESLOTS 5

class Storage
{
private:

public:
  Storage();
  void store(uint8_t slot, LedManager *ledManager);
};


#endif