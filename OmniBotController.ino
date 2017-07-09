#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055();

void robotMain();

void setup() {
   // bno.begin();
    Serial.begin(9600);
    robotMain();
}
void loop() {}
