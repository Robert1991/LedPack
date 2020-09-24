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
    virtual long maxValue(long firstValue, long secondValue) = 0;

    virtual long minValue(long firstValue, long secondValue) = 0;

    virtual long mapValue(long value, long currentLower, long currentUpper, long maximumLower, long maximumUpper) = 0;

    virtual long getCurrentMilliseconds() = 0;

    virtual int nextRandomInt() = 0;

    virtual int nextRandomIntInBounds(int lower, int upper) = 0;
};

class GyroscopeWire {
  public:
    virtual void initialize() = 0;

    virtual void requestMeasurement() = 0;
    
    virtual int readNextRegister() = 0;
};
#endif
