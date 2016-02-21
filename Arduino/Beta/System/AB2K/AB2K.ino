
#include <SoftwareSerial.h>

#include "AB2KIRSensors.h"
#include "AB2KSounds.h"


#define SFX_TX 5
#define SFX_RX 6
#define SFX_RST 4
#define SFX_ACT 2

SoftwareSerial smod(SFX_TX, SFX_RX); 
AB2KIRSensors * ir;
AB2KSounds * sounds;

#define GREEN_LIGHT 9
#define GO_BUTTON 10
int toggle; 
void setup()
{
  pinMode(GO_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LIGHT, OUTPUT);

  toggle = 0;
  Serial.begin(9600);
  Serial.println("Starting up.");
  // setup software serial
  smod.begin(9600);

  sounds = new AB2KSounds(&smod, SFX_RST, SFX_ACT, &Serial);
  ir = new AB2KIRSensors(&Serial);
  sounds->setVolume(140);

  if(!digitalRead(GO_BUTTON)) {
    sounds->playFileName("CALMODESWAV", true);
    ir->calibrateSensors();
    sounds->playFileName("CALMODEEWAV", true);  
  }
  else {
    sounds->playFileName("IAMLOST0WAV", true);      
  }
}


void loop()
{
  digitalWrite(GREEN_LIGHT, (toggle ? HIGH : LOW));

  while(1) {
    int pos = ir->getPosition(); 
    int sc = ir->getSenseCount(); 
    Serial.print(sc); Serial.print(" "); Serial.println(pos);
    if(sc == 5) break; 
    delay(1000);
  }
  sounds->playFileName("GIVECARDWAV", true); 

  while(1) {
    int pos = ir->getPosition(); 
    int sc = ir->getSenseCount(); 
    //     Serial.print(sc); Serial.print(" "); Serial.println(pos);
    if(sc == 5) {
      sounds->playFileName("KLAXON01WAV", true);
      delay(500);
    }
    delay(10);
  }
  
}
