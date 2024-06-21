/*
#ifndef MASKMANAGERR_H
#define MASKMANAGERR_H

#include "LedManager/ledManager.h"
#include "setup.h"

#define MASKMODE_DISABLED             0
#define MASKMODE_ONETIME_FADEIN       1
#define MASKMODE_ONETIME_FADEOUT      2
#define MASKMODE_REPEATING_MUSIC      3
#define MASKMODE_REPEATING_EMERGENCY  4
#define MASKMODE_REPEATING_STROKE     5

class MaskManager
{
private:
  Panel                                   *canvasPanels[CANVASWIDTH][CANVASHEIGHT];
  uint8_t                                 maskMode;
  void    printDebugCanvas                ();
  

public:
  MaskManager                             (Panel **panels);                       
  void    tick                            ();
  void    print                           ();
  void    setMode                         (uint8_t maskMode);
};

#endif
*/