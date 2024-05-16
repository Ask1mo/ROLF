#include "PuzzlePiece_Ledpipe.h"

//Constructor
PuzzlePiece_Ledpipe::PuzzlePiece_Ledpipe(const char* title, const unsigned char *sprite, uint8_t spriteType)
{
  //Serial.print(F("Constructing derived (PuzzlePiece_Ledpipe): "));
  //Serial.println(title);
  this->title = title;
  this->sprite = sprite;
  this->spriteType = spriteType;
  //thingsSize = 0;
}

// PuzzlePiece_Ledpipe::~PuzzlePiece_Ledpipe()
// {
//   //delete things;
// }


//Private
void PuzzlePiece_Ledpipe::updateData(bool data)
{
  this->data = data;
}

//Public
void PuzzlePiece_Ledpipe::toText(char* text)
{
  text[0] = '[';

  if (data == 0) text[1] = 'O';
  else if (data == 1) text[1] = 'X';
  else text[1] = '?';
  
  text[2] = ']';
  text[3] = ' ';
  //text[4] = title;
  strcpy(&text[4], title);
}
const unsigned char* PuzzlePiece_Ledpipe::getDataSprite()
{
  if (data)
  {
    return sprite_MenuItem1Bool_true;
  }
  else 
  {
    return sprite_MenuItem1Bool_false;
  }
}

void PuzzlePiece_Ledpipe::printName()
{
  Serial.print(F("1BOOL: Printing my title: "));
  Serial.print(title);
}