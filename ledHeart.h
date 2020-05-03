#ifndef ledHeart_h
#define ledHeart_h

#include "shiftregister.h"

const int HEART_LED_COUNT = 14;
const int HEART_LEVEL_COUNT = 6;
const int HEART_COLUMN_COUNT = 7;


class LeftRegisterLevelActivator : public LevelActivator {
  public:
    void turnOnLevel(int level, Led *leds);
    void turnOffLevel(int level, Led *leds);
};

class RightRegisterLevelActivator : public LevelActivator {
  public:
    void turnOnLevel(int level, Led *leds);
    void turnOffLevel(int level, Led *leds);
};

class LeftRegisterColumnActivator : public ColumnActivator {
  public:
    void turnOnColumn(int column, Led *leds);
    void turnOffColumn(int column, Led *leds);
};

class RightRegisterColumnActivator : public ColumnActivator {
  public:
    void turnOnColumn(int column, Led *leds);
    void turnOffColumn(int column, Led *leds);
};

class LedHeart {
  private:
    LedShiftRegister *leftShiftRegister;
    LedShiftRegister *rightShiftRegister;
    bool alreadyTurnedOn(int *turnedOn, int arrayLength, int ledIndex);

  public:
    LedHeart(LedShiftRegister *leftShiftRegister, LedShiftRegister *rightShiftRegister);

    void initialize();

    void turnOnAll();

    void turnOffAll();

    void turnOn(int ledIndex);

    void turnOff(int ledIndex);

    void turnLevelOn(int level);

    void turnLevelOff(int level);

    void turnColumnOn(int column);

    void turnColumnOff(int column);

    void toggleBrightness(int brightness);

    void turnOnRandomly(int minLedsTurnedOn = 0);
};

#endif
