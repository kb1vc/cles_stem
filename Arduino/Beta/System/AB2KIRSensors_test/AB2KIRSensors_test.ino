#include "AB2KIRSensors.h"

#include "AB2KSounds.h"

AB2KIRSensors ir;
AB2KSounds sounds; 

#define GREEN_LIGHT 9
int toggle; 
void setup()
{
  
  pinMode(GREEN_LIGHT, OUTPUT);
  toggle = 0;
  Serial.begin(9600);
}


void loop()
{
  digitalWrite(GREEN_LIGHT, (toggle ? HIGH : LOW));

  Serial.print(ir.getSensorAbs(toggle)); Serial.print(" ");
  int v = ir.getSensorAbs(toggle);
  sounds.sayNumber(v);
  toggle++;
  if(toggle > 4) {
    toggle = 0;
    delay(1000);
    Serial.println("");
  }
}
