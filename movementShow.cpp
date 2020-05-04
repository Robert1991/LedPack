#include "movementShow.h"

MovementShow::MovementShow(LedHeart *ledHeart, Gyroscope *gyroscope, IArduinoWrapper *wrapper, float changeLightsThreshold = 12000.0) {
  this -> accelerationBrightnessMapper = new LedBrightnessAccelerationRatioMapper(35, 10);
  this -> currentAcceleration = AccelerationMeasurementVector::defaultVector();
  this -> formerAcceleration = AccelerationMeasurementVector::defaultVector();
  this -> ledHeart = ledHeart;
  this -> gyroscope = gyroscope;
  this -> arduinoEnv = wrapper;
  this -> changeLightsThreshold = changeLightsThreshold;
}

void MovementShow::initialize() {
  this -> gyroscope -> wakeUp();
  currentAcceleration = this -> gyroscope -> measureAcceleration();
  formerAcceleration = this -> gyroscope -> measureAcceleration();
}

void MovementShow::executeIteration() {
  currentAcceleration = gyroscope -> measureAcceleration();
  AccerlationVectorDifference vectorDifference = currentAcceleration.euclideanDistanceTo(formerAcceleration);
  ledHeart -> toggleBrightness(vectorDifference.mapAccelerationRatioTo(accelerationBrightnessMapper));

  if (vectorDifference.overThreshold(changeLightsThreshold)) {
    ledHeart -> turnOnRandomly(4);
  }

  arduinoEnv -> delayFor(10);
  formerAcceleration = currentAcceleration;
}
