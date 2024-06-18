#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

#include <vector>
#include "connectedModule/connectedModule.h"
#include "setup.h" 

#define ADDR_NONE 0

#define TEMP_PUZZLEGRIDSIZE 20

#define INTERVAL_MODULECHANGESCAN 10000
#define INTERVAL_PATHFINDDEMO 16000
#define INTERVAL_DRAWPUZZLE 5000

struct XYZ
{
    int x;
    int y;
    int z;
};
struct PuzzlePiece
{
    ConnectedModule *parentModule;
    uint8_t pieceType;
    uint8_t basePiece; //THe direction-based code of each piece type (Check pipePieces.h for more info)
    
};
struct PuzzleVisitInfo
{
    bool visited;
};


class ModuleManager
{
    private:
    std::vector<ConnectedModule*> connectedModules;
    std::vector<ModuleLedInfo_Output> bufferedTransmissions;
    PuzzlePiece     puzzlePieces[TEMP_PUZZLEGRIDSIZE][TEMP_PUZZLEGRIDSIZE];
    PuzzleVisitInfo puzzleVisitInfo[TEMP_PUZZLEGRIDSIZE][TEMP_PUZZLEGRIDSIZE];
    uint64_t lastMillis_PuzzleDraw;
    uint64_t lastMillis_PathFindDemo;
    bool boardIsEmpty;
    
    //Module management
    bool    getIsModuleAlreadyKnown(uint8_t moduleID);
    uint8_t getModuleIDfromMacAdress(String macAdress);
    bool    updateModule(uint8_t moduleID, String macAdress, String ipAdress);
    ConnectedModule *getModule(uint8_t moduleID);
    
    

    //Puzzling
    void tryFitPuzzlePiece(ConnectedModule *connectedModule);
    ConnectedModule *findOldConnectedModule(uint8_t newModuleID);
    void rotateNewModule(ConnectedModule *newConnectedModule, ConnectedModule *oldConnectedModule);
    void getOldModuleCoords(uint8_t oldModuleID, uint8_t *x, uint8_t *y);
    void calculateNewModuleCoords(ConnectedModule *newConnectedModule, ConnectedModule *oldConnectedModule, uint8_t *x, uint8_t *y);
    void placePuzzlePiece(ConnectedModule *newConnectedModule, uint8_t firstFreeX, uint8_t firstFreeY);
    void editPuzzleGridPart(uint8_t x, uint8_t y, ConnectedModule *parentModule, uint8_t pieceType, uint8_t basePiece);
    void printPuzzleGrid();


    //Pathfinding
    void tempPathfindingDemo();
    
    std::vector<XYZ> tracePath(XYZ start, XYZ end);
    bool isMovementAllowed(XYZ current, uint8_t direction);

    bool isModuleAHorn(uint8_t templateID);


    public:
    ModuleManager();
    void tick();
    uint8_t addNewModule(NewClientInfo newClientInfo);
    void updateModuleConnection(ModuleChangeInfo moduleChangeInfo);
    bool getLedTransmission(String *transmission, String *ipAdress);
    std::vector<uint8_t> getFreeHornIDs(uint8_t excludedHornID);
    bool tempPathfindingDemoV2(uint8_t startModuleID, uint8_t endModuleID);
};



#endif 