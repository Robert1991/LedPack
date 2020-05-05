#ifndef microphone_h
#define microphone_h

#include "arduinoWrapper.h"

class Microphone
{
  private:
    IArduinoWrapper *arduinoEnv;
    int microphonePin;
    int digitalInputPin;
    
  public:
    Microphone(IArduinoWrapper *arduinoEnv, int microphonePin, int digitalInputPin);

    void init();
    
    int readAnalog();

    int readDigital();
};

#endif
