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
/*
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

                case PUZZLEPIECE_TYPE_PIPE_:
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

    for (uint8_t i = 0; i < connectedModules.size(); i++)
        {
            connectedModules[i]->printConnectors();
        }
}
*/
void ModuleManager::printPuzzleGrid()
{
    Serial.println();
    Serial.println("Printing grid:");

    Serial.print("Columns");    
        for(uint8_t j = 0; j < TEMP_PUZZLEGRIDSIZE; j++)
        {
            Serial.print(" ");
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
                Serial.print("..");
                break;

                case PUZZLEPIECE_TYPE_HEART:
                stupidID = puzzlePieces[i][j].parentModule->getModuleID();
                if(stupidID < 10) Serial.print(" ");
                Serial.print(stupidID);
                break;

                case PUZZLEPIECE_TYPE_PIPE:
                switch (puzzlePieces[i][j].basePiece)
                {
                    case BASE_PIPE_FORWARDBACKWARD:
                    Serial.print("||");
                    break;
                    case BASE_PIPE_LEFTRIGHT:
                    Serial.print("==");
                    break;
                    case BASE_PIPE_UPDOWN:
                    Serial.print("()");
                    break;
                }
                break;

                default:
                Serial.print("??");
                break;
            }
            Serial.print(" ");
        }
        Serial.println(" ]");        
    }

    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        connectedModules[i]->printConnectors();
    }
}

void ModuleManager::tryFitPuzzlePiece(ConnectedModule *newConnectedModule)
{
    ConnectedModule *oldConnectedModule = findOldConnectedModule(newConnectedModule->getModuleID());
    if (oldConnectedModule == NULL)return;

    //Check if both modules have found each other
    if (!oldConnectedModule->checkHasNeighbor(newConnectedModule->getModuleID())) return;
    if (!newConnectedModule->checkHasNeighbor(oldConnectedModule->getModuleID())) return;

    printPuzzleGrid();

    Serial.println();
    Serial.println("!!!!! ----- ADDING NEW MODULE ----- !!!!!");
    
    
    //Rotate the new module to the correct orientation so it can connect to the existing piece
    rotateNewModule(newConnectedModule, oldConnectedModule);

    uint8_t x = 0;
    uint8_t y = 0;
    getOldModuleCoords(oldConnectedModule->getModuleID(), &x, &y);
    
    oldConnectedModule->printConnectors();
    calculateNewModuleCoords(newConnectedModule, oldConnectedModule, &x, &y);

    Serial.print("Coordinates to place new piece: [");
    Serial.print(x);
    Serial.print("][");
    Serial.print(y);
    Serial.println("]");
    placePuzzlePiece(newConnectedModule, x, y);

    Serial.println("!!!!! ----- FINISHED ----- !!!!!");
    Serial.println();

    printPuzzleGrid();

    return;
}


ConnectedModule *ModuleManager::findOldConnectedModule(uint8_t newModuleID)
{
    //Implementation:
    // 1. Find the piece on the board that the new module should connect to
    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        
        if (connectedModules[i]->getModuleID() == newModuleID) continue; //Skip the module that is being placed (Self)
        if(!connectedModules[i]->getPuzzlePlaced()) continue; //Skip modules that aren't already placed on the board
        
        {
            //Find a module that has a neighbor that is the module that is being placed
            if(connectedModules[i]->checkHasNeighbor(newModuleID)) return connectedModules[i];
        }
    }
    return NULL;
}
    /*
    for (uint8_t i = 0; i < connectedModules.size(); i++)
    {
        if (connectedModules[i]->getModuleID() == newModuleID)
        {
            //If the module is found, check if it is already placed on the board
            if (connectedModules[i]->getPuzzlePlaced())
            {
                //If the module is already placed on the board, remove it from the board
                for (uint8_t j = 0; j < TEMP_PUZZLEGRIDSIZE; j++)
                {
                    for (uint8_t k = 0; k < TEMP_PUZZLEGRIDSIZE; k++)
                    {
                        if (puzzlePieces[j][k].parentModule != NULL)
                        {
                            if (puzzlePieces[j][k].parentModule->getModuleID() == newModuleID)
                            {
                                puzzlePieces[j][k].parentModule = NULL;
                                puzzlePieces[j][k].pieceType = PUZZLEPIECE_TYPE_EMPTY;
                            }
                        }
                    }
                }
            }
        }
    }
}
*/

void ModuleManager::rotateNewModule(ConnectedModule *newConnectedModule, ConnectedModule *oldConnectedModule)
{
    CompassConnector oldConnectedModuleMatchingConnector = oldConnectedModule->getConnectorFromID(newConnectedModule->getModuleID());
    CompassConnector newConnectedModuleMatchingConnector = newConnectedModule->getConnectorFromID(oldConnectedModule->getModuleID());
    
    //newConnectedModule->printConnectors();
    //oldConnectedModule->printConnectors();

    Serial.println("Rotating Module");
    Serial.print("Rotation compensated direction on OLD module to place NEW module on: ");
    Serial.println(directionToString(oldConnectedModuleMatchingConnector.rotationCompensatedDirection));
    Serial.print("Rotation compensated direction on NEW module to place OLD module on: ");
    Serial.println(directionToString(newConnectedModuleMatchingConnector.rotationCompensatedDirection));

    //Rotate the new module to the correct orientation so it can connect to the existing piece
    switch (oldConnectedModuleMatchingConnector.rotationCompensatedDirection)
    {
        case DIRECTION_NORTH:
        //The new module should be placed on the north side of the old module
        Serial.println("The new module should be placed on the North side of the old module");
        //Check the rotation needed on the new module
        switch (newConnectedModuleMatchingConnector.rotationCompensatedDirection)//Returns the direction to which the old module is relative to the new module.
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
            switch (newConnectedModuleMatchingConnector.rotationCompensatedDirection)
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
            switch (newConnectedModuleMatchingConnector.rotationCompensatedDirection)
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
            switch (newConnectedModuleMatchingConnector.rotationCompensatedDirection)
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
}
void ModuleManager::getOldModuleCoords(uint8_t oldModuleID, uint8_t *x, uint8_t *y)
{
    //Find the location of the old module on the board
    Serial.println("Searching old module coords");
    for (uint8_t k = 0; k < TEMP_PUZZLEGRIDSIZE; k++)
    {
        for (uint8_t j = 0; j < TEMP_PUZZLEGRIDSIZE; j++)
        {
            if (puzzlePieces[k][j].parentModule != NULL)
            {
                if (puzzlePieces[k][j].parentModule->getModuleID() == oldModuleID)
                {
                    if (puzzlePieces[k][j].pieceType == PUZZLEPIECE_TYPE_HEART)
                    {
                        *x = k;
                        *y = j;
                        Serial.print("Found old module coords at [");
                        Serial.print(*x);
                        Serial.print("][");
                        Serial.print(*y);
                        Serial.println("]");
                        return;
                    }
                }
            }
        }
    }
}
void ModuleManager::calculateNewModuleCoords(ConnectedModule *newConnectedModule, ConnectedModule *oldConnectedModule, uint8_t *x, uint8_t *y)
{
    Serial.println("Calculating X and Y of the new piece. Orignal piece coords: ");
    Serial.print(*x);
    Serial.print(", ");
    Serial.println(*y);

    

    switch (oldConnectedModule->getConnectorFromID(newConnectedModule->getModuleID()).rotationCompensatedDirection) //Base it on the old connector's direction
    {
        case DIRECTION_NORTH: //If the new module should be placed on the north side of the old module
        {
            Serial.println("Placing on north side of old piece");
            *x -= 1;

            CompassConnector oldConnectorData = oldConnectedModule->getConnectorFromCompensatedDirection(DIRECTION_NORTH);
            *x -= oldConnectorData.basePipe; //If the new module has a pipe on the south side
            CompassConnector newConnectorData = newConnectedModule->getConnectorFromCompensatedDirection(DIRECTION_SOUTH);
            *x -= newConnectorData.basePipe; //If the new module has a pipe on the south side
        }
        break;
        case DIRECTION_EAST: //If the new module should be placed on the east side of the old module
        {
            Serial.println("Placing on east side of old piece");
            *y += 1;
            CompassConnector oldConnectorData = oldConnectedModule->getConnectorFromCompensatedDirection(DIRECTION_EAST);
            *y += oldConnectorData.basePipe; //If the new module has a pipe on the west side
            CompassConnector newConnectorData = newConnectedModule->getConnectorFromCompensatedDirection(DIRECTION_WEST);
            *y += newConnectorData.basePipe; //If the new module has a pipe on the west side
        }
        break;
        case DIRECTION_SOUTH: //If the new module should be placed on the south side of the old module
        {
            Serial.println("Placing on south side of old piece");
            *x += 1;

            CompassConnector oldConnectorData = oldConnectedModule->getConnectorFromCompensatedDirection(DIRECTION_SOUTH);
            *x += oldConnectorData.basePipe; //If the new module has a pipe on the north side
            CompassConnector newConnectorData = newConnectedModule->getConnectorFromCompensatedDirection(DIRECTION_NORTH);
            *x += newConnectorData.basePipe; //If the new module has a pipe on the north side
        }
        break;
        case DIRECTION_WEST: //If the new module should be placed on the west side of the old module
        {
            Serial.println("Placing on west side of old piece");
            *y -= 1;
            CompassConnector oldConnectorData = oldConnectedModule->getConnectorFromCompensatedDirection(DIRECTION_WEST);
            *y -= oldConnectorData.basePipe; //If the new module has a pipe on the east side
            CompassConnector newConnectorData = newConnectedModule->getConnectorFromCompensatedDirection(DIRECTION_EAST);
            *y -= newConnectorData.basePipe; //If the new module has a pipe on the east side
        }
        break;
    }
}
void ModuleManager::placePuzzlePiece(ConnectedModule *newConnectedModule, uint8_t firstFreeX, uint8_t firstFreeY)
{
    Serial.print("Placing puzzle piece ");
    Serial.println(newConnectedModule->getModuleID());

    //Place the new heart piece on the board
    editPuzzleGridPart(firstFreeX, firstFreeY, newConnectedModule, PUZZLEPIECE_TYPE_HEART, newConnectedModule->getBaseInfo().heartPiece);

    //Place pipes from heart piece on the board
    for (uint8_t i = 1; i <= DIRECTIONS; i++)
    {
        CompassConnector newPieceConnectorData = newConnectedModule->getConnectorFromCompensatedDirection(i);

        Serial.print("Checking direction ");
        Serial.print(directionToString(i));
        Serial.print(" - has pipe with length ");
        Serial.println(newPieceConnectorData.basePipe);

        for (uint8_t j = 1; j <= newPieceConnectorData.basePipe; j++)
        {
            Serial.print("Placing pipe on the board ");
            Serial.println(j);
            switch (i)
            {
                case DIRECTION_NORTH:
                editPuzzleGridPart(firstFreeX - j, firstFreeY, newConnectedModule, PUZZLEPIECE_TYPE_PIPE, BASE_PIPE_FORWARDBACKWARD);
                break;
                case DIRECTION_EAST:
                editPuzzleGridPart(firstFreeX, firstFreeY + j, newConnectedModule, PUZZLEPIECE_TYPE_PIPE, BASE_PIPE_LEFTRIGHT);
                break;
                case DIRECTION_SOUTH:
                editPuzzleGridPart(firstFreeX + j, firstFreeY, newConnectedModule, PUZZLEPIECE_TYPE_PIPE, BASE_PIPE_FORWARDBACKWARD);
                break;
                case DIRECTION_WEST:
                editPuzzleGridPart(firstFreeX, firstFreeY - j, newConnectedModule, PUZZLEPIECE_TYPE_PIPE, BASE_PIPE_LEFTRIGHT);
                break;
            }
        }
    }
    newConnectedModule->setPuzzlePlaced(true);
    Serial.println("Puzzle piece placed");
}

void ModuleManager::editPuzzleGridPart(uint8_t x, uint8_t y, ConnectedModule *parentModule, uint8_t pieceType, uint8_t basePiece)
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
    puzzlePieces[x][y].basePiece = basePiece;
}
void ModuleManager::DFS(int x, int y, const XYZ& start, const XYZ& end)
{
    // Check if the current node is the end node
    if (x == end.x && y == end.y) {
        // Record the step
        path.push_back(XYZ{x, y, 0});
        return;
    }

    // Mark current node as visited
    puzzlePieces[x][y].visited = true;
    Serial.print("("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.println(")");

    // Record the step
    path.push_back(XYZ{x, y, 0});

    // Define the four possible directions
    uint8_t directions[] = {DIRECTION_NORTH, DIRECTION_EAST, DIRECTION_SOUTH, DIRECTION_WEST};

    // Try each direction
    for (int i = 0; i < 4; i++) {
        XYZ next = {x, y, 0};
        switch (directions[i]) {
            case DIRECTION_NORTH: next.x--; break;
            case DIRECTION_EAST: next.y++; break;
            case DIRECTION_SOUTH: next.x++; break;
            case DIRECTION_WEST: next.y--; break;
        }

        // Check if the new node is valid and not visited
        if (isMovementAllowed(XYZ{x, y, 0}, directions[i]) && !puzzlePieces[next.x][next.y].visited && puzzlePieces[next.x][next.y].pieceType != PUZZLEPIECE_TYPE_EMPTY)
        {
            DFS(next.x, next.y, start, end);
            // If the end node has been found, return
            if (!path.empty() && path.back().x == end.x && path.back().y == end.y) {
                return;
            }
        }
    }

    // If all directions have been tried and none of them lead to the end node, remove the current node from the path
    if (!path.empty()) {
        path.pop_back();
    }
}
bool ModuleManager::isMovementAllowed(XYZ current, uint8_t direction)
{
    //First, check if the current position is in bounds
    if (current.x < 0 || current.x >= TEMP_PUZZLEGRIDSIZE || current.y < 0 || current.y >= TEMP_PUZZLEGRIDSIZE) return false;

    //Second, check if the direction to go to is in bounds
    switch (direction)
    {
        case DIRECTION_NORTH:
        if (current.x - 1 < 0) return false;
        break;
        case DIRECTION_EAST:
        if (current.y + 1 >= TEMP_PUZZLEGRIDSIZE) return false;
        break;
        case DIRECTION_SOUTH:
        if (current.x + 1 >= TEMP_PUZZLEGRIDSIZE) return false;
        break;
        case DIRECTION_WEST:
        if (current.y - 1 < 0) return false;
        break;
    }

    //Third, see if you can move from the current position in the given direction
    switch (direction)
    {
        case DIRECTION_NORTH:
        //If the 7th bit (The first bit from the left) is true, movement is allowed. Do bitwise AND with 0b10000000. (Clear out all other bits and check if the 7th bit is true	)
        if ((puzzlePieces[current.x][current.y].basePiece & 0b10000000) == 0b10000000) return false;
        break;
        case DIRECTION_EAST:
        if ((puzzlePieces[current.x][current.y].basePiece & 0b01000000) == 0b01000000) return false;
        break;
        case DIRECTION_SOUTH:
        if ((puzzlePieces[current.x][current.y].basePiece & 0b00100000) == 0b00100000) return false;
        break;
        case DIRECTION_WEST:
        if ((puzzlePieces[current.x][current.y].basePiece & 0b00010000) == 0b00010000) return false;
        break;
    }


    //Fourth, see if you can move into the next position in the given direction
    switch (direction)
    {
        case DIRECTION_NORTH:
        if ((puzzlePieces[current.x - 1][current.y].basePiece & 0b00100000) == 0b00100000) return false;
        break;
        case DIRECTION_EAST:
        if ((puzzlePieces[current.x][current.y + 1].basePiece & 0b00010000) == 0b00010000) return false;
        break;
        case DIRECTION_SOUTH:
        if ((puzzlePieces[current.x + 1][current.y].basePiece & 0b10000000) == 0b10000000) return false;
        break;
        case DIRECTION_WEST:
        if ((puzzlePieces[current.x][current.y - 1].basePiece & 0b01000000) == 0b01000000) return false;
        break;
    }

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

    //Pathfinding demo
    if (millis() - lastMillis_PathFindDemo > INTERVAL_PATHFINDDEMO)
    {
        if (connectedModules.size() < 2) return;

        Serial.println("Minimum 2 modules found: Pathfinding");

        //Reset pathfinding
        lastMillis_PathFindDemo = millis();
        path.clear();

        Serial.println("Reset list");

        for (uint8_t i = 0; i < TEMP_PUZZLEGRIDSIZE; i++)
        {
            for (uint8_t j = 0; j < TEMP_PUZZLEGRIDSIZE; j++)
            {
                puzzlePieces[i][j].visited = false;
            }
        }

        Serial.println("Reset visisted");

        //Get start and end coordinates by getting random heart pieces (Have to be puzzle placed)
        uint16_t attempts = 0;

        ConnectedModule *pathfindModuleA;
        while (true)
        {
            attempts++;
            if (attempts > 1000) return;
            pathfindModuleA = connectedModules[random(0, connectedModules.size()-1)];
            if (pathfindModuleA->getPuzzlePlaced()) break;
        }

        Serial.print("Module A found: ");
        Serial.println(pathfindModuleA->getModuleID());

        ConnectedModule *pathfindModuleB;
        while (true)
        {
            attempts++;
            if (attempts > 1000) return;
            pathfindModuleB = connectedModules[random(0, connectedModules.size()-1)];
            if(!pathfindModuleB->getPuzzlePlaced() || pathfindModuleB->getModuleID() != pathfindModuleA->getModuleID()) break;
        }

        Serial.print("Module B found: ");
        Serial.println(pathfindModuleB->getModuleID());

        Serial.print("Pathfinding from ");
        Serial.print(pathfindModuleA->getModuleID());
        Serial.print(" to ");
        Serial.println(pathfindModuleB->getModuleID());
        


        uint8_t x = 0;
        uint8_t y = 0;
        getOldModuleCoords(pathfindModuleA->getModuleID(), &x, &y);
        XYZ start = XYZ{x, y, 0};
        
        getOldModuleCoords(pathfindModuleB->getModuleID(), &x, &y);
        XYZ end = XYZ{x, y, 0};
        
        DFS(start.x, start.y, start, end);
        Serial.println("Path:");
        for (uint8_t i = 0; i < path.size(); i++)
        {
            Serial.print("(");
            Serial.print(path[i].x);
            Serial.print(", ");
            Serial.print(path[i].y);
            Serial.println(")");
        }
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