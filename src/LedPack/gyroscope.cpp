#include "gyroscope.h"

AccelerationMeasurementVector::AccelerationMeasurementVector(int accX, int accY, int accZ) {
  this->accX = accX;
  this->accY = accY;
  this->accZ = accZ;
}

AccelerationMeasurementVector::AccelerationMeasurementVector() {}

AccelerationMeasurementVector AccelerationMeasurementVector::defaultVector() { return AccelerationMeasurementVector(); }

AccerlationVectorDifference AccelerationMeasurementVector::euclideanDistanceTo(AccelerationMeasurementVector otherVector) {
  float euclideanDistance = sqrt(pow(accX - otherVector.accX, 2) + pow(accY - otherVector.accY, 2) + pow(accZ - otherVector.accZ, 2));
  float accRatio = euclideanDistance / MAX_ACCELERATION_VECTOR_DIFFERENCE;
  return AccerlationVectorDifference(euclideanDistance, accRatio);
}

bool operator==(const AccelerationMeasurementVector &lhs, const AccelerationMeasurementVector &rhs) {
  return lhs.accX == rhs.accX && lhs.accY == rhs.accY && lhs.accZ == rhs.accZ;
}

bool operator!=(const AccelerationMeasurementVector &lhs, const AccelerationMeasurementVector &rhs) { return !(lhs == rhs); }

AccerlationVectorDifference::AccerlationVectorDifference(float rawDifference, float differenceRatio) {
  this->rawDifference = rawDifference;
  this->differenceRatio = differenceRatio;
}

int AccerlationVectorDifference::mapAccelerationRatioTo(IAccelerationRatioMapper *mapper) { return mapper->mapToInt((this->differenceRatio)); }

bool AccerlationVectorDifference::overThreshold(float rawValue) { return (this->rawDifference) > rawValue; }

bool operator==(const AccerlationVectorDifference &lhs, const AccerlationVectorDifference &rhs) {
  return (fabs(lhs.differenceRatio - rhs.differenceRatio) < 0.005f) && (fabs(lhs.rawDifference - rhs.rawDifference) < 0.005f);
}

bool operator==(const AccerlationVectorDifference *lhs, const AccerlationVectorDifference &rhs) {
  return (fabs(lhs->differenceRatio - rhs.differenceRatio) < 0.005f) && (fabs(lhs->rawDifference - rhs.rawDifference) < 0.005f);
}

bool operator!=(const AccerlationVectorDifference &lhs, const AccerlationVectorDifference &rhs) { return !(lhs == rhs); }

bool operator!=(const AccerlationVectorDifference *lhs, const AccerlationVectorDifference &rhs) { return !(lhs == rhs); }

Gyroscope::Gyroscope(GyroscopeWire *wire) { this->wire = wire; }

Gyroscope::Gyroscope() {}

AccelerationMeasurementVector Gyroscope::measureAcceleration() {
  requestMeasurement();
  int accX = wire->readNextRegister();
  int accY = wire->readNextRegister();
  int accZ = wire->readNextRegister();

  return AccelerationMeasurementVector(accX, accY, accZ);
}

void Gyroscope::wakeUp() { wire->initialize(); }

void Gyroscope::requestMeasurement() { wire->requestMeasurement(); }
