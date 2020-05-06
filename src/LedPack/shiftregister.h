#ifndef shiftregister_h
#define shiftregister_h

#include "arduinoWrapper.h"

const int MAX_SHIFT_REGISTER_LED_COUNT = 7;
const int MAX_LEVEL = 6;
const int MAX_COLUMNS = 4;

class Led {
  private:
    int ledNumber;

  public:
    int shiftRegisterPosition;
    bool on;

    Led(int ledNum, int shiftRegisterPosition);

    Led();

    void turnOn();

    void turnOff();
};

typedef struct LedShiftRegisterPins {
  int latchPin;
  int clockPin;
  int dataPin;
  int outputEnablePin;
};

class LevelActivator {
  public:
    LevelActivator();
    virtual void turnOnLevel(int level, Led *leds) = 0;
    virtual void turnOffLevel(int level, Led *leds) = 0;
};

class ColumnActivator {
  public:
    ColumnActivator();
    virtual void turnOnColumn(int column, Led *leds) = 0;
    virtual void turnOffColumn(int column, Led *leds) = 0;
};

class LedShiftRegister {
  private:
    IArduinoWrapper *arduinoEnv;
    LevelActivator *levelActivator;
    ColumnActivator *columnActivator;
    byte shiftRegisterByte;
    LedShiftRegisterPins pins;
    Led leds[MAX_SHIFT_REGISTER_LED_COUNT];
    
    void updateShiftRegister();
    void updateShiftRegisterByte();

  public:
    LedShiftRegister();

    LedShiftRegister(LedShiftRegisterPins pins, Led *ledsOnShiftRegister, LevelActivator *levelActivator, ColumnActivator *columnActivator, IArduinoWrapper *arduinoEnvironment);

    void turnOn(int ledIndex);

    void turnOff(int ledIndex);

    void turnOffAll();

    void turnOnAll();

    void turnOnLevel(int level);

    void turnOffLevel(int level);

    void turnOnColumn(int column);

    void turnOffColumn(int column);    

    void toggleBrightness(byte value);

    void initializePins();
};

#endif
