#ifndef PUZZLEPIECE_HEART_H
#define PUZZLEPIECE_HEART_H

#include "setup.h"
#include "../PuzzlePiece/PuzzlePiece.h"


class PuzzlePiece_Heart : public PuzzlePiece
{
    private:
    bool data;
    
    public:
    PuzzlePiece_Heart(ConnectedModule *parentModule);
    ~PuzzlePiece_Heart();
    void updateData(bool data);
    void toText(char* text);
    const unsigned char* getDataSprite();
    void printName();
};

#endif