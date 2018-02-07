#include <Arduino.h>
#include "ports.h"
#include "motors.h"
#include "encoders.h"
#include "Rate.h"
#include "SoftWatchdog.h"
#include <Wire.h>

#include <ros.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/Twist.h>
#include <omnibot_base/DriveSensors.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

SoftWatchdog motorKeepalive(500);

void handleDrivetrainUpdate(const geometry_msgs::Twist& velocity) {
  double robotRadius = 0.1;
  setMotors(
      velocity.linear.x - (velocity.angular.z * robotRadius),
      -velocity.linear.x - (velocity.angular.z * robotRadius),
      -velocity.linear.y - (velocity.angular.z * robotRadius),
      velocity.linear.y - (velocity.angular.z * robotRadius)
  );
  motorKeepalive.feed();
  digitalWrite(13, LOW);
}


void robotMain() {
  Adafruit_BNO055 bno = Adafruit_BNO055();
  {
    bno.begin(Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    uint8_t system, gyro, accel, mag;
    while ( gyro < 3 )
      bno.getCalibration(&system, &gyro, &accel, &mag);
  }

  pinMode(LOOPMONITOR, OUTPUT);

  initMotors();
  initEncoders();
  setMotors(0, 0, 0, 0);

  ros::NodeHandle nh;
  nh.getHardware()->setBaud(256000);
  nh.initNode();

  omnibot_base::DriveSensors sensors;
  ros::Publisher sensorPub("/drivetrain/sensors", &sensors);
  nh.advertise(sensorPub);

  sensor_msgs::Imu imu;
  ros::Publisher imuPub("/imu", &imu);
  nh.advertise(imuPub);

  ros::Subscriber<geometry_msgs::Twist> velocitySub("/cmd_vel", handleDrivetrainUpdate);
  nh.subscribe(velocitySub);

  Rate r(100);

  pinMode(13, OUTPUT);

  sensors.header.frame_id = "base_link";
  imu.header.frame_id = "base_link";

  float imuCov[] = 
  {
      1e-5, 0, 0,
      0, 1e-5, 0,
      0, 0, 1e-5 
  };

  for ( int i = 0; i < 9; i++ ) {
    imu.orientation_covariance[i] = imuCov[i]; 
    imu.angular_velocity_covariance[i] = imuCov[i]; 
    imu.linear_acceleration_covariance[i] = imuCov[i];
  }

  while(1) {
    digitalWrite(LOOPMONITOR, HIGH);

    sensors.header.stamp = nh.now();
    sensors.left = getLeftEncoder();
    sensors.right = getRightEncoder();
    sensors.front = getFrontEncoder();
    sensors.back = getBackEncoder();

    imu.header.stamp = nh.now();

    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    sensors.heading = (2*M_PI) - (euler.x() * (M_PI/180.0));

    sensorPub.publish(&sensors);


    imu::Quaternion imuQuat = bno.getQuat();
    imu::Vector<3> linearAccel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    imu::Vector<3> angularVel = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

    imu.orientation.x = imuQuat.x();
    imu.orientation.y = imuQuat.y();
    imu.orientation.z = imuQuat.z();
    imu.orientation.w = imuQuat.w();

    imu.angular_velocity.x = angularVel.x();
    imu.angular_velocity.y = angularVel.y();
    imu.angular_velocity.z = angularVel.z();

    imu.linear_acceleration.x = linearAccel.x();
    imu.linear_acceleration.y = linearAccel.y();
    imu.linear_acceleration.z = linearAccel.z();

    imuPub.publish(&imu);

    digitalWrite(LOOPMONITOR, LOW);

    if ( motorKeepalive.hungry() ) {
      setMotors(0, 0, 0, 0);
      digitalWrite(13, HIGH);
    }
    
    nh.spinOnce();
    r.sleep();

  }
}
