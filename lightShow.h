#ifndef lightShow_h
#define lightShow_h

#include "ledHeart.h"
#include "Arduino.h"

class LightShow {
  private:
    LedHeart *ledHeart;
    
    void turnOnAllLedsOnHeartInARow(int timeout);
    void letLedsOnHeartBlinkFor(int times, int onTimeout, int offTimeout);
    void letLedsTwistRight(int timeout, bool turnOffFormerLed = true);
    void letLedsTwistLeft(int timeout, bool turnOffFormerLed = true);
    void turnOnLevelsFromBottomToTop(int timeout, boolean turnOffFormerLevel = false);
    void turnOffLevelsFromBottomToTop(int timeout);
    void turnOffLevelsFromTopToBottom(int timeout);
    void turnOnColumnsOutsideToInside(int timeout, bool turnOffFormerColumn = false, bool turnOffAfterwards = false);
    void turnOffColumnsInsideToOutside(int timeout);
    void turnOnColumnsFromRightToLeft(int timeout, bool turnOffFormerColumn = false);
    void turnOnColumnsFromLeftToRight(int timeout, bool turnOffFormerColumn = false);
    
  public: 
    LightShow(LedHeart *ledHeart);
    void execute();
};

#endif
