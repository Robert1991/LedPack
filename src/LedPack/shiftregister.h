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

#ifndef LedShiftRegisterPins
typedef struct LedShiftRegisterPins {
  int latchPin;
  int clockPin;
  int dataPin;
  int outputEnablePin;
};
#endif

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

    virtual void turnOn(int ledIndex);

    virtual void turnOff(int ledIndex);

    virtual void turnOffAll();

    virtual void turnOnAll();

    virtual void turnOnLevel(int level);

    virtual void turnOffLevel(int level);

    virtual void turnOnColumn(int column);

    virtual void turnOffColumn(int column);    

    virtual void toggleBrightness(byte value);

    virtual void initializePins();
};

#endif
