
#include <SoftwareSerial.h>

#include "AB2KIRSensors.h"
#include "AB2KSounds.h"
#include "AB2KMotor.h"

#define SFX_TX 5
#define SFX_RX 6
#define SFX_RST 4
#define SFX_ACT 2

SoftwareSerial smod(SFX_TX, SFX_RX); 
AB2KIRSensors * ir;
AB2KSounds * sounds;
AB2KMotor * motors; 

#define GREEN_LIGHT 9
#define GO_BUTTON 10

#define LDIR 12
#define LPWM 3
#define RDIR 13
#define RPWM 11

int last_proportional;
int integral;
bool waiting_for_button_press;
unsigned long last_lamp_time;
unsigned long last_stop_time; 
bool lamp_state; 
unsigned int wait_count; 
void setup()
{
  pinMode(GO_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LIGHT, OUTPUT);

  Serial.begin(9600);
  Serial.println("Starting up.");
  // setup software serial
  smod.begin(9600);

  motors = new AB2KMotor(LDIR, LPWM, RDIR, RPWM); 
  sounds = new AB2KSounds(&smod, SFX_RST, SFX_ACT, &Serial);
  ir = new AB2KIRSensors(&Serial);
  sounds->setVolume(140);

  sounds->playFileName("FANFARE0WAV", true);
  delay(2000);
  
  if(!digitalRead(GO_BUTTON)) {
    sounds->playFileName("CALMODESWAV", true);
    ir->calibrateSensors();
    sounds->playFileName("CALMODEEWAV", true);  
    ir->saveCalibration(0);
  }
  else {
    ir->retrieveCalibration(0);    
    // int j; 
    // for(j = 0; j < 5; j++) {
    //   sounds->sayNumber(j);
    //   delay(500);
    //   sounds->sayNumber(ir->getSensorMin(j));
    //   delay(500);
    //   sounds->sayNumber(ir->getSensorRange(j));
    //   delay(500);
    // }
    sounds->playFileName("IAMLOST0WAV", true);
  }

  integral = 0;
  last_proportional = 0; 

  waiting_for_button_press = false;


  last_stop_time = millis();
  lamp_state = false; 
  digitalWrite(GREEN_LIGHT, lamp_state ? HIGH : LOW);

  // setup the button lamp interrupt
  last_lamp_time = last_stop_time;

}

// 8 no follow 4 better  2 better
#define PGain_DIV 8
#define IGain_DIV (8*1024)
// was ((a * 3) >> 1)
#define DGain_DIV(a) ((a * 3) >> 1)
#define VEL 200

void chaseLine()
{
  // lifted from 3pi linefollower code. 
  long pos = ir->getPosition();
  if(ir->getSenseCount() == 5) {
    waiting_for_button_press = true; 
    motors->stop();
    last_stop_time = millis() - 20000;
    wait_count = 0; 
    return; 
  }
  int proportional = ((int) pos) - 2048;
  int derivative = proportional - last_proportional;
  integral += proportional; 
  last_proportional = proportional; 
  int power_difference;
  //power_difference = proportional / 8 + integral / 10000 + derivative * 3 / 2;
  power_difference = proportional / PGain_DIV + DGain_DIV(derivative) + integral / IGain_DIV;
  
  const int max = VEL;
  
  if(power_difference > max) power_difference = max; 
  if(power_difference < -max) power_difference = -max; 
  
  motors->setSpeed(VEL + power_difference, VEL - power_difference);
  if(power_difference > 0) {
    motors->setSpeed(VEL, VEL - power_difference);    
  }
  else {
    motors->setSpeed(VEL + power_difference, VEL);
  }
  
}

char songbuf[] = "SONG0001WAV";

void loop()
{
  unsigned long ct = millis();        
  if(!waiting_for_button_press) {
    chaseLine();
  }
  else {
    if(!digitalRead(GO_BUTTON)) {
      waiting_for_button_press = false;
      digitalWrite(GREEN_LIGHT, LOW);
      sounds->stop();
      int songsel = (ct & 3);
      songbuf[7] = '0' + ((char) songsel);
      sounds->playFileName(songbuf, false);
    }
    else {
      if((ct - last_stop_time) > 20000) {
	last_stop_time = ct; 
	wait_count++; 
	if(wait_count > 3) {
	   sounds->playFileName("KLAXON01WAV", true);
	   sounds->playFileName("KLAXON01WAV", true);
	   sounds->playFileName("KLAXON01WAV", true);
	   sounds->playFileName("KLAXON00WAV", true);
	   sounds->playFileName("KLAXON00WAV", true);
	   sounds->playFileName("KLAXON00WAV", true);
	   wait_count = 1; 
	}
	sounds->stop();
	sounds->playFileName("GIVECARDWAV", true);
	while(sounds->isBusy()) { delay(100); }
	sounds->playFileName("POKEBUTNWAV", false);	
      }
    }

    if((ct - last_lamp_time) > 250) {
      lamp_state = !lamp_state; 
      digitalWrite(GREEN_LIGHT, lamp_state ? HIGH : LOW);
      last_lamp_time = ct; 
    }
    
  }


}

