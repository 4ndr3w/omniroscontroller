#include "Servo.h"
#include "ports.h"
#include "motors.h"

Servo leftPWM;
Servo rightPWM;
Servo frontPWM;
Servo backPWM;


void initMotors() {
    leftPWM.attach(LEFT_PWM);
    rightPWM.attach(RIGHT_PWM);
    frontPWM.attach(FRONT_PWM);
    backPWM.attach(BACK_PWM);

    setMotors(0, 0, 0, 0);
}

void setMotors(float left, float right, float front, float back) {
    left  /= MAX_VELOCITY;
    right /= MAX_VELOCITY;
    front /= MAX_VELOCITY;
    back  /= MAX_VELOCITY;


    left *= 500;
    left += 1500;

    right *= 500;
    right += 1500;

    front *= 500;
    front += 1500;

    back *= 500;
    back += 1500;

    leftPWM.writeMicroseconds(left);
    rightPWM.writeMicroseconds(right);
    frontPWM.writeMicroseconds(front);
    backPWM.writeMicroseconds(back);
}
