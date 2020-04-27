#ifndef microphone_h
#define microphone_h

#include "Arduino.h"

class Microphone
{
  private:
    int microphonePin;
    int digitalInputPin;
    
  public:
    Microphone(int microphonePin, int digitalInputPin);

    void init();
    
    int readAnalog();

    int readDigital();
};

#endif
