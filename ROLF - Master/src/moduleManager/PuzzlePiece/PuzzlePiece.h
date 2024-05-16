#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include "setup.h"
#include "../connectedModule/connectedModule.h"

struct PuzzlePieceStruct
{
    ConnectedModule *parentModule;
    uint8_t pieceType;
    uint8_t rotation;

    public:
    //MenuItem(char* title); //No constructor
    ~PuzzlePiece();

    ConnectedModule *getParentModule();
    uint8_t getType();
    uint8_t getRotation();
    void setRotation(uint8_t rotation);
};

class PuzzlePiece
{
    protected:
    ConnectedModule *parentModule;
    uint8_t rotation;

    public:
    //MenuItem(char* title); //No constructor
    ~PuzzlePiece();

    ConnectedModule *getParentModule();
    uint8_t getType();
    uint8_t getRotation();
    void setRotation(uint8_t rotation);
};

#endif