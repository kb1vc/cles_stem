#include "AB2KMotor.h"

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

AB2KMotor::AB2KMotor(int left_dirpin, int left_speedpin, 
		       int right_dirpin, int right_speedpin)
{
  ldir_pin = left_dirpin; 
  lspeed_pin = left_speedpin; 
  rdir_pin = right_dirpin; 
  rspeed_pin = right_speedpin; 

  pinMode(ldir_pin, OUTPUT);
  pinMode(rdir_pin, OUTPUT);
  pinMode(lspeed_pin, OUTPUT);
  pinMode(rspeed_pin, OUTPUT);    

#if 0  
  // set PWM rate to 4kHz or so. 
  TCCR2B = (TCCR2B & 0xF8) | 2;
  // whould ou believe 122 Hz
  TCCR2B = (TCCR2B & 0xF8) | 6; // 122 is good, is 30 better? 
#endif
  
  analogWrite(lspeed_pin, 0);
  analogWrite(rspeed_pin, 0);

  l_corr = 0; 
  r_corr = 0; 

  stop();
}


void AB2KMotor::stop()
{
  setSpeed(0,0);
}


void AB2KMotor::setSpeed(int left_speed, int right_speed, bool apply_corr)
{
  int lsp, rsp; 

  lsp = left_speed + l_corr; 
  rsp = right_speed + r_corr; 

  if(lsp < 0) {
    digitalWrite(ldir_pin, HIGH);
    lsp = (lsp < -1023) ? 1023 : (- lsp); 
  }
  else {
    digitalWrite(ldir_pin, LOW);
    lsp = (lsp > 1023) ? 1023 : lsp; 
  }
  if(rsp < 0) {
    digitalWrite(rdir_pin, HIGH);
    rsp = (rsp < -1023) ? 1023 : (- rsp); 
  }
  else {
    digitalWrite(rdir_pin, LOW);
    rsp = (rsp > 1023) ? 1023 : rsp;
  }

  analogWrite(lspeed_pin, lsp); 
  analogWrite(rspeed_pin, rsp); 
}


void AB2KMotor::setMotorCorrection(int left_incr, int right_incr)
{
  l_corr = left_incr; 
  r_corr = right_incr; 
}

