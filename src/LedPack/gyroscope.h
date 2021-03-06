#ifndef gyroscope_h
#define gyroscope_h

#include <math.h>

#include "arduinoWrapper.h"

const float MAX_ACCELERATION_VECTOR_DIFFERENCE = 65536.0;

class IAccelerationRatioMapper {
 public:
  virtual ~IAccelerationRatioMapper() {}
  virtual int mapToInt(float accelerationRatio) = 0;
};

class AccerlationVectorDifference {
 private:
  float rawDifference;
  float differenceRatio;

 public:
  AccerlationVectorDifference(float rawDifference, float differenceRatio);

  int mapAccelerationRatioTo(IAccelerationRatioMapper *mapper);
  bool overThreshold(float rawValue);

  friend bool operator==(const AccerlationVectorDifference &lhs, const AccerlationVectorDifference &rhs);
  friend bool operator==(const AccerlationVectorDifference *lhs, const AccerlationVectorDifference &rhs);
  friend bool operator!=(const AccerlationVectorDifference &lhs, const AccerlationVectorDifference &rhs);
  friend bool operator!=(const AccerlationVectorDifference *lhs, const AccerlationVectorDifference &rhs);
  friend bool operator!=(const AccerlationVectorDifference &lhs, const AccerlationVectorDifference &rhs);
};

class AccelerationMeasurementVector {
 public:
  int accX = 0;
  int accY = 0;
  int accZ = 0;

  static AccelerationMeasurementVector defaultVector();

  AccelerationMeasurementVector();
  AccelerationMeasurementVector(int accX, int accY, int accZ);

  AccerlationVectorDifference euclideanDistanceTo(AccelerationMeasurementVector otherVector);

  friend bool operator==(const AccelerationMeasurementVector &lhs, const AccelerationMeasurementVector &rhs);
  friend bool operator!=(const AccelerationMeasurementVector &lhs, const AccelerationMeasurementVector &rhs);
};

class Gyroscope {
 private:
  GyroscopeWire *wire;

  void requestMeasurement();

 public:
  Gyroscope();
  Gyroscope(GyroscopeWire *wire);

  virtual AccelerationMeasurementVector measureAcceleration();
  virtual void wakeUp();
};

#endif
