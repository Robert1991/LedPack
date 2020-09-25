#ifndef lightShow_h
#define lightShow_h

#define LIGHT_SHOW_ACTION 1
#define LIGHT_SHOW_ACTION_CONTAINER 2

#include "ledHeart.h"

class LightShow {
  private:
    LedHeart *ledHeart;
    IArduinoWrapper *arduinoEnv;
    
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
    LightShow(IArduinoWrapper *arduinoEnv, LedHeart *ledHeart);
    void execute();
};

class LightShowExecutionContainer {
  private:
    IArduinoWrapper *arduinoEnv;

    void delay();

  protected:
    int delayTime = 0;
    int brightnessFactor = HEART_MAX_BRIGHTNESS;
    int totalExecutions = 0;
    int currentExecution = 0;

    virtual void executeNextStepOn(LedHeart* heart) = 0;
    virtual void resetExtended();

  public:
    LightShowExecutionContainer(IArduinoWrapper *arduinoEnv, int totalExecutions);
    LightShowExecutionContainer(IArduinoWrapper *arduinoEnv, int totalExecutions, int delay, int brightnessFactor);
    void executeOn(LedHeart* heart);
    bool hasAnotherExecution();
    void reset();
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

class LightShowExecutionContainerRepeater : public LightShowExecutionContainer {
  private:
    int repetitions = 0;
    LightShowExecutionContainer* executionContainer;
  
  protected:
    void executeNextStepOn(LedHeart* heart);
    void resetExtended();

  public:
    LightShowExecutionContainerRepeater(IArduinoWrapper *arduinoEnv, LightShowExecutionContainer* executionContainer, int repetitions);
};

class SequentialLedActivationExecution : public LightShowExecutionContainer {
  private:
    static const int TOTAL_EXECUTIONS = 15;
    int startIndex = 0;
    int currentStep = 0;
    int currentLedIndex = 0;
    bool turnOffPrevious = false;
    bool spinLeft = false;

    void turnOffPreviousLedForCurrentIndex(LedHeart *heart);
    void incrementIndex();

  protected:
    void executeNextStepOn(LedHeart* heart);
    void resetExtended();

  public:
    SequentialLedActivationExecution(IArduinoWrapper *arduinoEnv, int delay, int brightness, int startIndex);
    SequentialLedActivationExecution(IArduinoWrapper *arduinoEnv, int delay, int brightness, int startIndex, bool turnOffPrevious, bool spinLeft);
};

class RandomHeartBlinkExecution : public LightShowExecutionContainer {
  private:
    // this sets as default, that the heart is turned off by default in the first iteration
    bool isOn = true;
    int minLedsTurnedOn = 1;

  protected:
    void resetExtended();
    void executeNextStepOn(LedHeart* heart);

  public:
    RandomHeartBlinkExecution(IArduinoWrapper *arduinoEnv, int delay, int brightness, int times, int minLedsTurnedOn);

};

#endif
