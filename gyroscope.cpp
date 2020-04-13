#include "gyroscope.h"
#include "Arduino.h"

AccelerationMeasurementVector::AccelerationMeasurementVector(int accX, int accY, int accZ) {
  this -> accX = accX;
  this -> accY = accY;
  this -> accZ = accZ;
}

static AccelerationMeasurementVector AccelerationMeasurementVector::defaultVector() {
  return AccelerationMeasurementVector(0, 0, 0);
}

float AccelerationMeasurementVector::euclideanDistanceTo(AccelerationMeasurementVector otherVector) {
  return sqrt(pow(this -> accX - otherVector.accX, 2) + pow(this -> accY - otherVector.accY, 2) + pow(this -> accZ - otherVector.accZ, 2));
}

AccerlationVectorDifference AccelerationMeasurementVector::euclideanDistanceTo2(AccelerationMeasurementVector otherVector) {
  float euclideanDistance = sqrt(pow(this -> accX - otherVector.accX, 2) + pow(this -> accY - otherVector.accY, 2) + pow(this -> accZ - otherVector.accZ, 2));
  float accRatio = euclideanDistance / MAX_ACCELERATION_VECTOR_DIFFERENCE;
  return AccerlationVectorDifference(euclideanDistance, accRatio);
}

void AccelerationMeasurementVector::printOut() {
  Serial.print("X = "); Serial.print(accX);
  Serial.print(" | Y = "); Serial.print(accY);
  Serial.print(" | Z = "); Serial.println(accZ);
}

AccerlationVectorDifference::AccerlationVectorDifference(float rawDifference, float differenceRatio) {
  this -> rawDifference = rawDifference;
  this -> differenceRatio = differenceRatio;
}

int AccerlationVectorDifference::mapAccelerationRatioTo(IAccelerationRatioMapper *mapper) {
  return mapper -> mapToInt((this -> differenceRatio));
}

bool AccerlationVectorDifference::overThreshold(float rawValue) {
  return (this -> rawDifference) > rawValue;
}

Gyroscope::Gyroscope(int mpuAdress) {
  this -> mpuAdress = mpuAdress;
}

AccelerationMeasurementVector Gyroscope::measureAcceleration() {
  requestMeasurement();

  int accX; int accY; int accZ;
  accX = Wire.read() << 8 | Wire.read();
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();

  return AccelerationMeasurementVector(accX, accY, accZ);
}

void Gyroscope::wakeUp() {
  Wire.begin();
  Wire.beginTransmission(mpuAdress); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void Gyroscope::requestMeasurement() {
  Wire.beginTransmission(mpuAdress);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(mpuAdress, 12, true); // request a total of 7*2=14 registers
}
