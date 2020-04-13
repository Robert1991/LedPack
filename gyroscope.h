#ifndef gyroscope_h
#define gyroscope_h

#include "Arduino.h"
#include "Wire.h"

const float MAX_ACCELERATION_VECTOR_DIFFERENCE = 65536.0;

class IAccelerationRatioMapper
{
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
};

class AccelerationMeasurementVector {
  public:
    int accX;
    int accY;
    int accZ;

    static AccelerationMeasurementVector defaultVector();

    AccelerationMeasurementVector(int accX, int accY, int accZ);

    float euclideanDistanceTo(AccelerationMeasurementVector otherVector);

    AccerlationVectorDifference euclideanDistanceTo2(AccelerationMeasurementVector otherVector);

    void printOut();
};

class Gyroscope {
  private:
    int mpuAdress;
    void requestMeasurement();

  public:
    Gyroscope(int mpuAdress);

    AccelerationMeasurementVector measureAcceleration();

    void wakeUp();
};

#endif
