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

    Serial.print("Columns");    
        for(uint8_t j = 0; j < TEMP_PUZZLEGRIDSIZE; j++)
        {
            Serial.print(":");
            if(j<10) Serial.print(" ");
            Serial.print(j);
        }
        Serial.println();


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

                case PUZZLEPIECE_TYPE_PIPE_FORWARDBACKWARD:
                Serial.print("||");
                break;

                case PUZZLEPIECE_TYPE_PIPE_LEFTRIGHT:
                Serial.print("==");
                break;

                case PUZZLEPIECE_TYPE_PIPE_UPDOWN:
                Serial.print("PZ");
                break;

                default:
                Serial.print("??");
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
void ModuleManager::tryFitPuzzlePiece(ConnectedModule *newConnectedModule)
{
    Serial.print("Trying to fit puzzle piece ");
    Serial.println(newConnectedModule->getModuleID());

    //Implementation:
    // 1. Find the piece on the board that the new module should connect to
    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        
        if (connectedModules[i]->getModuleID() == newConnectedModule->getModuleID()) continue; //Skip the module that is being placed (Self)
        if(!connectedModules[i]->getPuzzlePlaced()) continue; //Skip modules that aren't already placed on the board
        
        {
            //Find a module that has a neighbor that is the module that is being placed
            uint8_t originalPieceConnectorDirection = connectedModules[i]->checkHasNeighbor(newConnectedModule->getModuleID());
            if(originalPieceConnectorDirection != DIRECTION_NONE)
            {
                //Find the side of the old module on which the new module should be placed
                CompassConnector originalPieceConnectorData = connectedModules[i]->getConnectorInfo(originalPieceConnectorDirection); //Conains a rotation adjusted direction to which the new module should be placed
               
                //Check if new and old modules have discovered each other on one of their sides. (If this isn't the case, the modules dont know where to connect to each other)
                if(newConnectedModule->checkHasNeighbor(connectedModules[i]->getModuleID()) == DIRECTION_NONE)
                {
                    Serial.println("Error: New module does not have the old module as a neighbor");
                    return;
                }
                if (connectedModules[i]->checkHasNeighbor(newConnectedModule->getModuleID()) == DIRECTION_NONE)
                {
                    Serial.println("Error: Old module does not have the new module as a neighbor");
                    return;
                }

                //Find the location of the old module on the board
                Serial.println("Calculating X and Y of the old piece");
                uint8_t originalPieceRow = 0;
                uint8_t originalPieceColumn = 0;
                for (uint8_t k = 0; k < TEMP_PUZZLEGRIDSIZE; k++)
                {
                    for (uint8_t j = 0; j < TEMP_PUZZLEGRIDSIZE; j++)
                    {
                        if (puzzlePieces[k][j].parentModule != NULL)
                        {
                            Serial.print("Checking piece at [");
                            Serial.print(k);
                            Serial.print("][");
                            Serial.print(j);
                            Serial.print("] ID: ");
                            Serial.print(puzzlePieces[k][j].parentModule->getModuleID());
                            Serial.print(" - Looking for ID: ");
                            Serial.println(connectedModules[i]->getModuleID());


                            if (puzzlePieces[k][j].parentModule->getModuleID() == connectedModules[i]->getModuleID())
                            {
                                Serial.print("Found piece with ID match at [");
                                Serial.print(k);
                                Serial.print("][");
                                Serial.print(j);
                                Serial.println("]");
                                if (puzzlePieces[k][j].pieceType == PUZZLEPIECE_TYPE_HEART)
                                {
                                    originalPieceRow = k;
                                    originalPieceColumn = j;
                                    Serial.print("Found old piece at [");
                                    Serial.print(originalPieceRow);
                                    Serial.print("][");
                                    Serial.print(originalPieceColumn);
                                    Serial.println("]");
                                }
                                else
                                {
                                    Serial.println("Error: Found piece with pointer match, but it is not a heart piece");
                                }
                            }
                            Serial.println("Did not find match");
                        }
                    }
                }

                Serial.println("Rotating Module");
                //Rotate the new module to the correct orientation so it can connect to the existing piece
                switch (originalPieceConnectorData.rotationCompensatedDirection)
                {
                    case DIRECTION_NORTH:
                    //The new module should be placed on the north side of the old module
                    Serial.println("The new module should be placed on the North side of the old module");

                    //Check the rotation needed on the new module
                    switch (newConnectedModule->checkHasNeighbor(connectedModules[i]->getModuleID()))//Returns the direction to which the old module is relative to the new module.
                    {
                        case DIRECTION_NORTH:
                        //The new module is placed on the north side of the old module.
                        //The old module is placed on the north side of the new module.
                        //The new module should be rotated 180 degrees. (opposite direction)
                        Serial.println("The old module is placed on the North side of the new module");
                        newConnectedModule->rotate(ROTATION_180Deg);
                        break;

                        case DIRECTION_EAST:
                        //The new module is placed on the north side of the old module
                        //The old module is placed on the east side of the new module.
                        //The new module should be rotated 90 degrees clockwise
                        Serial.println("The old module is placed on the East side of the new module");
                        newConnectedModule->rotate(ROTATION_90Deg);
                        break;

                        case DIRECTION_SOUTH:
                        //The new module is placed on the north side of the old module
                        //The old module is placed on the south side of the new module.
                        //The new module does not need rotation (Already aligned)
                        Serial.println("The old module is placed on the South side of the new module");
                        newConnectedModule->rotate(ROTATION_0Deg);
                        break;

                        case DIRECTION_WEST:
                        //The new module is placed on the north side of the old module
                        //The old module is placed on the west side of the new module.
                        //The new module should be rotated 90 degrees counterclockwise
                        Serial.println("The old module is placed on the West side of the new module");
                        newConnectedModule->rotate(ROTATION_270Deg);
                        break;
                    }
                    break;
                    
                    case DIRECTION_EAST:
                    {
                        //The new module should be placed on the east side of the old module
                        Serial.println("The new module should be placed on the East side of the old module");

                        //Check the rotation needed on the new module
                        switch (newConnectedModule->checkHasNeighbor(connectedModules[i]->getModuleID()))
                        {
                            case DIRECTION_NORTH:
                            //The new module is placed on the east side of the old module.
                            //The old module is placed on the north side of the new module.
                            //The new module should be rotated 90 degrees counterclockwise
                            Serial.println("The old module is placed on the North side of the new module");
                            newConnectedModule->rotate(ROTATION_270Deg);
                            break;

                            case DIRECTION_EAST:
                            //The new module is placed on the east side of the old module.
                            //The old module is placed on the east side of the new module.
                            //The new module should be rotated 180 degrees. (opposite direction)
                            Serial.println("The old module is placed on the East side of the new module");
                            newConnectedModule->rotate(ROTATION_180Deg);
                            break;

                            case DIRECTION_SOUTH:
                            //The new module is placed on the east side of the old module.
                            //The old module is placed on the south side of the new module.
                            //The new module should be rotated 90 degrees clockwise
                            Serial.println("The old module is placed on the South side of the new module");
                            newConnectedModule->rotate(ROTATION_90Deg);
                            break;

                            case DIRECTION_WEST:
                            //The new module is placed on the east side of the old module.
                            //The old module is placed on the west side of the new module.
                            //The new module does not need rotation (Already aligned)
                            Serial.println("The old module is placed on the West side of the new module");
                            newConnectedModule->rotate(ROTATION_0Deg);
                            break;
                        }
                    }
                    break;

                    case DIRECTION_SOUTH:
                    {
                        //The new module should be placed on the south side of the old module
                        Serial.println("The new module should be placed on the South side of the old module");

                        //Check the rotation needed on the new module
                        switch (newConnectedModule->checkHasNeighbor(connectedModules[i]->getModuleID()))
                        {
                            case DIRECTION_NORTH:
                            //The new module is placed on the south side of the old module.
                            //The old module is placed on the north side of the new module.
                            //The new module does not need rotation (Already aligned)
                            Serial.println("The old module is placed on the North side of the new module");
                            newConnectedModule->rotate(ROTATION_0Deg);
                            break;

                            case DIRECTION_EAST:
                            //The new module is placed on the south side of the old module.
                            //The old module is placed on the east side of the new module.
                            //The new module should be rotated 90 degrees counterclockwise
                            Serial.println("The old module is placed on the East side of the new module");
                            newConnectedModule->rotate(ROTATION_270Deg);
                            break;

                            case DIRECTION_SOUTH:
                            //The new module is placed on the south side of the old module.
                            //The old module is placed on the south side of the new module.
                            //The new module should be rotated 180 degrees. (opposite direction)
                            Serial.println("The old module is placed on the South side of the new module");
                            newConnectedModule->rotate(ROTATION_180Deg);
                            break;

                            case DIRECTION_WEST:
                            //The new module is placed on the south side of the old module.
                            //The old module is placed on the west side of the new module.
                            //The new module should be rotated 90 degrees clockwise
                            Serial.println("The old module is placed on the West side of the new module");
                            newConnectedModule->rotate(ROTATION_90Deg);
                            break;
                        }
                    }
                    break;

                    case DIRECTION_WEST:
                    {
                        //The new module should be placed on the west side of the old module
                        Serial.println("The new module should be placed od the West side of the old module");

                        //Check the rotation needed on the new module
                        switch (newConnectedModule->checkHasNeighbor(connectedModules[i]->getModuleID()))
                        {
                            case DIRECTION_NORTH:
                            //The new module is placed on the west side of the old module.
                            //The old module is placed on the north side of the new module.
                            //The new module should be rotated 90 degrees clockwise
                            Serial.println("The old module is placed on the North side of the new module");
                            newConnectedModule->rotate(ROTATION_90Deg);
                            break;

                            case DIRECTION_EAST:
                            //The new module is placed on the west side of the old module.
                            //The old module is placed on the east side of the new module.
                            //The new module does not need rotation (Already aligned)
                            Serial.println("The old module is placed on the East side of the new module");
                            newConnectedModule->rotate(ROTATION_0Deg);
                            break;

                            case DIRECTION_SOUTH:
                            //The new module is placed on the west side of the old module.
                            //The old module is placed on the south side of the new module.
                            //The new module should be rotated 90 degrees counterclockwise
                            Serial.println("The old module is placed on the South side of the new module");
                            newConnectedModule->rotate(ROTATION_270Deg);
                            break;

                            case DIRECTION_WEST:
                            //The new module is placed on the west side of the old module.
                            //The old module is placed on the west side of the new module.
                            //The new module should be rotated 180 degrees. (opposite direction)
                            Serial.println("The old module is placed on the West side of the new module");
                            newConnectedModule->rotate(ROTATION_180Deg);
                            break;
                        }
                    }
                    break;
                }

                Serial.println("Calculating X and Y of the new piece. Orignal piece coords: ");
                Serial.print(originalPieceRow);
                Serial.print(", ");
                Serial.println(originalPieceColumn);
                //Calculate the location where the new heart piece should be placed on the board
                uint8_t firstFreeX = 0;
                uint8_t firstFreeY = 0;
                switch (originalPieceConnectorData.rotationCompensatedDirection)
                {
                    case DIRECTION_NORTH: //If the new module should be placed on the north side of the old module
                    {
                        Serial.println("Placing on north side of old piece");
                        firstFreeX = originalPieceRow - 1;
                        firstFreeY = originalPieceColumn;
                        //add pipe length on the south side of the new module
                        CompassConnector newPieceConnectorData = newConnectedModule->getConnectorInfo_RotationAdjusted(DIRECTION_SOUTH);
                        firstFreeX -= newPieceConnectorData.basePipe; //If the new module has a pipe on the south side
                        Serial.print("Resulting coordinates: [");
                        Serial.print(firstFreeX);
                        Serial.print("][");
                        Serial.print(firstFreeY);
                        Serial.println("]");
                    }
                    break;
                    case DIRECTION_EAST: //If the new module should be placed on the east side of the old module
                    {
                        Serial.println("Placing on east side of old piece");
                        firstFreeX = originalPieceRow;
                        firstFreeY = originalPieceColumn + 1;
                        CompassConnector newPieceConnectorData = newConnectedModule->getConnectorInfo_RotationAdjusted(DIRECTION_WEST);
                        firstFreeY += newPieceConnectorData.basePipe; //If the new module has a pipe on the west side
                    }
                    break;
                    case DIRECTION_SOUTH: //If the new module should be placed on the south side of the old module
                    {
                        Serial.println("Placing on south side of old piece");
                        firstFreeX = originalPieceRow + 1;
                        firstFreeY = originalPieceColumn;
                        CompassConnector newPieceConnectorData = newConnectedModule->getConnectorInfo_RotationAdjusted(DIRECTION_NORTH);
                        firstFreeX += newPieceConnectorData.basePipe; //If the new module has a pipe on the north side
                    }
                    break;
                    case DIRECTION_WEST: //If the new module should be placed on the west side of the old module
                    {
                        Serial.println("Placing on west side of old piece");
                        firstFreeX = originalPieceRow;
                        firstFreeY = originalPieceColumn - 1;
                        CompassConnector newPieceConnectorData = newConnectedModule->getConnectorInfo_RotationAdjusted(DIRECTION_EAST);
                        firstFreeY -= newPieceConnectorData.basePipe; //If the new module has a pipe on the east side
                    }
                    break;
                }
                Serial.print("Coordinates to place new piece: [");
                Serial.print(firstFreeX);
                Serial.print("][");
                Serial.print(firstFreeY);
                Serial.println("]");
                placePuzzlePiece(newConnectedModule, firstFreeX, firstFreeY);
                return;
            }
        }
    }
}

void ModuleManager::placePuzzlePiece(ConnectedModule *newConnectedModule, uint8_t firstFreeX, uint8_t firstFreeY)
{
    Serial.print("Placing puzzle piece ");
    Serial.println(newConnectedModule->getModuleID());

    //Place the new heart piece on the board
                puzzlePieces[firstFreeX][firstFreeY].parentModule = newConnectedModule;
                puzzlePieces[firstFreeX][firstFreeY].pieceType = PUZZLEPIECE_TYPE_HEART;
                editPuzzleGridPart(firstFreeX, firstFreeY, newConnectedModule, PUZZLEPIECE_TYPE_HEART);

                //Place pipes from heart piece on the board
                for (uint8_t i = 1; i <= DIRECTIONS; i++)
                {
                    
                    CompassConnector newPieceConnectorData = newConnectedModule->getConnectorInfo_RotationAdjusted(i);
                    Serial.print("Checking direction ");
                    Serial.print(i);
                    Serial.print(" - has pipe with length ");
                    Serial.println(newPieceConnectorData.basePipe);

                        for (uint8_t j = 1; j <= newPieceConnectorData.basePipe; j++)
                        {
                            Serial.print("Placing pipe on the board ");
                            Serial.println(j);
                            
                            switch (i)
                            {
                                case DIRECTION_NORTH:
                                editPuzzleGridPart(firstFreeX - j, firstFreeY, newConnectedModule, PUZZLEPIECE_TYPE_PIPE_FORWARDBACKWARD);
                                break;

                                case DIRECTION_EAST:
                                editPuzzleGridPart(firstFreeX, firstFreeY + j, newConnectedModule, PUZZLEPIECE_TYPE_PIPE_LEFTRIGHT);
                                break;

                                case DIRECTION_SOUTH:
                                editPuzzleGridPart(firstFreeX + j, firstFreeY, newConnectedModule, PUZZLEPIECE_TYPE_PIPE_FORWARDBACKWARD);
                                break;

                                case DIRECTION_WEST:
                                editPuzzleGridPart(firstFreeX, firstFreeY - j, newConnectedModule, PUZZLEPIECE_TYPE_PIPE_LEFTRIGHT);
                                break;
                            }
                        }
                        
            
                }
                newConnectedModule->setPuzzlePlaced(true);
                Serial.println("Puzzle piece placed");
}

void ModuleManager::editPuzzleGridPart(uint8_t x, uint8_t y, ConnectedModule *parentModule, uint8_t pieceType)
{
    Serial.print("Editing puzzle grid part [");
    Serial.print(x);
    Serial.print("][");
    Serial.print(y);
    Serial.print("] Type: ");
    Serial.println(pieceType);

    if (x < 0 || x >= TEMP_PUZZLEGRIDSIZE || y < 0 || y >= TEMP_PUZZLEGRIDSIZE)
    {
        Serial.println("Error: Out of bounds");
        return;
    }

    puzzlePieces[x][y].parentModule = parentModule;
    puzzlePieces[x][y].pieceType = pieceType;
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

    if (boardIsEmpty)
    {
        if (connectedModules.size() > 0)
        {
            placePuzzlePiece(connectedModules[0], TEMP_PUZZLEGRIDSIZE/2, TEMP_PUZZLEGRIDSIZE/2);
            boardIsEmpty = false;
        }
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