#ifndef AB2K_IR_SENS_HDR
#define AB2K_IR_SENS_HDR

#include <Arduino.h>

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

#define HIST_BITS 8
#define HIST_DIV (10 - HIST_BITS)
#define HIST_SIZE (1 << HIST_BITS)
#define HIST_LIM (HIST_SIZE - 1)

class AB2KIRSensors {
 public:
  /**
   * @brief Constructor for 5 IR sensor array on AttentanceBot 2000.
   */
  AB2KIRSensors(HardwareSerial * dbp = NULL); 

  /**
   * @brief fetch calibration constants from EEPROM location
   * @param base_addr address of start of region in EEPROM with calibration constants
   */
  void retrieveCalibration(int base_addr);

  /** 
   * @brief perform calibration experiment across all sensors
   */
  void calibrateSensors(); 

  /**
   * @brief perform calibration experiment for a channel
   * 
   * @param chan_number number of channel to be tested. 
   * @return true if calibration is complete, false otherwise. 
   */
  bool doCalibration(int chan_number);

  /**
   * @brief save calibration constants to EEPROM location
   * @param base_addr address of start of region in EEPROM with calibration constants
   */
  void saveCalibration(int base_addr);

  /**
   * @brief calculate position of line under sensor array
   * 
   * @return a value in the range 0..4000 indicates the position of the 
   * line with 2000 indicating dead center. A value of -1 indicates that
   * more than 3 sensors have seen a line, indicating that we should stop.
   */
  int getPosition();

  int getSensorMin(int chan_number); 
  int getSensorRange(int chan_number);
  int getSensorAbs(int chan_number);  
  int getSenseCount() { return sense_count; }

  // private:
  int sensor_range[5]; 
  int sensor_min[5]; 
  int sensor_val[5]; 
  int histo[HIST_SIZE]; 
  
  int last_position; 
  int sense_count; 

  static int sensors[5]; 

  void clearHisto();
  int readChannel(int chan_number); 

  void intWriteEEPROM(int addr, int val);
  int intReadEEPROM(int addr); 

  HardwareSerial * dbg_serialp; 
}; 

#endif
