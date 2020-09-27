#ifndef lightShow_h
#define lightShow_h

#define LIGHT_SHOW_ACTION 1
#define LIGHT_SHOW_ACTION_CONTAINER 2

#include "ledHeart.h"

class LightShow {
 private:
  LedHeart* ledHeart;
  IArduinoWrapper* arduinoEnv;

  void turnOnAllLedsOnHeartInARow(int timeout);
  void letLedsOnHeartBlinkFor(int times, int onTimeout, int offTimeout);
  void letLedsTwistRight(int timeout, bool turnOffFormerLed = true);
  void letLedsTwistLeft(int timeout, bool turnOffFormerLed = true);
  void turnOnLevelsFromBottomToTop(int timeout, bool turnOffFormerLevel = false);
  void turnOffLevelsFromBottomToTop(int timeout);
  void turnOffLevelsFromTopToBottom(int timeout);
  void turnOnColumnsOutsideToInside(int timeout, bool turnOffFormerColumn = false, bool turnOffAfterwards = false);
  void turnOffColumnsInsideToOutside(int timeout);
  void turnOnColumnsFromRightToLeft(int timeout, bool turnOffFormerColumn = false);
  void turnOnColumnsFromLeftToRight(int timeout, bool turnOffFormerColumn = false);

 public:
  LightShow(IArduinoWrapper* arduinoEnv, LedHeart* ledHeart);
  void execute();
};

class LightShowExecutionContainer {
 private:
  IArduinoWrapper* arduinoEnv;

  void delay();

 protected:
  int currentExecution = 1;
  int totalExecutions = 0;
  int originalDelayTime;

  virtual void resetExtended();
  virtual void executeNextStepOn(LedHeart* heart) = 0;

 public:
  int delayTime = 0;
  int brightnessFactor = HEART_MAX_BRIGHTNESS;

  LightShowExecutionContainer(IArduinoWrapper* arduinoEnv, int totalExecutions);
  LightShowExecutionContainer(IArduinoWrapper* arduinoEnv, int totalExecutions, int delay, int brightnessFactor);

  virtual void executeOn(LedHeart* heart);
  virtual int applyDelayFactor(float delayFactor);
  virtual void resetDelayTime();
  bool hasAnotherExecution();
  void reset();
  int getTotalExecutions();
};

class LightShowExecutionContainerRepeater : public LightShowExecutionContainer {
 private:
  int repetitions = 0;
  int originalContainerDelay = 0;
  float delayFactor = 1.0;
  float originalDelayFactor = 1.0;
  LightShowExecutionContainer* executionContainer;

 protected:
  void executeNextStepOn(LedHeart* heart);
  void resetExtended();

 public:
  LightShowExecutionContainerRepeater(IArduinoWrapper* arduinoEnv, LightShowExecutionContainer* executionContainer, int repetitions);
  LightShowExecutionContainerRepeater(IArduinoWrapper* arduinoEnv, LightShowExecutionContainer* executionContainer, int repetitions,
                                      float delayFactor);
  void resetDelayTime();
  int applyDelayFactor(float delayFactor);
  void executeOn(LedHeart* heart);
};

class LightShowExecutionContainerSequence : public LightShowExecutionContainer {
 private:
  int sequenceLength = 0;
  int currentContainerIndex = 0;
  int lastContainerAddedIndex = 0;
  int times = 1;
  float delayFactor = 1.0;
  float originalDelayFactor = 1.0;
  LightShowExecutionContainer** containerSequence;
  int* originalContainerDelays;

 protected:
  void executeNextStepOn(LedHeart* heart);
  void resetExtended();

 public:
  LightShowExecutionContainerSequence(int sequenceLength);
  LightShowExecutionContainerSequence(int sequenceLength, int times);
  LightShowExecutionContainerSequence(int sequenceLength, int times, float delayFactor);

  void executeOn(LedHeart* heart);
  LightShowExecutionContainerSequence* addContainer(LightShowExecutionContainer* container);
  LightShowExecutionContainerSequence* executeTimes(int times);
  int applyDelayFactor(float delayFactor);
  void resetDelayTime();
};

class LightShowExecutionContainerIterator {
 private:
  int currentIndex = 0;
  int lightShowStepCount = 0;
  LightShowExecutionContainer** lightShowExecutionContainers;

  void incrementIndex();

 public:
  LightShowExecutionContainerIterator();
  LightShowExecutionContainerIterator(LightShowExecutionContainer** lightShowExecutionContainers, int lightShowStepCount);

  virtual void executeNextStepOn(LedHeart* heart);
  virtual void reset();
};

class LightShowSequencer {
 private:
  LedHeart* ledHeart;
  LightShowExecutionContainerIterator* executionContainer;

 public:
  LightShowSequencer(LedHeart* ledHeart, LightShowExecutionContainerIterator* executionContainer);

  void executeIteration();
};

class SequentialLedActivationExecution : public LightShowExecutionContainer {
 private:
  static const int TOTAL_EXECUTIONS = 15;
  int startIndex = 0;
  int currentStep = 0;
  int currentLedIndex = 0;
  bool turnOffPrevious = false;
  bool spinLeft = false;

  void turnOffPreviousLedForCurrentIndex(LedHeart* heart);
  void incrementIndex();

 protected:
  void executeNextStepOn(LedHeart* heart);
  void resetExtended();

 public:
  SequentialLedActivationExecution(IArduinoWrapper* arduinoEnv, int delay, int brightness, int startIndex);
  SequentialLedActivationExecution(IArduinoWrapper* arduinoEnv, int delay, int brightness, int startIndex, bool turnOffPrevious, bool spinLeft);
};

class OffOnSwitchExecution : public LightShowExecutionContainer {
 private:
  // this sets as default, that the heart is turned off by default in the first iteration
  bool isOn = true;

 protected:
  void resetExtended();
  void executeNextStepOn(LedHeart* heart);
  virtual void applyOnActionTo(LedHeart* heart) = 0;

 public:
  OffOnSwitchExecution(IArduinoWrapper* arduinoEnv, int delay, int brightness, int times);
};

class GlobalHeartBlinkExecution : public OffOnSwitchExecution {
 protected:
  void applyOnActionTo(LedHeart* heart);

 public:
  GlobalHeartBlinkExecution(IArduinoWrapper* arduinoEnv, int delay, int brightness, int times);
};

class RandomHeartBlinkExecution : public OffOnSwitchExecution {
 private:
  int minLedsTurnedOn = 1;

 protected:
  void applyOnActionTo(LedHeart* heart);

 public:
  RandomHeartBlinkExecution(IArduinoWrapper* arduinoEnv, int delay, int brightness, int times, int minLedsTurnedOn);
};

class SequentialRowActivator : public LightShowExecutionContainer {
 private:
  int startIndex = 1;
  bool turnOffPreviousLed = false;

 protected:
  int currentLevelIndex = 1;
  virtual void incrementIndex() = 0;
  void resetExtended();
  void executeNextStepOn(LedHeart* heart);

 public:
  static SequentialRowActivator* createUpwardsMovingRowActivator(IArduinoWrapper* arduinoEnv, int delay, int brightness);
  static SequentialRowActivator* createDownwardsMovingRowActivator(IArduinoWrapper* arduinoEnv, int delay, int brightness);
  SequentialRowActivator(IArduinoWrapper* arduinoEnv, int delay, int brightness);

  SequentialRowActivator* withStartIndex(int startIndex);
  SequentialRowActivator* turnOffPrevious(bool turnOff);
};

class UpwardsMovingSequentialRowActivator : SequentialRowActivator {
 protected:
  void incrementIndex();

 public:
  UpwardsMovingSequentialRowActivator(IArduinoWrapper* arduinoEnv, int delay, int brightness);
};

class DownwardsMovingSequentialRowActivator : SequentialRowActivator {
 protected:
  void incrementIndex();

 public:
  DownwardsMovingSequentialRowActivator(IArduinoWrapper* arduinoEnv, int delay, int brightness);
};

#endif
