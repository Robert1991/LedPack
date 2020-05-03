#include "lightShow.h"
#include "ledHeart.h"

LightShow::LightShow(LedHeart *ledHeart) {
  this -> ledHeart = ledHeart;
}

void LightShow::execute() {
  turnOnAllLedsOnHeartInARow(50);
  letLedsOnHeartBlinkFor(6, 500, 100);
  
  for (int j = 0; j < 4; j++) {
    letLedsTwistRight(50, false);
    letLedsTwistLeft(50, false);
  }

  for (int i = 0; i < 3; i++) {
    turnOnLevelsFromBottomToTop(100);
    turnOffLevelsFromBottomToTop(100);
  }
  
  for (int times = 0; times < 3; times++) {
    letLedsTwistRight(50);
  }
  
  for (int times = 0; times < 3; times++) {
    letLedsTwistLeft(50);
  }
  
  for (int times = 0; times < 5; times++) {
    turnOnLevelsFromBottomToTop(100, true);
  }
  
  turnOnLevelsFromBottomToTop(125);
  
  delay(250);
  
  turnOffLevelsFromTopToBottom(50);
  
  for (int times = 0; times < 5; times++) {
    turnOnColumnsOutsideToInside(75, true);
  }
  
  for (int j = 0; j < 5; j++) {
    turnOnColumnsOutsideToInside(75, false, true);
  }
  
  turnOnColumnsOutsideToInside(200);
  
  delay(500);
  
  turnOffColumnsInsideToOutside(125);
  turnOnColumnsFromRightToLeft(75, true);
  turnOnColumnsFromLeftToRight(75, true);
}

void LightShow::turnOnAllLedsOnHeartInARow(int timeout) {
  this -> ledHeart -> turnOffAll();
  for (int led = 0; led < HEART_LED_COUNT; led++) {
    ledHeart -> turnOn(led);
    delay(timeout);
  }
}

void LightShow::letLedsOnHeartBlinkFor(int times, int onTimeout, int offTimeout) {
  for (int i = 0; i < times; i++) {
    this -> ledHeart -> turnOnAll();
    delay(onTimeout);
    this -> ledHeart -> turnOffAll();
    delay(offTimeout);
  }
}


void LightShow::letLedsTwistRight(int timeout, bool turnOffFormerLed = true) {
  for (int led = 0; led < 7; led++) {
    if (led >= 1 && turnOffFormerLed) {
      ledHeart -> turnOff(led - 1);
      ledHeart -> turnOff(led + 7 - 1);
    }

    ledHeart -> turnOn(led);
    ledHeart -> turnOn(led + 7);

    delay(timeout);
  }

  ledHeart -> turnOffAll();
}

void LightShow::letLedsTwistLeft(int timeout, bool turnOffFormerLed = true) {
  for (int led = 7; led >= 1; led--) {
    if (led < 7 && turnOffFormerLed) {
      ledHeart -> turnOff(led);
      ledHeart -> turnOff(led + 7);
    }

    ledHeart -> turnOn(led - 1);
    ledHeart -> turnOn(led + 7 - 1);
    delay(timeout);
  }

  ledHeart -> turnOffAll();
}

void LightShow::turnOnLevelsFromBottomToTop(int timeout, boolean turnOffFormerLevel = false) {
  for (int level = 1; level <= HEART_LEVEL_COUNT; level++) {
    if (level > 1 && turnOffFormerLevel) {
      ledHeart -> turnLevelOff(level - 1);
    }
    
    ledHeart -> turnLevelOn(level);
    delay(timeout);
  }

  if (turnOffFormerLevel) {
    ledHeart -> turnOffAll();
  }
}

void LightShow::turnOffLevelsFromBottomToTop(int timeout) {
  for (int level = 1; level <= HEART_LEVEL_COUNT; level++) {
    ledHeart -> turnLevelOff(level);
    delay(timeout);
  }
}

void LightShow::turnOffLevelsFromTopToBottom(int timeout) {
  for (int i = HEART_LEVEL_COUNT; i >= 1; i--) {
    ledHeart -> turnLevelOff(i);
    delay(timeout);
  }
}


void LightShow::turnOnColumnsOutsideToInside(int timeout, bool turnOffFormerColumn = false, bool turnOffAfterwards = false) {
  for (int column = 1; column <= 4; column++) {
    if (column > 1 && turnOffFormerColumn) {
      ledHeart -> turnColumnOff(column - 1);
      ledHeart -> turnColumnOff(HEART_COLUMN_COUNT + 1 - (column - 1));
    }

    ledHeart -> turnColumnOn(column);
    ledHeart -> turnColumnOn(HEART_COLUMN_COUNT + 1 - column);
    delay(timeout);
  }

  if (turnOffAfterwards || turnOffFormerColumn) {
    ledHeart -> turnOffAll();  
  }
}

void LightShow::turnOffColumnsInsideToOutside(int timeout) {
  for (int column = 4; column > 0; column--) {
    if (column == 4) {
      ledHeart -> turnColumnOff(column);
    } else {
      ledHeart -> turnColumnOff(column);
      ledHeart -> turnColumnOff(HEART_COLUMN_COUNT + 1 - column);
    }

    delay(timeout);
  }
}

void LightShow::turnOnColumnsFromRightToLeft(int timeout, bool turnOffFormerColumn = false) {
  for (int column = 1; column <= HEART_COLUMN_COUNT; column++) {
    if (column > 1 && turnOffFormerColumn) {
      ledHeart -> turnColumnOff(column - 1);
    }
    ledHeart -> turnColumnOn(column);
    delay(timeout);
  }

  if (turnOffFormerColumn) {
    ledHeart -> turnOffAll();  
  }
}

void LightShow::turnOnColumnsFromLeftToRight(int timeout, bool turnOffFormerColumn = false) {
  for (int column = HEART_COLUMN_COUNT; column > 0; column--) {
    if (column < HEART_COLUMN_COUNT && turnOffFormerColumn) {
      ledHeart -> turnColumnOff(column + 1);
    }
    ledHeart -> turnColumnOn(column);
    delay(timeout);
  }

  if (turnOffFormerColumn) {
    ledHeart -> turnOffAll();  
  }
}
