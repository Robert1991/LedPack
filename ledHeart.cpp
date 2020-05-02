#include "ledHeart.h"
#include "shiftregister.h"

LedHeart::LedHeart(LedShiftRegister *leftShiftRegister, LedShiftRegister *rightShiftRegister) {
  this -> leftShiftRegister = leftShiftRegister;
  this -> rightShiftRegister = rightShiftRegister;
}

void LedHeart::initialize() {
  this -> leftShiftRegister -> initializePins();
  this -> rightShiftRegister -> initializePins();
}

void LedHeart::turnOnAll() {
  this -> leftShiftRegister -> turnOnAll();
  this -> rightShiftRegister -> turnOnAll();
}

void LedHeart::turnOffAll() {
  this -> leftShiftRegister -> turnOffAll();
  this -> rightShiftRegister -> turnOffAll();
}

void LedHeart::turnOnRandomly(int minLedsTurnedOn = 0) {
  if (minLedsTurnedOn <= HEART_LED_COUNT) {
    int numberOfLedsTurnedOn = minLedsTurnedOn + ( rand() % ( HEART_LED_COUNT - minLedsTurnedOn + 1 ) );
    int turnedOnLeds[numberOfLedsTurnedOn];

    for (int i = 0; i < numberOfLedsTurnedOn; i++) {
      int nextLed = rand() % 14;
      while (alreadyTurnedOn(turnedOnLeds, numberOfLedsTurnedOn, nextLed)) {
        nextLed = rand() % 14;
      }
      turnedOnLeds[i] = nextLed;
    }

    this -> leftShiftRegister -> turnOffAll();
    this -> rightShiftRegister -> turnOffAll();

    for (int i = 0; i < sizeof(turnedOnLeds); i++) {
      int turnedOnLedIndex = turnedOnLeds[i];

      if (turnedOnLedIndex >= 7) {
        this -> rightShiftRegister -> turnOn(turnedOnLedIndex - 7);
      } else {
        this -> leftShiftRegister -> turnOn(turnedOnLedIndex);
      }
    }
  }
}

void LedHeart::toggleBrightness(int brightness) {
  if (brightness >= 0 && brightness < 256) {
    this -> leftShiftRegister -> toggleBrightness(brightness);
    this -> rightShiftRegister -> toggleBrightness(brightness);
  }
}


bool LedHeart::alreadyTurnedOn(int *turnedOn, int arrayLength, int ledIndex) {
  for (int i = 0; i < arrayLength; i++) {
    if (turnedOn[i] == ledIndex) {
      return true;
    }
  }

  return false;
}
