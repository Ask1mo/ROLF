#include "PuzzlePiece.h"

PuzzlePiece::~PuzzlePiece()
{
  Serial.println(F("DEstructing base (PuzzlePiece): "));
}

//Private

//Public
ConnectedModule *PuzzlePiece::getParentModule()
{
  return parentModule;
}
uint8_t PuzzlePiece::getRotation()
{
  return rotation;
}
void PuzzlePiece::setRotation(uint8_t rotation)
{
  this->rotation = rotation;
}
