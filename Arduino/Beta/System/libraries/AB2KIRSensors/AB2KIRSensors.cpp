#include "AB2KIRSensors.h"
#include <EEPROM.h>

int AB2KIRSensors::sensors[] = { A1, A0, A2, A6, A7 };

AB2KIRSensors::AB2KIRSensors()
{
  int i; 
  for(i = 0; i < 5; i++) {
    sensor_range[i] = 1024; 
    sensor_min[i] = 0;
    pinMode(sensors[i], INPUT);
    digitalWrite(sensors[i], LOW); 
  }
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

bool AB2KIRSensors::setCalibration(int chan_number)
{
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
}

int AB2KIRSensors::getSensorMin(int chan_number)
{
}

int AB2KIRSensors::getSensorRange(int chan_number)
{
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
