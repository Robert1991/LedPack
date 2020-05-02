#ifndef ledHeart_h
#define ledHeart_h

#include "shiftregister.h"

const int HEART_LED_COUNT = 14;

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

    void toggleBrightness(int brightness);

    void turnOnRandomly(int minLedsTurnedOn = 0);
};

#endif
