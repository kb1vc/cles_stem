#ifndef AB2K_MOTO_DEF
#define AB2K_MOTO_DEF

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


/**
 * @brief AttendanceBot 2000 Motor Driver 
 */
#include <Arduino.h>

class AB2KMotor {
 public:
  /**
   * @brief constructor
   *
   * @param left_dirpin Arduino IO pin controlling left motor direction
   * @param left_speedpin Arduino IO pin controlling left motor speed
   * @param right_dirpin Arduino IO pin controlling right motor direction
   * @param right_speedpin Arduino IO pin controlling right motor speed
   */
  AB2KMotor(int left_dirpin, int left_speedpin, 
	    int right_dirpin, int right_speedpin);


  /**
   * @brief stop both motors
   */
  void stop(); 

  /**
   * @brief set the speed for both motors
   *
   * @param left_speed (integer clipped to range -1023 to 1023)
   * @param right_speed (integer clipped to range -1023 to 1023)
   * @param apply_corr if true, apply correction from setMotorCorrection
   */
  void setSpeed(int left_speed, int right_speed, bool apply_corr = true); 

  /**
   * @brief set a correction factor to compensate for imperfect
   * match between the left and right motor. 
   * 
   * @param left_incr -- added to left motor speed 
   * @param right_incr -- added to right motor speed
   */
  void setMotorCorrection(int left_incr, int right_incr); 

 private:
  int ldir_pin; 
  int lspeed_pin; 
  int rdir_pin; 
  int rspeed_pin; 

  int l_corr; 
  int r_corr; 
}; 
#endif
