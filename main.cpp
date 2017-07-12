#include <Arduino.h>
#include "ports.h"
#include "motors.h"
#include "encoders.h"
#include "gyro.h"
#include "Rate.h"
#include "SerialPort.h"
#include "Watchdog.h"

struct RobotCommand {
  float leftVelocity;
  float rightVelocity;
  float frontVelocity;
  float backVelocity;
};

struct RobotStatus {
  float x;
  float y;
  float heading;

  float vx;
  float vy;
  float vth;
};

struct out {
	double val1;
	double val2;
};

struct resp {
	volatile char res[50];
};

void robotMain() {
  pinMode(LOOPMONITOR, OUTPUT);
  SerialPort<RobotCommand, RobotStatus> p(9600);

  initMotors();
  initEncoders();
  initGyro();
  setMotors(0, 0, 0, 0);

  float lastDist = 0;

  Watchdog watchdog(500);

  float dt = 1.0 / 100.0;
  Rate r(100);
  RobotStatus pose = { 0, 0, 0 };

  float lastHeading = 0;
  pinMode(13, OUTPUT);
  while(1) {
    if ( r.needsRun() ) {
      digitalWrite(LOOPMONITOR, HIGH);

      float dist = (getLeftEncoder() + getRightEncoder()) / 2.0;
      float deltaDist = dist-lastDist;
      float heading = getHeading();
      float headingRad = heading * (M_PI/180.0);

      float dy = deltaDist * cos(headingRad);
      float dx = deltaDist * sin(headingRad);
      
      pose.y += dy;
      pose.x -= dx;
      pose.heading = heading;

      pose.vy = dy / dt;
      pose.vx = dx / dt;
      pose.vth = (heading - lastHeading) / dt;

      lastHeading = heading;
      lastDist = dist;

      p.sendMessage(pose);
      digitalWrite(LOOPMONITOR, LOW);
    }

    if ( p.hasMessage() ) {
      RobotCommand command = p.getMessage();
      setMotors(command.leftVelocity, command.rightVelocity, command.frontVelocity, command.backVelocity);
      watchdog.feed();
      digitalWrite(13, LOW);
    }
  
    if ( watchdog.hungry() ) {
      setMotors(0, 0, 0, 0);
      digitalWrite(13, HIGH);
    }

  }
}
