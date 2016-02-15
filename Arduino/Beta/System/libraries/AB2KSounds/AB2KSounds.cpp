#include "AB2KSounds.h"

/*
  Copyright (c) 2016, Matthew H. Reilly (kb1vc)
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <SoftwareSerial.h>

#define SFX_TX 5
#define SFX_RX 6
#define SFX_RST 4
#define SFX_ACT 2

/*
  0	name: CENTER00WAV	size: 8044
  1	name: LEFT0000WAV	size: 8044
  2	name: RIGHT000WAV	size: 8044
  3	name: DIGIT000WAV	size: 8044
  4	name: DIGIT111WAV	size: 8044
  5	name: DIGIT222WAV	size: 8044
  6	name: DIGIT333WAV	size: 8044
  7	name: DIGIT444WAV	size: 8044
  8	name: DIGIT555WAV	size: 8044
  9	name: DIGIT666WAV	size: 8044
  10	name: DIGIT777WAV	size: 8044
  11	name: DIGIT888WAV	size: 8044
  12	name: DIGIT999WAV	size: 8044
  13	name: SPACE000WAV	size: 8044
  14	name: GIVECARDWAV	size: 40044
  15	name: POKEBUTNWAV	size: 40044
  16	name: IAMLOST0WAV	size: 40044
  17	name: KLAXON~1WAV	size: 19568
  18	name: KLAXON~2WAV	size: 6986
  19	name: KLAXON~3WAV	size: 29466
  20	name: SONG0000WAV	size: 422088
  21	name: SONG0001WAV	size: 275712
  22	name: SONG0002WAV	size: 396084
  23	name: SONG0003WAV	size: 396270
*/

SoftwareSerial smod(SFX_TX, SFX_RX);

char * AB2KSounds::digit_strings[] = { 
  "DIGIT000WAV",
  "DIGIT111WAV",
  "DIGIT222WAV",
  "DIGIT333WAV",
  "DIGIT444WAV",
  "DIGIT555WAV",
  "DIGIT666WAV",
  "DIGIT777WAV",
  "DIGIT888WAV",
  "DIGIT999WAV"
};

AB2KSounds::AB2KSounds()
{
  // setup software serial
  smod.begin(9600);

  // assert reset
  pinMode(SFX_RST, OUTPUT);
  digitalWrite(SFX_RST, LOW);
  digitalWrite(SFX_RST, HIGH); 

  // set volume
  setVolume(154); 
}

void AB2KSounds::sayDigit(int digit)
{
  if(digit > 9) return; 
  if(digit < 0) return; 
  playFileName(digit_strings[digit], true); 
}

void AB2KSounds::sayNumber(int num)
{
  int rem;
  int dig; 
  // say a number starting from its MSB. 
  if(num >= 10000) {
    dig = num / 10000; 
    sayDigit(dig); 
    rem = num - dig * 10000; 
  }

  if(num >= 1000) {
    dig = rem / 1000; 
    sayDigit(dig); 
    rem = rem - dig * 1000; 
  }
  
  if(num >= 100) {
    dig = rem / 100; 
    sayDigit(dig); 
    rem = rem - dig * 100; 
  }
  
  if(num >= 10) {
    dig = rem / 10; 
    sayDigit(dig); 
    rem = rem - dig * 10; 
  }
  else {
    sayDigit(rem); 
  }

}
void AB2KSounds::saySensorName(int chan_num)
{
  switch (chan_num) {
  case 1:
    playFileName("CENTER00WAV", true);
  case 0:
    playFileName("LEFT0000WAV", true);
    break; 
  case 2:
    playFileName("CENTER00WAV", true);
    break;
  case 3:
    playFileName("CENTER00WAV", true);
  case 4:
    playFileName("RIGHT000WAV", true);
  }
}

void AB2KSounds::flushInput()
{
  while(smod.available()) smod.read();  
}

   
int AB2KSounds::setVolume(int tv)
{
  // flush the serial input
  flushInput();
  
  int i; 
  int v = 1024; 
  char lb[20]; 
  char cmd[2]; 
  cmd[0] = '-'; cmd[1] = '\000';
  for(i = 0; i < 256; i++) {
    smod.println(cmd);
    smod.readBytesUntil('\n', lb, 20);
    v = atoi(lb);
    if(v = tv) return v; 
    if(v < tv) cmd[0] = '+';
    if(v > tv) cmd[0] = '-'; 
  }
  
  return v; 
}

void AB2KSounds::playSong(int song_num)
{
  if(song_num > 3) song_num = 3; 
  if(song_num < 0) song_num = 0; 
  char sname[] = "SONG0000WAV"; 
  sname[7] = '0' + song_num; 
  playFileName(sname, false);
}

void AB2KSounds::playFileName(const char * fname, bool wait_for_end)
{
  // this is pre-emptive.   If anything else is going on, we cause it to quit.
  if(isBusy()) smod.println("q");

  smod.print("P"); smod.println(fname); 
  while(!isBusy()) delay(1); // wait for the sound to start. 

  if(wait_for_end) {
    while(isBusy()) delay(1); 
  }
  flushInput(); 
}

bool AB2KSounds::isBusy()
{
  int v = digitalRead(SFX_ACT); 
  return v == 0;
}
