#ifndef PUZZLEPIECE_LEDPIPE_H
#define PUZZLEPIECE_LEDPIPE_H

#include "setup.h"
#include "../PuzzlePiece/PuzzlePiece.h"

class PuzzlePiece_Ledpipe : public PuzzlePiece
{
    private:
    uint8_t pipeLength;
    

    public:
    PuzzlePiece_Ledpipe(ConnectedModule *parentModule);
    ~PuzzlePiece_Ledpipe();
    void updateData(bool data);
    void toText(char* text);
    uint8_t getPuzzlePieceType();
    void printName();
};

#endif