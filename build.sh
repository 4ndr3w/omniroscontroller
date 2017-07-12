#!/bin/sh
arduino-builder -hardware /usr/share/arduino/hardware -tools /usr/share/arduino/tools -fqbn teensy:avr:teensy35:usb=serial,speed=120,keys=en-us,opt=o2std -compile OmniBotController.ino
