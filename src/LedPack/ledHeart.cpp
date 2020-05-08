#include "ledHeart.h"
#include "shiftregister.h"

void LeftRegisterLevelActivator::turnOnLevel(int level, Led *leds) {
  if (level == 1) {
    leds[0].turnOn();
  } else if (level == 2) {
    leds[1].turnOn();
  } else if (level == 3) {
    leds[2].turnOn();
  } else if (level == 4) {
    leds[3].turnOn();
  } else if (level == 5) {
    leds[4].turnOn();
    leds[6].turnOn();
  } else if (level == 6) {
    leds[5].turnOn();
  }
}

void LeftRegisterLevelActivator::turnOffLevel(int level, Led *leds) {
  if (level == 1) {
    leds[0].turnOff();
  } else if (level == 2) {
    leds[1].turnOff();
  } else if (level == 3) {
    leds[2].turnOff();
  } else if (level == 4) {
    leds[3].turnOff();
  } else if (level == 5) {
    leds[4].turnOff();
    leds[6].turnOff();
  } else if (level == 6) {
    leds[5].turnOff();
  }
}
void RightRegisterLevelActivator::turnOnLevel(int level, Led *leds) {
  if (level == 1) {
    // there is no level 1 for this
  } else if (level == 2) {
    leds[6].turnOn();
  } else if (level == 3) {
    leds[5].turnOn();
  } else if (level == 4) {
    leds[0].turnOn();
    leds[4].turnOn();
  } else if (level == 5) {
    leds[1].turnOn();
    leds[3].turnOn();
  } else if (level == 6){
    leds[2].turnOn();
  }
}
void RightRegisterLevelActivator::turnOffLevel(int level, Led *leds) {
  if (level == 1) {
    // there is no level 1 for this
  } else if (level == 2) {
    leds[6].turnOff();
  } else if (level == 3) {
    leds[5].turnOff();
  } else if (level == 4) {
    leds[0].turnOff();
    leds[4].turnOff();
  } else if (level == 5) {
    leds[1].turnOff();
    leds[3].turnOff();
  } else if (level == 6){
    leds[2].turnOff();
  }
}
void LeftRegisterColumnActivator::turnOnColumn(int column, Led *leds) {
  if (column == 1) {
    leds[4].turnOn();
  } else if (column == 2) {
    leds[3].turnOn();
    leds[2].turnOn();
    leds[5].turnOn();
  } else if (column == 3) {
    leds[1].turnOn();
    leds[6].turnOn();
  } else if (column == 4) {
    leds[0].turnOn();
  }
}

void LeftRegisterColumnActivator::turnOffColumn(int column, Led *leds) {
  if (column == 1) {
    leds[4].turnOff();
  } else if (column == 2) {
    leds[3].turnOff();
    leds[2].turnOff();
    leds[5].turnOff();
  } else if (column == 3) {
    leds[1].turnOff();
    leds[6].turnOff();
  } else if (column == 4){
    leds[0].turnOff();
  }
}

void RightRegisterColumnActivator::turnOnColumn(int column, Led *leds) {
  if (column == 1) {
    leds[3].turnOn();
  } else if (column == 2) {
    leds[2].turnOn();
    leds[4].turnOn();
    leds[5].turnOn();
  } else if (column == 3) {
    leds[1].turnOn();
    leds[6].turnOn();
  } else if (column == 4){
    leds[0].turnOn();
  }
}

void RightRegisterColumnActivator::turnOffColumn(int column, Led *leds) {
  if (column == 1) {
    leds[3].turnOff();
  } else if (column == 2) {
    leds[2].turnOff();
    leds[4].turnOff();
    leds[5].turnOff();
  } else if (column == 3) {
    leds[1].turnOff();
    leds[6].turnOff();
  } else if (column == 4){
    leds[0].turnOff();
  }
}

LedHeart::LedHeart() {
  
}

LedHeart::LedHeart(IStandardFunctions *stdFunctions, LedShiftRegister *leftShiftRegister, LedShiftRegister *rightShiftRegister) {
  this -> stdFunctions = stdFunctions;
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

void LedHeart::turnOn(int ledIndex) {
  if (ledIndex >= 0 && ledIndex < HEART_LED_COUNT) {
    if (ledIndex < HEART_LED_COUNT/2) {
      leftShiftRegister -> turnOn(ledIndex);
    } else {
      rightShiftRegister -> turnOn(ledIndex - HEART_LED_COUNT/2);
    }
  }
}

void LedHeart::turnOff(int ledIndex) {
  if (ledIndex >= 0 && ledIndex < 14) {
    if (ledIndex < 7) {
      leftShiftRegister -> turnOff(ledIndex);
    } else {
      rightShiftRegister -> turnOff(ledIndex - 7);
    }
  }
}

void LedHeart::turnOnRandomly(int minLedsTurnedOn) {
  if (minLedsTurnedOn > 0 && minLedsTurnedOn <= HEART_LED_COUNT) {
    int numberOfLedsTurnedOn = stdFunctions -> nextRandomIntInBounds(minLedsTurnedOn, HEART_LED_COUNT);
    int turnedOnLeds[numberOfLedsTurnedOn];

    for (int i = 0; i < numberOfLedsTurnedOn; i++) {
      int nextLed = stdFunctions -> nextRandomIntInBounds(0, 14);
      while (alreadyTurnedOn(turnedOnLeds, numberOfLedsTurnedOn, nextLed)) {
        nextLed = stdFunctions -> nextRandomIntInBounds(0, 14);
      }
      turnedOnLeds[i] = nextLed;
    }

    this -> leftShiftRegister -> turnOffAll();
    this -> rightShiftRegister -> turnOffAll();

    for (int i = 0; i < numberOfLedsTurnedOn; i++) {
      int turnedOnLedIndex = turnedOnLeds[i];

      if (turnedOnLedIndex >= 7) {
        this -> rightShiftRegister -> turnOn(turnedOnLedIndex - 7);
      } else {
        this -> leftShiftRegister -> turnOn(turnedOnLedIndex);
      }
    }
  }
}

void LedHeart::turnLevelOn(int level) {
  if (level >= 1 && level <= HEART_LEVEL_COUNT) {
    leftShiftRegister -> turnOnLevel(level);
    rightShiftRegister -> turnOnLevel(level);
  }
}

void LedHeart::turnLevelOff(int level) {
  if (level >= 1 && level <= HEART_LEVEL_COUNT) {
    leftShiftRegister -> turnOffLevel(level);
    rightShiftRegister -> turnOffLevel(level);
  }
}

void LedHeart::turnColumnOn(int column) {
  if (column >= 1 && column <= HEART_COLUMN_COUNT) {
    if (column <= 3) {
      rightShiftRegister -> turnOnColumn(column);
    } else if (column == 4) {
      rightShiftRegister -> turnOnColumn(column);
      leftShiftRegister -> turnOnColumn(column);
    } else if (column > 4) {
      leftShiftRegister -> turnOnColumn(HEART_COLUMN_COUNT + 1 - column);
    }
  }
}


void LedHeart::turnColumnOff(int column) {
  if (column >= 1 && column <= HEART_COLUMN_COUNT) {
    if (column <= 3) {
      rightShiftRegister -> turnOffColumn(column);
    } else if (column == 4) {
      rightShiftRegister -> turnOffColumn(column);
      leftShiftRegister -> turnOffColumn(column);
    } else if (column > 4) {
      leftShiftRegister -> turnOffColumn(HEART_COLUMN_COUNT + 1 - column);
    }
  }
}

void LedHeart::toggleBrightness(int brightness) {
  if (brightness >= 0 && brightness <= HEART_MAX_BRIGHTNESS) {
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
