#ifndef arduinoWrapper_h
#define arduinoWrapper_h

#include "stdint.h"

#ifndef byte
#define byte uint8_t 
#endif

class IArduinoWrapper {
  public:
    virtual void beginSerial(int baudRate) = 0;

    virtual void setPinToInputMode(int pinNumber) = 0;

    virtual void setPinToOutputMode(int pinNumber) = 0;

    virtual int readAnalogFrom(int pinNumber) = 0;

    virtual int readDigitalFrom(int pinNumber) = 0;

    virtual void writeToAnalog(int pinNumber, int value) = 0;

    virtual void writeToDigital(int pinNumber, int value) = 0;

    virtual void writeHIGHToDigital(int pinNumber) = 0;

    virtual void writeLOWToDigital(int pinNumber) = 0;

    virtual void shift(int dataPin, int cloakPin, byte value) = 0;

    virtual void setBit(byte& byteToBeChanged, int bitPosition) = 0;

    virtual void clearBit(byte& byteToBeChanged, int bitPosition) = 0;
    
    virtual void print(int value) = 0;

    virtual void print(char* value) = 0;

    virtual void println(int value) = 0;

    virtual void println(char* value) = 0;

    virtual void delayFor(int milliseconds) = 0;
};

class IStandardFunctions {
  public:
    virtual int maxValue(int firstValue, int secondValue) = 0;

    virtual int minValue(int firstValue, int secondValue) = 0;

    virtual int mapValue(int value, int currentLower, int currentUpper, int maximumUpper, int maximumLower) = 0;

    virtual int getCurrentMilliseconds() = 0;

    virtual int nextRandomInt() = 0;
};

#endif
