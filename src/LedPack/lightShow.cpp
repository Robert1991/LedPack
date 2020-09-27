#include "lightShow.h"

LightShow::LightShow(IArduinoWrapper* arduinoEnv, LedHeart* ledHeart) {
  this->ledHeart = ledHeart;
  this->arduinoEnv = arduinoEnv;
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

  arduinoEnv->delayFor(250);

  turnOffLevelsFromTopToBottom(50);

  for (int times = 0; times < 5; times++) {
    turnOnColumnsOutsideToInside(75, true);
  }

  for (int j = 0; j < 5; j++) {
    turnOnColumnsOutsideToInside(75, false, true);
  }

  turnOnColumnsOutsideToInside(200);

  arduinoEnv->delayFor(500);

  turnOffColumnsInsideToOutside(125);
  turnOnColumnsFromRightToLeft(75, true);
  turnOnColumnsFromLeftToRight(75, true);
}

void LightShow::turnOnAllLedsOnHeartInARow(int timeout) {
  this->ledHeart->turnOffAll();
  for (int led = 0; led < HEART_LED_COUNT; led++) {
    ledHeart->turnOn(led);
    arduinoEnv->delayFor(timeout);
  }
}

void LightShow::letLedsOnHeartBlinkFor(int times, int onTimeout, int offTimeout) {
  for (int i = 0; i < times; i++) {
    this->ledHeart->turnOnAll();
    arduinoEnv->delayFor(onTimeout);
    this->ledHeart->turnOffAll();
    arduinoEnv->delayFor(offTimeout);
  }
}

void LightShow::letLedsTwistRight(int timeout, bool turnOffFormerLed) {
  for (int led = 0; led < 7; led++) {
    if (led >= 1 && turnOffFormerLed) {
      ledHeart->turnOff(led - 1);
      ledHeart->turnOff(led + 7 - 1);
    }

    ledHeart->turnOn(led);
    ledHeart->turnOn(led + 7);

    arduinoEnv->delayFor(timeout);
  }

  ledHeart->turnOffAll();
}

void LightShow::letLedsTwistLeft(int timeout, bool turnOffFormerLed) {
  for (int led = 7; led >= 1; led--) {
    if (led < 7 && turnOffFormerLed) {
      ledHeart->turnOff(led);
      ledHeart->turnOff(led + 7);
    }

    ledHeart->turnOn(led - 1);
    ledHeart->turnOn(led + 7 - 1);
    arduinoEnv->delayFor(timeout);
  }

  ledHeart->turnOffAll();
}

void LightShow::turnOnLevelsFromBottomToTop(int timeout, bool turnOffFormerLevel) {
  for (int level = 1; level <= HEART_LEVEL_COUNT; level++) {
    if (level > 1 && turnOffFormerLevel) {
      ledHeart->turnLevelOff(level - 1);
    }

    ledHeart->turnLevelOn(level);
    arduinoEnv->delayFor(timeout);
  }

  if (turnOffFormerLevel) {
    ledHeart->turnOffAll();
  }
}

void LightShow::turnOffLevelsFromBottomToTop(int timeout) {
  for (int level = 1; level <= HEART_LEVEL_COUNT; level++) {
    ledHeart->turnLevelOff(level);
    arduinoEnv->delayFor(timeout);
  }
}

void LightShow::turnOffLevelsFromTopToBottom(int timeout) {
  for (int i = HEART_LEVEL_COUNT; i >= 1; i--) {
    ledHeart->turnLevelOff(i);
    arduinoEnv->delayFor(timeout);
  }
}

void LightShow::turnOnColumnsOutsideToInside(int timeout, bool turnOffFormerColumn, bool turnOffAfterwards) {
  for (int column = 1; column <= 4; column++) {
    if (column > 1 && turnOffFormerColumn) {
      ledHeart->turnColumnOff(column - 1);
      ledHeart->turnColumnOff(HEART_COLUMN_COUNT + 1 - (column - 1));
    }

    ledHeart->turnColumnOn(column);
    ledHeart->turnColumnOn(HEART_COLUMN_COUNT + 1 - column);
    arduinoEnv->delayFor(timeout);
  }

  if (turnOffAfterwards || turnOffFormerColumn) {
    ledHeart->turnOffAll();
  }
}

void LightShow::turnOffColumnsInsideToOutside(int timeout) {
  for (int column = 4; column > 0; column--) {
    if (column == 4) {
      ledHeart->turnColumnOff(column);
    } else {
      ledHeart->turnColumnOff(column);
      ledHeart->turnColumnOff(HEART_COLUMN_COUNT + 1 - column);
    }

    arduinoEnv->delayFor(timeout);
  }
}

void LightShow::turnOnColumnsFromRightToLeft(int timeout, bool turnOffFormerColumn) {
  for (int column = 1; column <= HEART_COLUMN_COUNT; column++) {
    if (column > 1 && turnOffFormerColumn) {
      ledHeart->turnColumnOff(column - 1);
    }
    ledHeart->turnColumnOn(column);
    arduinoEnv->delayFor(timeout);
  }

  if (turnOffFormerColumn) {
    ledHeart->turnOffAll();
  }
}

void LightShow::turnOnColumnsFromLeftToRight(int timeout, bool turnOffFormerColumn) {
  for (int column = HEART_COLUMN_COUNT; column > 0; column--) {
    if (column < HEART_COLUMN_COUNT && turnOffFormerColumn) {
      ledHeart->turnColumnOff(column + 1);
    }
    ledHeart->turnColumnOn(column);
    arduinoEnv->delayFor(timeout);
  }

  if (turnOffFormerColumn) {
    ledHeart->turnOffAll();
  }
}

// LightShowExecutionContainer
LightShowExecutionContainer::LightShowExecutionContainer(IArduinoWrapper* arduinoEnv, int totalExecutions) {
  this->arduinoEnv = arduinoEnv;
  this->totalExecutions = totalExecutions;
}

LightShowExecutionContainer::LightShowExecutionContainer(IArduinoWrapper* arduinoEnv, int totalExecutions, int delay, int brightnessFactor) {
  this->arduinoEnv = arduinoEnv;
  this->totalExecutions = totalExecutions;
  this->delayTime = delay;
  this->brightnessFactor = brightnessFactor;
  this->originalDelayTime = delay;
}

void LightShowExecutionContainer::executeOn(LedHeart* heart) {
  if (currentExecution <= totalExecutions) {
    this->executeNextStepOn(heart);
    heart->toggleBrightness(brightnessFactor);
    this->delay();
    this->currentExecution++;
  }
}

void LightShowExecutionContainer::delay() { arduinoEnv->delayFor(delayTime); }

bool LightShowExecutionContainer::hasAnotherExecution() { return currentExecution <= totalExecutions; }

void LightShowExecutionContainer::reset() {
  currentExecution = 1;
  resetExtended();
}

void LightShowExecutionContainer::resetExtended() {}

void LightShowExecutionContainer::resetDelayTime() { this->delayTime = originalDelayTime; }

int LightShowExecutionContainer::applyDelayFactor(float delayFactor) {
  delayTime = static_cast<int>(delayTime * delayFactor);
  return delayTime;
}

int LightShowExecutionContainer::getTotalExecutions() { return totalExecutions; };

LightShowExecutionContainerRepeater::LightShowExecutionContainerRepeater(IArduinoWrapper* arduinoEnv, LightShowExecutionContainer* executionContainer,
                                                                         int repetitions)
    : LightShowExecutionContainer(arduinoEnv, 0) {
  this->totalExecutions = repetitions * executionContainer->getTotalExecutions();
  this->executionContainer = executionContainer;
  this->originalContainerDelay = executionContainer->delayTime;
}

LightShowExecutionContainerRepeater::LightShowExecutionContainerRepeater(IArduinoWrapper* arduinoEnv, LightShowExecutionContainer* executionContainer,
                                                                         int repetitions, float delayFactor)
    : LightShowExecutionContainer(arduinoEnv, 0) {
  this->totalExecutions = repetitions * executionContainer->getTotalExecutions();
  this->executionContainer = executionContainer;
  this->delayFactor = delayFactor;
  this->originalDelayFactor = delayFactor;
  this->originalContainerDelay = executionContainer->delayTime;
}

int LightShowExecutionContainerRepeater::applyDelayFactor(float newDelayFactor) {
  this->delayFactor = this->delayFactor * newDelayFactor;
  return this->delayTime;
}

void LightShowExecutionContainerRepeater::executeOn(LedHeart* heart) {
  if (currentExecution <= totalExecutions) {
    executeNextStepOn(heart);
    currentExecution++;
  }
}

void LightShowExecutionContainerRepeater::executeNextStepOn(LedHeart* heart) {
  if (!executionContainer->hasAnotherExecution()) {
    executionContainer->reset();
    executionContainer->applyDelayFactor(delayFactor);
  }
  executionContainer->executeOn(heart);
}

void LightShowExecutionContainerRepeater::resetExtended() {
  executionContainer->resetDelayTime();
  executionContainer->reset();
}

void LightShowExecutionContainerRepeater::resetDelayTime() { delayFactor = originalDelayFactor; }

// LightShowExecutionContainerIterator

LightShowExecutionContainerIterator::LightShowExecutionContainerIterator() {}

LightShowExecutionContainerIterator::LightShowExecutionContainerIterator(LightShowExecutionContainer** lightShowExecutionContainers,
                                                                         int lightShowStepCount) {
  this->lightShowExecutionContainers = lightShowExecutionContainers;
  this->lightShowStepCount = lightShowStepCount;
}

void LightShowExecutionContainerIterator::executeNextStepOn(LedHeart* heart) {
  LightShowExecutionContainer* containerExecution = lightShowExecutionContainers[currentIndex];
  containerExecution->executeOn(heart);
  if (!containerExecution->hasAnotherExecution()) {
    containerExecution->reset();
    incrementIndex();
  }
}

void LightShowExecutionContainerIterator::incrementIndex() {
  if (currentIndex == (lightShowStepCount - 1)) {
    currentIndex = 0;
  } else {
    currentIndex++;
  }
}

void LightShowExecutionContainerIterator::reset() {
  currentIndex = 0;

  for (int execution = 0; execution < lightShowStepCount; execution++) {
    lightShowExecutionContainers[execution]->reset();
  }
}

// LightShowSequencer

LightShowSequencer::LightShowSequencer(LedHeart* ledHeart, LightShowExecutionContainerIterator* executionContainer) {
  this->ledHeart = ledHeart;
  this->executionContainer = executionContainer;
}

void LightShowSequencer::executeIteration() { executionContainer->executeNextStepOn(ledHeart); }

// LightShowActions

SequentialLedActivationExecution::SequentialLedActivationExecution(IArduinoWrapper* arduinoEnv, int delay, int brightness, int startIndex,
                                                                   bool turnOffPrevious, bool spinLeft)
    : LightShowExecutionContainer(arduinoEnv, TOTAL_EXECUTIONS, delay, brightness) {
  this->turnOffPrevious = turnOffPrevious;
  this->startIndex = startIndex;
  this->currentLedIndex = startIndex;
  this->spinLeft = spinLeft;
}

SequentialLedActivationExecution::SequentialLedActivationExecution(IArduinoWrapper* arduinoEnv, int delay, int brightness, int startIndex)
    : LightShowExecutionContainer(arduinoEnv, TOTAL_EXECUTIONS, delay, brightness) {
  this->startIndex = startIndex;
  this->currentLedIndex = startIndex;
}

void SequentialLedActivationExecution::executeNextStepOn(LedHeart* heart) {
  if (currentExecution <= totalExecutions) {
    if (currentExecution == 1) {
      heart->turnOffAll();
    } else {
      heart->turnOn(currentLedIndex);

      if (turnOffPrevious) {
        turnOffPreviousLedForCurrentIndex(heart);
      }

      incrementIndex();
    }
  }
}

void SequentialLedActivationExecution::turnOffPreviousLedForCurrentIndex(LedHeart* heart) {
  if (spinLeft) {
    if (currentLedIndex == HEART_LED_COUNT - 1) {
      heart->turnOff(0);
    } else {
      heart->turnOff(currentLedIndex + 1);
    }
  } else {
    if (currentLedIndex == 0) {
      heart->turnOff(HEART_LED_COUNT - 1);
    } else {
      heart->turnOff(currentLedIndex - 1);
    }
  }
}

void SequentialLedActivationExecution::incrementIndex() {
  if (spinLeft) {
    currentLedIndex--;
    if (currentLedIndex == -1) {
      currentLedIndex = HEART_LED_COUNT - 1;
    }
  } else {
    currentLedIndex++;
    if (currentLedIndex == HEART_LED_COUNT) {
      currentLedIndex = 0;
    }
  }
}

void SequentialLedActivationExecution::resetExtended() { currentLedIndex = startIndex; }

OffOnSwitchExecution::OffOnSwitchExecution(IArduinoWrapper* arduinoEnv, int delay, int brightness, int times)
    : LightShowExecutionContainer(arduinoEnv, 0, delay, brightness) {
  this->totalExecutions = 2 * times;
}

void OffOnSwitchExecution::executeNextStepOn(LedHeart* heart) {
  if (isOn) {
    heart->turnOffAll();
    isOn = false;
  } else {
    applyOnActionTo(heart);
    isOn = true;
  }
}

void OffOnSwitchExecution::resetExtended() { isOn = true; }

GlobalHeartBlinkExecution::GlobalHeartBlinkExecution(IArduinoWrapper* arduinoEnv, int delay, int brightness, int times)
    : OffOnSwitchExecution(arduinoEnv, delay, brightness, times) {}

void GlobalHeartBlinkExecution::applyOnActionTo(LedHeart* heart) { heart->turnOnAll(); }

RandomHeartBlinkExecution::RandomHeartBlinkExecution(IArduinoWrapper* arduinoEnv, int delay, int brightness, int times, int minLedsTurnedOn)
    : OffOnSwitchExecution(arduinoEnv, delay, brightness, times) {
  this->minLedsTurnedOn = minLedsTurnedOn;
}

void RandomHeartBlinkExecution::applyOnActionTo(LedHeart* heart) { heart->turnOnRandomly(minLedsTurnedOn); }

LightShowExecutionContainerSequence::LightShowExecutionContainerSequence(int sequenceLength) : LightShowExecutionContainer(nullptr, 0) {
  containerSequence = new LightShowExecutionContainer*[sequenceLength];
  this->sequenceLength = sequenceLength;
};

LightShowExecutionContainerSequence::LightShowExecutionContainerSequence(int sequenceLength, int times) : LightShowExecutionContainer(nullptr, 0) {
  containerSequence = new LightShowExecutionContainer*[sequenceLength];
  this->sequenceLength = sequenceLength;
  this->times = times;
};

LightShowExecutionContainerSequence::LightShowExecutionContainerSequence(int sequenceLength, int times, float delayFactor)
    : LightShowExecutionContainer(nullptr, 0) {
  containerSequence = new LightShowExecutionContainer*[sequenceLength];
  this->sequenceLength = sequenceLength;
  this->times = times;
  this->delayFactor = delayFactor;
  this->originalDelayFactor = delayFactor;
}

void LightShowExecutionContainerSequence::executeOn(LedHeart* heart) {
  if (currentExecution <= totalExecutions) {
    executeNextStepOn(heart);
    currentExecution++;
  }
}
void LightShowExecutionContainerSequence::executeNextStepOn(LedHeart* heart) {
  if (!containerSequence[currentContainerIndex]->hasAnotherExecution()) {
    containerSequence[currentContainerIndex]->reset();
    containerSequence[currentContainerIndex]->applyDelayFactor(delayFactor);

    currentContainerIndex++;
    if (currentContainerIndex == sequenceLength) {
      currentContainerIndex = 0;
    }
  }
  containerSequence[currentContainerIndex]->executeOn(heart);
};

void LightShowExecutionContainerSequence::resetExtended() {
  containerSequence[currentContainerIndex]->reset();
  currentContainerIndex = 0;

  for (int containerIndex = 0; containerIndex < lastContainerAddedIndex; containerIndex++) {
    containerSequence[containerIndex]->resetDelayTime();
  }
};

int LightShowExecutionContainerSequence::applyDelayFactor(float delayFactor) {
  this->delayFactor = this->delayFactor * delayFactor;
  return delayTime;
}
void LightShowExecutionContainerSequence::resetDelayTime() { this->delayFactor = originalDelayFactor; }

LightShowExecutionContainerSequence* LightShowExecutionContainerSequence::addContainer(LightShowExecutionContainer* container) {
  this->totalExecutions += times * container->getTotalExecutions();
  this->containerSequence[lastContainerAddedIndex] = container;
  lastContainerAddedIndex++;
  return this;
}

LightShowExecutionContainerSequence* LightShowExecutionContainerSequence::executeTimes(int times) {
  this->times = times;
  return this;
}

SequentialRowActivator* SequentialRowActivator::createUpwardsMovingRowActivator(IArduinoWrapper* arduinoEnv, int delay, int brightness) {
  return (SequentialRowActivator*)(new UpwardsMovingSequentialRowActivator(arduinoEnv, delay, brightness));
}

SequentialRowActivator* SequentialRowActivator::createDownwardsMovingRowActivator(IArduinoWrapper* arduinoEnv, int delay, int brightness) {
  return (SequentialRowActivator*)(new DownwardsMovingSequentialRowActivator(arduinoEnv, delay, brightness));
}

SequentialRowActivator::SequentialRowActivator(IArduinoWrapper* arduinoEnv, int delay, int brightness)
    : LightShowExecutionContainer(arduinoEnv, HEART_LEVEL_COUNT, delay, brightness){};

void SequentialRowActivator::executeNextStepOn(LedHeart* heart) {
  if (turnOffPreviousLed || currentExecution == 1) {
    heart->turnOffAll();
  }
  heart->turnLevelOn(currentLevelIndex);
  incrementIndex();
}

void SequentialRowActivator::resetExtended() { this->currentLevelIndex = startIndex; };

SequentialRowActivator* SequentialRowActivator::withStartIndex(int startIndex) {
  this->startIndex = startIndex;
  this->currentLevelIndex = startIndex;
  return this;
}

SequentialRowActivator* SequentialRowActivator::turnOffPrevious(bool turnOff) {
  this->turnOffPreviousLed = turnOff;
  return this;
}

UpwardsMovingSequentialRowActivator::UpwardsMovingSequentialRowActivator(IArduinoWrapper* arduinoEnv, int delay, int brightness)
    : SequentialRowActivator(arduinoEnv, delay, brightness){};

void UpwardsMovingSequentialRowActivator::incrementIndex() {
  if (currentLevelIndex == HEART_LEVEL_COUNT) {
    currentLevelIndex = 1;
  } else {
    currentLevelIndex++;
  }
}
DownwardsMovingSequentialRowActivator::DownwardsMovingSequentialRowActivator(IArduinoWrapper* arduinoEnv, int delay, int brightness)
    : SequentialRowActivator(arduinoEnv, delay, brightness){};

void DownwardsMovingSequentialRowActivator::incrementIndex() {
  if (currentLevelIndex == 1) {
    currentLevelIndex = HEART_LEVEL_COUNT;
  } else {
    currentLevelIndex--;
  }
}
