#include "AB2KIRSensors.h"
#include <EEPROM.h>

int AB2KIRSensors::sensors[] = { A1, A0, A2, A6, A7 };

AB2KIRSensors::AB2KIRSensors(HardwareSerial * dbp)
{
  int i; 
  dbg_serialp = dbp; 
  for(i = 0; i < 5; i++) {
    sensor_range[i] = 1024; 
    sensor_min[i] = 0;
    pinMode(sensors[i], INPUT);
    digitalWrite(sensors[i], LOW); 
  }

  last_position = 0; 
}

void AB2KIRSensors::retrieveCalibration(int base_addr)
{
  int i; 
  int addr = base_addr;
  for(i = 0; i < 5; i++) {
    sensor_range[i] = intReadEEPROM(addr);
    addr += 2; 
    sensor_min[i] = intReadEEPROM(addr);
    addr += 2; 
  }
}

void AB2KIRSensors::calibrateSensors()
{
  int i; 
  for(i = 0; i < 5; i++) {
    if(dbg_serialp != NULL) {
      dbg_serialp->print("Cal for channel "); dbg_serialp->println(i); 
    }

    while(1) {
      if(doCalibration(i)) {
	if(dbg_serialp != NULL) {
	  dbg_serialp->print("\tCal success: min = "); dbg_serialp->print(getSensorMin(i));
	  dbg_serialp->print(" range = "); dbg_serialp->println(getSensorRange(i));
	}
	break; 
      }
      else{
	if(dbg_serialp != NULL) {
	  dbg_serialp->println("\tCal fails.");
	}
      }
    }
  }
}

bool AB2KIRSensors::doCalibration(int chan_number)
{
  // clear the histogram
  clearHisto();

  int i; 
  // now collect 1024 samples.. That will take 
  // 10 seconds.
  for(i = 0; i < 1024; i++) {
    int val = analogRead(sensors[chan_number]);
    histo[(val >> HIST_DIV) & HIST_LIM]++; 
    delay(2);
  }

  // now look for the top and bottom
  int top_peak = 0; 
  int top_idx = 0; 
  int bot_peak = 0; 
  int bot_idx = 0; 
  int found_top = 0; 
  int found_bot = 0; 

  for(i = 0; i < HIST_SIZE; i++) {
    if(found_top == 0) {
      // look for top value
      if(histo[HIST_LIM - i] > top_peak) {
	top_peak = histo[HIST_LIM - i];
	top_idx = HIST_LIM - i; 
      }
      else if(histo[HIST_LIM - i] < (top_peak >> 2)) {
	found_top = 1; 
      }
    }

    if(found_bot == 0) {
      // look for bottom value
      if(histo[i] > bot_peak) {
	bot_peak = histo[i];
	bot_idx = i;
      }
      else if(histo[i] < (bot_peak >> 2)) {
	found_bot = 1; 
      }
    }
  }

  if((found_top && found_bot) && ((top_idx - bot_idx) > 20)) {
    // we have a good cal. 
    sensor_min[chan_number] = (bot_idx << HIST_DIV);
    sensor_range[chan_number] = ((top_idx - bot_idx) << HIST_DIV);
    return true; 
  }
  return false; 
}

void AB2KIRSensors::saveCalibration(int base_addr)
{
  int i; 
  int addr = base_addr; 
  for(i = 0; i < 5; i++) {
    intWriteEEPROM(addr, sensor_range[i]);
    addr += 2; 
    intWriteEEPROM(addr, sensor_min[i]);
    addr += 2; 
  }
}

int AB2KIRSensors::getPosition()
{
  // first, read all five sensors. 
  long i; 
  long sum = 0; 
  long wsum = 0; 
  sense_count = 0; 
  for(i = 0; i < 5; i++) {
    long val = readChannel(i);
    val = ((val - ((long) sensor_min[i])) * 1024L) / ((long) sensor_range[i]);
    if(val < 0) val = 0; 
    if(val > 1024) val = 1024; 
    if(val > 512) sense_count++; 

    sensor_val[i] = val; 
    sum += val; 
    wsum += ((i * val) << 10); 
  }

  int ret; 
  if(sense_count < 1) ret = last_position; 
  else ret = ((int) (wsum / sum));

  last_position = ret; 
  
  return ret; 
}

int AB2KIRSensors::getSensorMin(int chan_number)
{
  return sensor_min[chan_number]; 
}

int AB2KIRSensors::getSensorRange(int chan_number)
{
  return sensor_range[chan_number]; 
}

void AB2KIRSensors::clearHisto()
{
  int i; 
  for(i = 0; i < HIST_SIZE; i++) histo[i] = 0; 
}

int AB2KIRSensors::readChannel(int chan_number)
{
  delay(1); 
  int ret = analogRead(sensors[chan_number]);
  return ret; 
}

// EEPROM data is written LSbyte first. 
void AB2KIRSensors::intWriteEEPROM(int addr, int val)
{
  EEPROM.update(addr << 1, (val & 0xff));
  EEPROM.update((addr << 1) + 1, ((val >> 8) & 0xff));
}

int AB2KIRSensors::intReadEEPROM(int addr)
{
  int ret = 0; 
  ret = EEPROM.read(addr << 1); 
  ret = ret | ((EEPROM.read((addr << 1) + 1)) << 8);
  return ret; 
}


int AB2KIRSensors::getSensorAbs(int chan_number)
{
  return analogRead(sensors[chan_number]);
}
