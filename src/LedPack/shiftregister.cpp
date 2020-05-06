#include "shiftregister.h"
#include "arduinoWrapper.h"

Led::Led() {
  this-> ledNumber = -1;
  this-> shiftRegisterPosition = -1;
}

Led::Led(int ledNum, int shiftRegisterPosition) {
  this-> ledNumber = ledNum;
  this-> shiftRegisterPosition = shiftRegisterPosition;
}

void Led::turnOn() {
  on = true;
}

void Led::turnOff() {
  on = false;
}

LevelActivator::LevelActivator() {}

ColumnActivator::ColumnActivator() {}

LedShiftRegister::LedShiftRegister(LedShiftRegisterPins pins, Led *ledsOnShiftRegister, LevelActivator *levelActivator, ColumnActivator *columnActivator, IArduinoWrapper *arduinoEnv) {
  this -> pins = pins;
  this -> shiftRegisterByte = 0;
  this -> levelActivator = levelActivator;
  this -> columnActivator = columnActivator;
  this -> arduinoEnv = arduinoEnv;
  for (int i = 0; i < MAX_SHIFT_REGISTER_LED_COUNT; i++) {
    this->leds[i] = ledsOnShiftRegister[i];
  }
}

void LedShiftRegister::updateShiftRegisterByte() {
  shiftRegisterByte = 0;

  for (int i = 0; i < MAX_SHIFT_REGISTER_LED_COUNT; i++) {
    Led led = leds[i];
    if (led.on) {
      arduinoEnv -> setBit(shiftRegisterByte, led.shiftRegisterPosition);
    } else {
      arduinoEnv -> clearBit(shiftRegisterByte, led.shiftRegisterPosition);
    }
  }
}

void LedShiftRegister::updateShiftRegister() {
  updateShiftRegisterByte();
  arduinoEnv -> writeLOWToDigital(pins.latchPin);
  arduinoEnv -> shift(pins.dataPin, pins.clockPin, shiftRegisterByte);
  arduinoEnv -> writeHIGHToDigital(pins.latchPin);
}

void LedShiftRegister::turnOn(int ledIndex) {
  if (ledIndex >= 0 && ledIndex < MAX_SHIFT_REGISTER_LED_COUNT) {
    leds[ledIndex].turnOn();
    updateShiftRegister();
  }
}

void LedShiftRegister::turnOff(int ledIndex) {
  if (ledIndex >= 0 && ledIndex < MAX_SHIFT_REGISTER_LED_COUNT) {
    leds[ledIndex].turnOff();
    updateShiftRegister();
  }
}

void LedShiftRegister::turnOffAll() {
  for (int i = 0; i < MAX_SHIFT_REGISTER_LED_COUNT; i++) {
    leds[i].turnOff();
  }

  updateShiftRegister();
}

void LedShiftRegister::turnOnAll() {
  for (int i = 0; i < MAX_SHIFT_REGISTER_LED_COUNT; i++) {
    leds[i].turnOn();
  }

  updateShiftRegister();
}

void LedShiftRegister::turnOnLevel(int level) {
  if (level > 0 && level <= MAX_LEVEL) {
    levelActivator-> turnOnLevel(level, leds);
    updateShiftRegister();
  }
}

void LedShiftRegister::turnOffLevel(int level) {
  if (level > 0 && level <= MAX_LEVEL) {
    levelActivator-> turnOffLevel(level, leds);
    updateShiftRegister();
  }
}

void LedShiftRegister::turnOnColumn(int column) {
  if (column > 0 && column <= MAX_COLUMNS) {
    columnActivator-> turnOnColumn(column, leds);
    updateShiftRegister();
  }
}

void LedShiftRegister::turnOffColumn(int column) {
  if (column > 0 && column <= MAX_COLUMNS) {
    columnActivator-> turnOffColumn(column, leds);
    updateShiftRegister();
  }
}

void LedShiftRegister::toggleBrightness(byte value) {
  if (value >= 0 && value < 256) {
    arduinoEnv -> writeToAnalog(pins.outputEnablePin, 255-value);
  }
}

void LedShiftRegister::initializePins() {
  arduinoEnv -> setPinToOutputMode(pins.latchPin);
  arduinoEnv -> setPinToOutputMode(pins.dataPin);
  arduinoEnv -> setPinToOutputMode(pins.clockPin);
  arduinoEnv -> setPinToOutputMode(pins.outputEnablePin);
}
