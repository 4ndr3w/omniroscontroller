#include <Arduino.h>
#include "encoders.h"
#include "ports.h"

volatile int leftEncoder = 0;
volatile int rightEncoder = 0;
volatile int frontEncoder = 0;
volatile int backEncoder = 0;

void leftEncoderISR() {
    leftEncoder += digitalRead(LEFT_ENC_B) ? 1 : -1;
}

void rightEncoderISR() {
    rightEncoder += digitalRead(RIGHT_ENC_B) ? -1 : 1;
}

void frontEncoderISR() {
    frontEncoder += digitalRead(FRONT_ENC_B) ? 1 : -1;
}

void backEncoderISR() {
    backEncoder += digitalRead(BACK_ENC_B) ? 1 : -1;
}

void initEncoders() {
    pinMode(LEFT_ENC, INPUT);
    pinMode(LEFT_ENC_B, INPUT);

    pinMode(RIGHT_ENC, INPUT);
    pinMode(RIGHT_ENC_B, INPUT);

    pinMode(FRONT_ENC, INPUT);
    pinMode(FRONT_ENC_B, INPUT);

    pinMode(BACK_ENC, INPUT);
    pinMode(BACK_ENC_B, INPUT);

    attachInterrupt(digitalPinToInterrupt(LEFT_ENC), leftEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENC), rightEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(FRONT_ENC), frontEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(BACK_ENC), backEncoderISR, RISING);
}


double getLeftEncoder() {
    return leftEncoder*TICKS2DISTANCE;
}

double getRightEncoder() {
    return rightEncoder*TICKS2DISTANCE;
}

double getFrontEncoder() {
    return frontEncoder*TICKS2DISTANCE;
}

double getBackEncoder() {
    return backEncoder*TICKS2DISTANCE;
}
