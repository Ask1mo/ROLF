#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

#include <vector>
#include "connectedModule/connectedModule.h"
#include "setup.h" 

#define ADDR_NONE 0

#define TEMP_PUZZLEGRIDSIZE 20

#define PUZZLEPIECE_TYPE_EMPTY  0
#define PUZZLEPIECE_TYPE_HEART  1
#define PUZZLEPIECE_TYPE_PIPE   2

#define INTERVAL_DRAWPUZZLE 5000

#define ORIENTATION_HORIZONTAL 0
#define ORIENTATION_VERTICAL   1
#define DIRECTION_FORWARD      0
#define DIRECTION_BACKWARD     1

struct PuzzlePiece
{
    ConnectedModule *parentModule;
    uint8_t pieceType;
};


class ModuleManager
{
    private:
    std::vector<ConnectedModule*> connectedModules;
    PuzzlePiece puzzlePieces[TEMP_PUZZLEGRIDSIZE][TEMP_PUZZLEGRIDSIZE];
    uint64_t lastMillis_PuzzleDraw;
    bool boardIsEmpty;
    


    bool    checkModuleID(uint8_t moduleID);
    uint8_t getModuleID_macAdress(String macAdress);
    uint8_t findModule_ipAdress(String ipAdress);
    bool    updateModule(uint8_t moduleID, String macAdress, String ipAdress);
    ConnectedModule *getModule(uint8_t moduleID);
    void printPuzzleGrid();
    void tryFitPuzzlePiece(ConnectedModule *connectedModule);
    void tryFitPuzzlePiece_Pipes(uint8_t heartX, uint8_t heartY, ConnectedModule *connectedModule, uint8_t rotation);
    bool tryFitPuzzlePiece_PipeSingle(uint8_t heartX, uint8_t heartY, ConnectedModule *connectedModule, uint8_t pipeLength, bool horizontalOrVertical, bool forwardOrBackward);
    bool addPieceToPuzzleGrid(uint8_t x, uint8_t y, ConnectedModule *connectedModule, uint8_t pieceType);

    public:
    ModuleManager();
    void tick();
    uint8_t addNewModule(String macAdress, String ipAdress, BaseInfo baseInfo);
    void updateModuleConnection(uint8_t moduleID, uint8_t direction, uint8_t neighborID, uint8_t neighborDirection);
};



#endif