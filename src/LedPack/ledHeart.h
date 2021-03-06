#ifndef ledHeart_h
#define ledHeart_h

#include "shiftregister.h"

const int HEART_LED_COUNT = 14;
const int HEART_LEVEL_COUNT = 6;
const int HEART_COLUMN_COUNT = 7;
const int HEART_MAX_BRIGHTNESS = 255;

class LeftRegisterLevelActivator : public LevelActivator {
 private:
  void doForLevel(int level, Led *leds, void (Led::*ledFunctionPointer)());

 public:
  void turnOnLevel(int level, Led *leds);
  void turnOffLevel(int level, Led *leds);
};

class RightRegisterLevelActivator : public LevelActivator {
 private:
  void doForLevel(int level, Led *leds, void (Led::*ledFunctionPointer)());

 public:
  void turnOnLevel(int level, Led *leds);
  void turnOffLevel(int level, Led *leds);
};

class LeftRegisterColumnActivator : public ColumnActivator {
 private:
  void doForColumn(int column, Led *leds, void (Led::*ledFunctionPointer)());

 public:
  void turnOnColumn(int column, Led *leds);
  void turnOffColumn(int column, Led *leds);
};

class RightRegisterColumnActivator : public ColumnActivator {
 private:
  void doForColumn(int column, Led *leds, void (Led::*ledFunctionPointer)());

 public:
  void turnOnColumn(int column, Led *leds);
  void turnOffColumn(int column, Led *leds);
};

class LedHeart {
 private:
  LedShiftRegister *leftShiftRegister;
  LedShiftRegister *rightShiftRegister;
  IStandardFunctions *stdFunctions;

  bool alreadyTurnedOn(int *turnedOn, int arrayLength, int ledIndex);
  void doForColumn(int column, void (LedShiftRegister::*shiftRegisterFunctionPointer)(int));
  void doForLevel(int level, void (LedShiftRegister::*shiftRegisterFunctionPointer)(int));
  void doForLed(int ledIndex, void (LedShiftRegister::*shiftRegisterFunctionPointer)(int));

 public:
  LedHeart();
  LedHeart(IStandardFunctions *stdFunctions, LedShiftRegister *leftShiftRegister, LedShiftRegister *rightShiftRegister);

  virtual void initialize();
  virtual void turnOnAll();
  virtual void turnOffAll();
  virtual void turnOn(int ledIndex);
  virtual void turnOff(int ledIndex);
  virtual void turnLevelOn(int level);
  virtual void turnLevelOff(int level);
  virtual void turnColumnOn(int column);
  virtual void turnColumnOff(int column);
  virtual void toggleBrightness(int brightness);
  virtual void turnOnRandomly(int minLedsTurnedOn = 0);
};

#endif
