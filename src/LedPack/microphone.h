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
    Microphone();

    Microphone(IArduinoWrapper *arduinoEnv, int microphonePin, int digitalInputPin);

    virtual void init();
    
    virtual int readAnalog();

    virtual int readDigital();
};

#endif
