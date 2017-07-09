#include <Arduino.h>
#include "ports.h"
#include "encoders.h"

#include <Servo.h>

Servo s;

#include "Rate.h"
#include "SerialPort.h"

struct Data {
  int16_t number;
};

struct RobotState {
  
}


void robotMain() {  

  SerialPort<int16_t, Data> p(9600);

  while ( 1 ) {
    int16_t n = p.getMessage();

    Data d;

    d.number = n;

    sprintf(d.text, "You sent: %d\n", n);

    p.sendMessage(d);
  }

  /*
  bool state = true;
  Rate t(1000);
  pinMode(17, OUTPUT);
  while ( 1 ) {
    
    digitalWrite(17, HIGH);
    delayMicroseconds(500);
    digitalWrite(17, LOW);
    
    //Serial.print('w');
    t.sleep();
  }*/

  /*initEncoders();
  //s.attach(FRONT_PWM);
  double us = 1.5 * 1000.0;
  while(1) {
    //s.writeMicroseconds(500);
  }*/
}
