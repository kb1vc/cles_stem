#!/bin/bash

arduino --pref sketchbook.path=`pwd` --board arduino:avr:pro --port /dev/ttyUSB0  --upload AB2K/AB2K.ino
