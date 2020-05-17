#include "movementShow.h"

LedBrightnessAccelerationRatioMapper::LedBrightnessAccelerationRatioMapper(int ledBrightnessMagnifier, int magnifierThreshold) {
  this -> ledBrightnessMagnifier = ledBrightnessMagnifier;
  this -> magnifierThreshold = magnifierThreshold;
}

int LedBrightnessAccelerationRatioMapper::mapToInt(float accelerationRatio) {
  int brightness =  accelerationRatio * MAX_BRIGHTNESS;

  if (brightness > magnifierThreshold) {
    brightness = brightness + ledBrightnessMagnifier;
  }

  if (brightness > MAX_BRIGHTNESS) {
    return MAX_BRIGHTNESS;
  } else {
    return brightness;
  }
}

MovementShowActionMapper::MovementShowActionMapper(LedHeart *heart, IAccelerationRatioMapper *ratioMapper) {
  this->ledHeart = heart;
  this->ratioMapper = ratioMapper;
}

RandomLedBlinkMovementShow::RandomLedBlinkMovementShow(LedHeart *heart, LedBrightnessAccelerationRatioMapper *ratioMapper, 
                                                       IArduinoWrapper *wrapper, 
                                                       float changeLightsThreshold) : MovementShowActionMapper(heart, ratioMapper) {
    this->arduinoEnv = wrapper;
    this->changeLightsThreshold = changeLightsThreshold;
}

void RandomLedBlinkMovementShow::mapToHeart(AccerlationVectorDifference *difference) {
  ledHeart -> toggleBrightness(difference -> mapAccelerationRatioTo(ratioMapper));

  if (difference -> overThreshold(changeLightsThreshold)) {
    ledHeart -> turnOnRandomly(4);
    arduinoEnv -> delayFor(10);
  }
}

MovementShow::MovementShow(Gyroscope *gyroscope) {
  this -> currentAcceleration = AccelerationMeasurementVector::defaultVector();
  this -> formerAcceleration = AccelerationMeasurementVector::defaultVector();
  this -> gyroscope = gyroscope;
}

void MovementShow::initialize() {
  this -> gyroscope -> wakeUp();
  currentAcceleration = this -> gyroscope -> measureAcceleration();
  formerAcceleration = this -> gyroscope -> measureAcceleration();
}

void MovementShow::executeIterationWith(MovementShowActionMapper *ledHeartActionMapper) {
  currentAcceleration = gyroscope -> measureAcceleration();
  AccerlationVectorDifference vectorDifference = formerAcceleration.euclideanDistanceTo(currentAcceleration);
  ledHeartActionMapper -> mapToHeart(&vectorDifference);
  formerAcceleration = currentAcceleration;
}