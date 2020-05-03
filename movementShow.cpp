#include "movementShow.h"
#include "gyroscope.h"
#include "ledHeart.h"
#include "Arduino.h"

MovementShow::MovementShow(LedHeart *ledHeart, Gyroscope *gyroscope, float changeLightsThreshold = 12000.0) {
  this -> accelerationBrightnessMapper = new LedBrightnessAccelerationRatioMapper(35, 10);
  this -> currentAcceleration = AccelerationMeasurementVector::defaultVector();
  this -> formerAcceleration = AccelerationMeasurementVector::defaultVector();
  this -> ledHeart = ledHeart;
  this -> gyroscope = gyroscope;
  this -> changeLightsThreshold = changeLightsThreshold;
}

void MovementShow::initialize() {
  this -> gyroscope -> wakeUp();
  currentAcceleration = this -> gyroscope -> measureAcceleration();
  formerAcceleration = this -> gyroscope -> measureAcceleration();
}

void MovementShow::executeIteration() {
  currentAcceleration = gyroscope -> measureAcceleration();
  AccerlationVectorDifference vectorDifference = currentAcceleration.euclideanDistanceTo2(formerAcceleration);
  ledHeart -> toggleBrightness(vectorDifference.mapAccelerationRatioTo(accelerationBrightnessMapper));

  if (vectorDifference.overThreshold(changeLightsThreshold)) {
    ledHeart -> turnOnRandomly(4);
  }

  delay(10);
  formerAcceleration = currentAcceleration;
}
