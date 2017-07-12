#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055();

void initGyro() {
    bno.begin(Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    bno.setExtCrystalUse(true);

    uint8_t system, gyro, accel, mag = 0;
    while ( gyro < 3 )
        bno.getCalibration(&system, &gyro, &accel, &mag);
}

float getHeading() {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    return euler.x();
}