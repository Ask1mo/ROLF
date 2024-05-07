#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello World");
}

void loop()
{
  //Print anything on serial read
  if (Serial.available() > 0)
  {
    int data = Serial.read();
    Serial.println(data);
  }
  
}
