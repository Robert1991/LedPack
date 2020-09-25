#include "ledHeart.h"

#include "shiftregister.h"

// Level Activator
void LeftRegisterLevelActivator::turnOnLevel(int level, Led *leds) {
  void (Led::*ledFunctionPointer)() = &Led::turnOn;
  doForLevel(level, leds, ledFunctionPointer);
}

void LeftRegisterLevelActivator::turnOffLevel(int level, Led *leds) {
  void (Led::*ledFunctionPointer)() = &Led::turnOff;
  doForLevel(level, leds, ledFunctionPointer);
}

void LeftRegisterLevelActivator::doForLevel(int level, Led *leds, void (Led::*ledFunctionPointer)()) {
  if (level == 1) {
    (leds[0].*ledFunctionPointer)();
  } else if (level == 2) {
    (leds[1].*ledFunctionPointer)();
  } else if (level == 3) {
    (leds[2].*ledFunctionPointer)();
  } else if (level == 4) {
    (leds[3].*ledFunctionPointer)();
  } else if (level == 5) {
    (leds[4].*ledFunctionPointer)();
    (leds[6].*ledFunctionPointer)();
  } else if (level == 6) {
    (leds[5].*ledFunctionPointer)();
  }
}

void RightRegisterLevelActivator::turnOnLevel(int level, Led *leds) {
  void (Led::*ledFunctionPointer)() = &Led::turnOn;
  doForLevel(level, leds, ledFunctionPointer);
}
void RightRegisterLevelActivator::turnOffLevel(int level, Led *leds) {
  void (Led::*ledFunctionPointer)() = &Led::turnOff;
  doForLevel(level, leds, ledFunctionPointer);
}

void RightRegisterLevelActivator::doForLevel(int level, Led *leds, void (Led::*ledFunctionPointer)()) {
  if (level == 1) {
    // there is no level 1 for this
  } else if (level == 2) {
    (leds[6].*ledFunctionPointer)();
  } else if (level == 3) {
    (leds[5].*ledFunctionPointer)();
  } else if (level == 4) {
    (leds[0].*ledFunctionPointer)();
    (leds[4].*ledFunctionPointer)();
  } else if (level == 5) {
    (leds[1].*ledFunctionPointer)();
    (leds[3].*ledFunctionPointer)();
  } else if (level == 6) {
    (leds[2].*ledFunctionPointer)();
  }
}

// Column Activator
void LeftRegisterColumnActivator::turnOnColumn(int column, Led *leds) {
  void (Led::*ledFunctionPointer)() = &Led::turnOn;
  doForColumn(column, leds, ledFunctionPointer);
}

void LeftRegisterColumnActivator::turnOffColumn(int column, Led *leds) {
  void (Led::*ledFunctionPointer)() = &Led::turnOff;
  doForColumn(column, leds, ledFunctionPointer);
}

void LeftRegisterColumnActivator::doForColumn(int column, Led *leds, void (Led::*ledFunctionPointer)()) {
  if (column == 1) {
    (leds[4].*ledFunctionPointer)();
  } else if (column == 2) {
    (leds[3].*ledFunctionPointer)();
    (leds[2].*ledFunctionPointer)();
    (leds[5].*ledFunctionPointer)();
  } else if (column == 3) {
    (leds[1].*ledFunctionPointer)();
    (leds[6].*ledFunctionPointer)();
  } else if (column == 4) {
    (leds[0].*ledFunctionPointer)();
  }
}

void RightRegisterColumnActivator::turnOnColumn(int column, Led *leds) {
  void (Led::*ledFunctionPointer)() = &Led::turnOn;
  doForColumn(column, leds, ledFunctionPointer);
}

void RightRegisterColumnActivator::turnOffColumn(int column, Led *leds) {
  void (Led::*ledFunctionPointer)() = &Led::turnOff;
  doForColumn(column, leds, ledFunctionPointer);
}

void RightRegisterColumnActivator::doForColumn(int column, Led *leds, void (Led::*ledFunctionPointer)()) {
  if (column == 1) {
    (leds[3].*ledFunctionPointer)();
  } else if (column == 2) {
    (leds[2].*ledFunctionPointer)();
    (leds[4].*ledFunctionPointer)();
    (leds[5].*ledFunctionPointer)();
  } else if (column == 3) {
    (leds[1].*ledFunctionPointer)();
    (leds[6].*ledFunctionPointer)();
  } else if (column == 4) {
    (leds[0].*ledFunctionPointer)();
  }
}

// Led Heart
LedHeart::LedHeart() {}

LedHeart::LedHeart(IStandardFunctions *stdFunctions, LedShiftRegister *leftShiftRegister, LedShiftRegister *rightShiftRegister) {
  this->stdFunctions = stdFunctions;
  this->leftShiftRegister = leftShiftRegister;
  this->rightShiftRegister = rightShiftRegister;
}

void LedHeart::initialize() {
  leftShiftRegister->initializePins();
  rightShiftRegister->initializePins();
}

void LedHeart::turnOnAll() {
  leftShiftRegister->turnOnAll();
  rightShiftRegister->turnOnAll();
}

void LedHeart::turnOffAll() {
  leftShiftRegister->turnOffAll();
  rightShiftRegister->turnOffAll();
}

void LedHeart::turnOn(int ledIndex) {
  void (LedShiftRegister::*ledShiftRegisterTurnLedOn)(int) = &LedShiftRegister::turnOn;
  doForLed(ledIndex, ledShiftRegisterTurnLedOn);
}

void LedHeart::turnOff(int ledIndex) {
  void (LedShiftRegister::*ledShiftRegisterTurnLedOff)(int) = &LedShiftRegister::turnOff;
  doForLed(ledIndex, ledShiftRegisterTurnLedOff);
}

void LedHeart::turnOnRandomly(int minLedsTurnedOn) {
  if (minLedsTurnedOn > 0 && minLedsTurnedOn <= HEART_LED_COUNT) {
    int numberOfLedsTurnedOn = stdFunctions->nextRandomIntInBounds(minLedsTurnedOn, HEART_LED_COUNT);
    int turnedOnLeds[numberOfLedsTurnedOn];

    for (int i = 0; i < numberOfLedsTurnedOn; i++) {
      int nextLed = stdFunctions->nextRandomIntInBounds(0, HEART_LED_COUNT);
      while (alreadyTurnedOn(turnedOnLeds, numberOfLedsTurnedOn, nextLed)) {
        nextLed = stdFunctions->nextRandomIntInBounds(0, HEART_LED_COUNT);
      }
      turnedOnLeds[i] = nextLed;
    }

    turnOffAll();
    for (int i = 0; i < numberOfLedsTurnedOn; i++) {
      turnOn(turnedOnLeds[i]);
    }
  }
}

void LedHeart::turnLevelOn(int level) {
  void (LedShiftRegister::*ledShiftRegisterTurnLevelOn)(int) = &LedShiftRegister::turnOnLevel;
  doForLevel(level, ledShiftRegisterTurnLevelOn);
}

void LedHeart::turnLevelOff(int level) {
  void (LedShiftRegister::*ledShiftRegisterTurnLevelOff)(int) = &LedShiftRegister::turnOffLevel;
  doForLevel(level, ledShiftRegisterTurnLevelOff);
}

void LedHeart::turnColumnOn(int column) {
  void (LedShiftRegister::*ledShiftRegisterTurnColumnOff)(int) = &LedShiftRegister::turnOnColumn;
  doForColumn(column, ledShiftRegisterTurnColumnOff);
}

void LedHeart::turnColumnOff(int column) {
  void (LedShiftRegister::*ledShiftRegisterTurnColumnOff)(int) = &LedShiftRegister::turnOffColumn;
  doForColumn(column, ledShiftRegisterTurnColumnOff);
}

void LedHeart::toggleBrightness(int brightness) {
  if (brightness >= 0 && brightness <= HEART_MAX_BRIGHTNESS) {
    leftShiftRegister->toggleBrightness(brightness);
    rightShiftRegister->toggleBrightness(brightness);
  }
}

void LedHeart::doForLevel(int level, void (LedShiftRegister::*shiftRegisterFunctionPointer)(int)) {
  if (level >= 1 && level <= HEART_LEVEL_COUNT) {
    (leftShiftRegister->*shiftRegisterFunctionPointer)(level);
    (rightShiftRegister->*shiftRegisterFunctionPointer)(level);
  }
}

void LedHeart::doForColumn(int column, void (LedShiftRegister::*shiftRegisterFunctionPointer)(int)) {
  if (column >= 1 && column <= HEART_COLUMN_COUNT) {
    if (column <= 3) {
      (rightShiftRegister->*shiftRegisterFunctionPointer)(column);
    } else if (column == 4) {
      (rightShiftRegister->*shiftRegisterFunctionPointer)(column);
      (leftShiftRegister->*shiftRegisterFunctionPointer)(column);
    } else if (column > 4) {
      (leftShiftRegister->*shiftRegisterFunctionPointer)(HEART_COLUMN_COUNT + 1 - column);
    }
  }
}

void LedHeart::doForLed(int ledIndex, void (LedShiftRegister::*shiftRegisterFunctionPointer)(int)) {
  if (ledIndex >= 0 && ledIndex < HEART_LED_COUNT) {
    if (ledIndex < HEART_LED_COUNT / 2) {
      (leftShiftRegister->*shiftRegisterFunctionPointer)(ledIndex);
    } else {
      (rightShiftRegister->*shiftRegisterFunctionPointer)(ledIndex - HEART_LED_COUNT / 2);
    }
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
