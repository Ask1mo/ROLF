#include "moduleManager.h"

//NOTE: ADRESS_NONE is 0, AND MAY NEVER BE USED AS A MODULE ID
//NOTE: MINIMISE MODULEID CHANGING IF SAME MAC ADRESS IS FOUND. UPDATE MODULE INSTEAD (IP ADRESS)
//MAX MODULES: 255 (This is okay, system will probably only have around 100 modules)





//Constructor
ModuleManager::ModuleManager()
{
    Serial.println("ModuleManager created");
    boardIsEmpty = true;

    //Initialize puzzlePieces
    for (uint8_t i = 0; i < TEMP_PUZZLEGRIDSIZE; i++)
    {
        for (uint8_t j = 0; j < TEMP_PUZZLEGRIDSIZE; j++)
        {
            puzzlePieces[i][j].parentModule = NULL;
            puzzlePieces[i][j].pieceType = PUZZLEPIECE_TYPE_EMPTY;
        }
    }
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
void ModuleManager::printPuzzleGrid()
{
    Serial.println();
    Serial.println("Printing grid:");
    for (uint8_t i = 0; i < TEMP_PUZZLEGRIDSIZE; i++)
    {
        Serial.print("Row ");
        Serial.print(i);
        if(i<10) Serial.print(" ");
        Serial.print(" [");

        uint8_t stupidID = 0;

        for (uint8_t j = 0; j < TEMP_PUZZLEGRIDSIZE; j++)
        {
            switch (puzzlePieces[i][j].pieceType)
            {
                case PUZZLEPIECE_TYPE_EMPTY:
                Serial.print("  ");
                break;

                case PUZZLEPIECE_TYPE_HEART:
                stupidID = puzzlePieces[i][j].parentModule->getModuleID();
                if(stupidID < 10) Serial.print(" ");
                Serial.print(stupidID);
                break;

                case PUZZLEPIECE_TYPE_PIPE:
                Serial.print("P");
                break;
            }
            Serial.print(":");
        }
        Serial.println(" ]");
        Serial.println();
    
        Serial.print("       ");    
        for(uint8_t j = 0; j < TEMP_PUZZLEGRIDSIZE; j++)
        {
            Serial.print(":..");
        }
        Serial.println();
        
    }
}
void ModuleManager::tryFitPuzzlePiece(ConnectedModule *connectedModule)
{
    //This function adds a connectedModule (heart piece) to the puzzle grid in the form of a Puzzle piece . It also adds the connectedModule's pipes to the puzzle grid as puzzle pieces.
    //Before a module can be added to the grid, The required rotation has to be calculated. (If Module 1's east pipe is connected to module 2's north pipe.  module 2 has to be rotated 90 degrees counter clockwise)
    //Before placing a module you need to "Scout" if the heart and all pipe pieces can be placed. If not, the module should not be placed.
    //Also, if the board is empty, the first piece should be placed in the middle of the board
    //If the board is not empty, the module should be placed in a way that it connects to the existing pieces on the board:
    // 1. Find the piece on the board that the new module should connect to
    // 2. Rotate the module to the correct orientation so it can connect to the existing piece
    // 3. Scout the board to see if the module can be placed (Take the led pipes that come out of the new and old heart pieces into account)
    // 4. If the module can be placed, place it on the board together with the led pipes that come out of the heart piece
    






    //Assuming piece is not already in grid. (Fix later)
    Serial.print("Trying to fit puzzle piece ");
    Serial.print(connectedModule->getModuleID());

    if (boardIsEmpty)
    {
        Serial.println("Board is empty, placing first piece in middle");
        puzzlePieces[TEMP_PUZZLEGRIDSIZE/2][TEMP_PUZZLEGRIDSIZE/2].parentModule = connectedModule;
        puzzlePieces[TEMP_PUZZLEGRIDSIZE/2][TEMP_PUZZLEGRIDSIZE/2].pieceType = PUZZLEPIECE_TYPE_HEART;
        connectedModule->setPuzzlePlaced(true);
        boardIsEmpty = false;
        tryFitPuzzlePiece_Pipes(TEMP_PUZZLEGRIDSIZE/2, TEMP_PUZZLEGRIDSIZE/2, connectedModule, 0);
        return;
    }

}
void ModuleManager::tryFitPuzzlePiece_Pipes(uint8_t heartX, uint8_t heartY, ConnectedModule *connectedModule, uint8_t rotation)
{
    Serial.println("Trying to fit pipes");
    BaseInfo baseInfo = connectedModule->getBaseInfo();

    for (uint8_t i = 0; i < DIRECTIONS; i++)
    {
        if(baseInfo.northPipe == ) Serial.println("North pipe found");
    }

    //North
    if (baseInfo.northPipe == BASE_PIPE_FORWARDBACKWARD)
    {
        if (!tryFitPuzzlePiece_PipeSingle(heartX, heartY - 1, connectedModule, 1, ORIENTATION_VERTICAL, DIRECTION_BACKWARD)) return;
    }
    else if (baseInfo.northPipe == BASE_PIPE_LEFTRIGHT)
    {
        if (!tryFitPuzzlePiece_PipeSingle(heartX, heartY - 1, connectedModule, 1, ORIENTATION_HORIZONTAL, DIRECTION_FORWARD)) return;
    }
    else if (baseInfo.northPipe == BASE_PIPE_UPDOWN)
    {
        if (!tryFitPuzzlePiece_PipeSingle(heartX, heartY - 1, connectedModule, 1, ORIENTATION_VERTICAL, DIRECTION_FORWARD)) return;
    }
    else if (baseInfo.northPipe == BASE_PIPE_ENDCAP)
    {
        if (!tryFitPuzzlePiece_PipeSingle(heartX, heartY - 1, connectedModule, 1, ORIENTATION_HORIZONTAL, DIRECTION_FORWARD)) return;
    }
    else if (baseInfo.northPipe


    if (!tryFitPuzzlePiece_PipeSingle(heartX, heartY - 1, connectedModule, 1, ORIENTATION_VERTICAL, DIRECTION_BACKWARD)) return;
    if (!tryFitPuzzlePiece_PipeSingle(heartX + 1, heartY, connectedModule, 1, ORIENTATION_HORIZONTAL, DIRECTION_FORWARD)) return;
    if (!tryFitPuzzlePiece_PipeSingle(heartX, heartY + 1, connectedModule, 1, ORIENTATION_VERTICAL, DIRECTION_FORWARD)) return;
    if (!tryFitPuzzlePiece_PipeSingle(heartX - 1, heartY, connectedModule, 1, ORIENTATION_HORIZONTAL, DIRECTION_BACKWARD)) return;
    if (!tryFitPuzzlePiece_PipeSingle(heartX, heartY, connectedModule, 1, ORIENTATION_VERTICAL, DIRECTION_FORWARD)) return;
    if (!tryFitPuzzlePiece_PipeSingle(heartX, heartY, connectedModule, 1, ORIENTATION_VERTICAL, DIRECTION_FORWARD)) return;
}
bool ModuleManager::tryFitPuzzlePiece_PipeSingle(uint8_t heartX, uint8_t heartY, ConnectedModule *connectedModule, uint8_t pipeLength, bool horizontalOrVertical, bool forwardOrBackward)
{
    for (uint8_t i = 0; i < pipeLength; i++)
    {
        if (horizontalOrVertical == ORIENTATION_HORIZONTAL)
        {
            if (forwardOrBackward == DIRECTION_FORWARD)
            {
                if (puzzlePieces[heartX + i][heartY].pieceType != PUZZLEPIECE_TYPE_EMPTY) return addPieceToPuzzleGrid(heartX + i, heartY, connectedModule, PUZZLEPIECE_TYPE_PIPE);
                
            }
            else //DIRECTION_BACKWARD
            {
                if (puzzlePieces[heartX - i][heartY].pieceType != PUZZLEPIECE_TYPE_EMPTY) return addPieceToPuzzleGrid(heartX - i, heartY, connectedModule, PUZZLEPIECE_TYPE_PIPE);
            }
        }
        else //ORIENTATION_VERTICAL
        {
            if (forwardOrBackward == DIRECTION_FORWARD)
            {
                if (puzzlePieces[heartX][heartY + i].pieceType != PUZZLEPIECE_TYPE_EMPTY) return addPieceToPuzzleGrid(heartX, heartY + i, connectedModule, PUZZLEPIECE_TYPE_PIPE);
            }
            else //DIRECTION_BACKWARD
            {
                if (puzzlePieces[heartX][heartY - i].pieceType != PUZZLEPIECE_TYPE_EMPTY) return addPieceToPuzzleGrid(heartX, heartY - i, connectedModule, PUZZLEPIECE_TYPE_PIPE);
            }
        }
    }
    return false;
}
bool ModuleManager::addPieceToPuzzleGrid (uint8_t x, uint8_t y, ConnectedModule *connectedModule, uint8_t pieceType)
{
    Serial.print("Adding piece to grid at ");
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
    //Check if x and y are within bounds (Upper and lower bounds)
    if (x >= TEMP_PUZZLEGRIDSIZE || x < 0 || y >= TEMP_PUZZLEGRIDSIZE || y <0) return false;


    if (puzzlePieces[x][y].pieceType != PUZZLEPIECE_TYPE_EMPTY) return false;
    puzzlePieces[x][y].parentModule = connectedModule;
    puzzlePieces[x][y].pieceType = pieceType;
    Serial.println("Piece added to grid");
    return true;
}

//Public
void ModuleManager::tick()
{
    //Draw puzzle grid every INTERVAL_DRAWPUZZLE
    if (millis() - lastMillis_PuzzleDraw > INTERVAL_DRAWPUZZLE)
    {
        lastMillis_PuzzleDraw = millis();
        printPuzzleGrid();
    }


    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        if(!connectedModules[i]->getPuzzlePlaced()) tryFitPuzzlePiece(connectedModules[i]);
    }
}
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
void ModuleManager::updateModuleConnection(uint8_t moduleID, uint8_t direction, uint8_t neighborID, uint8_t neighborDirection)
{
    ConnectedModule *module = getModule(moduleID);
    if (module == NULL)
    {
        Serial.println("Module not found");
        return;
    }
    module->updateConnection(direction, neighborID, neighborDirection);
}