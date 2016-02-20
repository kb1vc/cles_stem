#!/bin/bash

arduino --pref sketchbook.path=`pwd` --board arduino:avr:pro --port /dev/ttyUSB0  --upload AB2KIRSensors_test/AB2KIRSensors_test.ino
