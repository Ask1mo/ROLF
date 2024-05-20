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
void ModuleManager::tryFitPuzzlePiece(ConnectedModule *newConnectedModule)
{
    Serial.print("Trying to fit puzzle piece ");
    Serial.println(newConnectedModule->getModuleID());

    //Implementation:
    // 1. Find the piece on the board that the new module should connect to
    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        //Skip the module that is being placed (Self)
        if (connectedModules[i]->getModuleID() == newConnectedModule->getModuleID()) continue; 

        //Find a module that has a neighbor that is the module that is being placed
        if(connectedModules[i]->getPuzzlePlaced())
        {
            uint8_t originalPieceConnectorDirection = connectedModules[i]->checkHasNeighbor(newConnectedModule->getModuleID());
            if(originalPieceConnectorDirection != DIRECTION_NONE)
            {
                //Find the side of the old module on which the new module should be placed (In compassconnector data) Get a copy of said data
                //Note: pipe length is not needed for this operation, since you should check the grid for the pipes that are already placed
                CompassConnector originalPieceConnectorData =  connectedModules[i]->getConnectorInfo(originalPieceConnectorDirection); //Conains a rotation adjusted direction to which the new module should be placed

                //We now have the relevant data from the original piece (The rotation adjusted side on the original piece on which the new piece should be placed)

                //Rotate the new module to the correct orientation so it can connect to the existing piece
                switch (originalPieceConnectorData.rotationCompensatedDirection)
                {
                    case DIRECTION_NORTH:
                    //The new module should be placed on the north side of the old module

                    //Check the rotation needed on the new module
                    switch (newConnectedModule->checkHasNeighbor(connectedModules[i]->getModuleID()))//Returns the direction to which the old module is relative to the new module.
                    {
                        case DIRECTION_NORTH:
                        //The new module is placed on the north side of the old module.
                        //The old module is placed on the north side of the new module.
                        //The new module should be rotated 180 degrees. (opposite direction)
                        newConnectedModule->rotate(ROTATION_180Deg);
                        break;

                        case DIRECTION_EAST:
                        //The new module is placed on the north side of the old module
                        //The old module is placed on the east side of the new module.
                        //The new module should be rotated 90 degrees clockwise
                        newConnectedModule->rotate(ROTATION_90Deg);
                        break;

                        case DIRECTION_SOUTH:
                        //The new module is placed on the north side of the old module
                        //The old module is placed on the south side of the new module.
                        //The new module does not need rotation (Already aligned)
                        newConnectedModule->rotate(ROTATION_0Deg);
                        break;

                        case DIRECTION_WEST:
                        //The new module is placed on the north side of the old module
                        //The old module is placed on the west side of the new module.
                        //The new module should be rotated 90 degrees counterclockwise
                        newConnectedModule->rotate(ROTATION_270Deg);
                        break;
                    }
                    break;
                    
                    case DIRECTION_EAST:
                    {
                        //The new module should be placed on the east side of the old module

                        //Check the rotation needed on the new module
                        switch (newConnectedModule->checkHasNeighbor(connectedModules[i]->getModuleID()))
                        {
                            case DIRECTION_NORTH:
                            //The new module is placed on the east side of the old module.
                            //The old module is placed on the north side of the new module.
                            //The new module should be rotated 90 degrees counterclockwise
                            newConnectedModule->rotate(ROTATION_270Deg);
                            break;

                            case DIRECTION_EAST:
                            //The new module is placed on the east side of the old module.
                            //The old module is placed on the east side of the new module.
                            //The new module should be rotated 180 degrees. (opposite direction)
                            newConnectedModule->rotate(ROTATION_180Deg);
                            break;

                            case DIRECTION_SOUTH:
                            //The new module is placed on the east side of the old module.
                            //The old module is placed on the south side of the new module.
                            //The new module should be rotated 90 degrees clockwise
                            newConnectedModule->rotate(ROTATION_90Deg);
                            break;

                            case DIRECTION_WEST:
                            //The new module is placed on the east side of the old module.
                            //The old module is placed on the west side of the new module.
                            //The new module does not need rotation (Already aligned)
                            newConnectedModule->rotate(ROTATION_0Deg);
                            break;
                        }
                    }
                    break;

                    case DIRECTION_SOUTH:
                    {
                        //The new module should be placed on the south side of the old module

                        //Check the rotation needed on the new module
                        switch (newConnectedModule->checkHasNeighbor(connectedModules[i]->getModuleID()))
                        {
                            case DIRECTION_NORTH:
                            //The new module is placed on the south side of the old module.
                            //The old module is placed on the north side of the new module.
                            //The new module does not need rotation (Already aligned)
                            newConnectedModule->rotate(ROTATION_0Deg);
                            break;

                            case DIRECTION_EAST:
                            //The new module is placed on the south side of the old module.
                            //The old module is placed on the east side of the new module.
                            //The new module should be rotated 90 degrees counterclockwise
                            newConnectedModule->rotate(ROTATION_270Deg);
                            break;

                            case DIRECTION_SOUTH:
                            //The new module is placed on the south side of the old module.
                            //The old module is placed on the south side of the new module.
                            //The new module should be rotated 180 degrees. (opposite direction)
                            newConnectedModule->rotate(ROTATION_180Deg);
                            break;

                            case DIRECTION_WEST:
                            //The new module is placed on the south side of the old module.
                            //The old module is placed on the west side of the new module.
                            //The new module should be rotated 90 degrees clockwise
                            newConnectedModule->rotate(ROTATION_90Deg);
                            break;
                        }
                    }
                    break;

                    case DIRECTION_WEST:
                    {
                        //The new module should be placed on the west side of the old module

                        //Check the rotation needed on the new module
                        switch (newConnectedModule->checkHasNeighbor(connectedModules[i]->getModuleID()))
                        {
                            case DIRECTION_NORTH:
                            //The new module is placed on the west side of the old module.
                            //The old module is placed on the north side of the new module.
                            //The new module should be rotated 90 degrees clockwise
                            newConnectedModule->rotate(ROTATION_90Deg);
                            break;

                            case DIRECTION_EAST:
                            //The new module is placed on the west side of the old module.
                            //The old module is placed on the east side of the new module.
                            //The new module does not need rotation (Already aligned)
                            newConnectedModule->rotate(ROTATION_0Deg);
                            break;

                            case DIRECTION_SOUTH:
                            //The new module is placed on the west side of the old module.
                            //The old module is placed on the south side of the new module.
                            //The new module should be rotated 90 degrees counterclockwise
                            newConnectedModule->rotate(ROTATION_270Deg);
                            break;

                            case DIRECTION_WEST:
                            //The new module is placed on the west side of the old module.
                            //The old module is placed on the west side of the new module.
                            //The new module should be rotated 180 degrees. (opposite direction)
                            newConnectedModule->rotate(ROTATION_180Deg);
                            break;
                        }
                    }
                    break;
                }
            

                //Find the location of the old module on the board
                uint8_t originalPieceRow = 0;
                uint8_t originalPieceColumn = 0;
                for (uint8_t i = 0; i < TEMP_PUZZLEGRIDSIZE; i++)
                {
                    for (uint8_t j = 0; j < TEMP_PUZZLEGRIDSIZE; j++)
                    {
                        if (puzzlePieces[i][j].parentModule == connectedModules[i] && puzzlePieces[i][j].pieceType == PUZZLEPIECE_TYPE_HEART)
                        {
                            originalPieceRow = i;
                            originalPieceColumn = j;
                        }
                    }
                }


                //Place new module on the board
                uint8_t firstFreeX = 0;
                uint8_t firstFreeY = 0;

                switch (originalPieceConnectorData.rotationCompensatedDirection)
                {
                    case DIRECTION_NORTH: //If the new module should be placed on the north side of the old module
                    {
                        firstFreeX = originalPieceRow - 1;
                        firstFreeY = originalPieceColumn;
                        //add pipe length on the south side of the new module
                        CompassConnector newPieceConnectorData = newConnectedModule->getConnectorInfo_RotationAdjusted(DIRECTION_SOUTH);
                        if (newPieceConnectorData.basePipe =! BASE_PIPE_ENDCAP)firstFreeX += newPieceConnectorData.basePipe; //If the new module has a pipe on the south side
                    }
                    break;
                    case DIRECTION_EAST: //If the new module should be placed on the east side of the old module
                    {
                        firstFreeX = originalPieceRow;
                        firstFreeY = originalPieceColumn + 1;
                        CompassConnector newPieceConnectorData = newConnectedModule->getConnectorInfo_RotationAdjusted(DIRECTION_WEST);
                        if (newPieceConnectorData.basePipe =! BASE_PIPE_ENDCAP)firstFreeY += newPieceConnectorData.basePipe; //If the new module has a pipe on the west side
                    }
                    break;
                    case DIRECTION_SOUTH: //If the new module should be placed on the south side of the old module
                    {
                        firstFreeX = originalPieceRow + 1;
                        firstFreeY = originalPieceColumn;
                        CompassConnector newPieceConnectorData = newConnectedModule->getConnectorInfo_RotationAdjusted(DIRECTION_NORTH);
                        if (newPieceConnectorData.basePipe =! BASE_PIPE_ENDCAP)firstFreeX += newPieceConnectorData.basePipe; //If the new module has a pipe on the north side
                    }
                    break;
                    case DIRECTION_WEST: //If the new module should be placed on the west side of the old module
                    {
                        firstFreeX = originalPieceRow;
                        firstFreeY = originalPieceColumn - 1;
                        CompassConnector newPieceConnectorData = newConnectedModule->getConnectorInfo_RotationAdjusted(DIRECTION_EAST);
                        if (newPieceConnectorData.basePipe =! BASE_PIPE_ENDCAP)firstFreeY += newPieceConnectorData.basePipe; //If the new module has a pipe on the east side
                    }
                    break;
                }

                //We now have all the data to place the heart piece on the board
                puzzlePieces[firstFreeX][firstFreeY].parentModule = newConnectedModule;
                puzzlePieces[firstFreeX][firstFreeY].pieceType = PUZZLEPIECE_TYPE_HEART;

                //Now place the pipes on the board from the new module
                for (uint8_t i = 0; i < DIRECTIONS; i++)
                {
                    CompassConnector newPieceConnectorData = newConnectedModule->getConnectorInfo_RotationAdjusted(i);
                    if (newPieceConnectorData.neighborAdress != ADDR_NONE)
                    {
                        for (uint8_t i = 0; i < newPieceConnectorData.basePipe; i++)
                        {
                            switch (i)
                            {
                                case DIRECTION_NORTH:
                                puzzlePieces[firstFreeX - i][firstFreeY].pieceType = BASE_PIPE_FORWARDBACKWARD;
                                break;

                                case DIRECTION_EAST:
                                puzzlePieces[firstFreeX][firstFreeY + i].pieceType = BASE_PIPE_LEFTRIGHT;
                                break;

                                case DIRECTION_SOUTH:
                                puzzlePieces[firstFreeX + i][firstFreeY].pieceType = BASE_PIPE_FORWARDBACKWARD;
                                break;

                                case DIRECTION_WEST:
                                puzzlePieces[firstFreeX][firstFreeY - i].pieceType = BASE_PIPE_LEFTRIGHT;
                                break;
                            }
                        }
                        
                    }
                }
            }
        }
    }




    

}

//Functions needed:



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