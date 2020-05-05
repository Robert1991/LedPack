#ifndef mockClasses_h
#define mockClasses_h

#include "gmock/gmock.h"
#include "arduinoWrapper.h"

class TestArduinoEnvironment : public IArduinoWrapper
{
public:
    MOCK_METHOD(void, beginSerial, (int baudRate), (override));
    MOCK_METHOD(void, setPinToInputMode, (int pinNumber), (override));
    MOCK_METHOD(void, setPinToOutputMode, (int pinNumber), (override));
    MOCK_METHOD(int, readAnalogFrom, (int pinNumber), (override));
    MOCK_METHOD(int, readDigitalFrom, (int pinNumber), (override));
    MOCK_METHOD(void, writeToAnalog, (int pinNumber, int value), (override));
    MOCK_METHOD(void, writeToDigital, (int pinNumber, int value), (override));
    MOCK_METHOD(void, writeHIGHToDigital, (int pinNumber), (override));
    MOCK_METHOD(void, writeLOWToDigital, (int pinNumber), (override));
    MOCK_METHOD(void, shift, (int dataPin, int cloakPin, byte value), (override));
    MOCK_METHOD(void, setBit, (byte & byteToBeChanged, int bitPosition), (override));
    MOCK_METHOD(void, clearBit, (byte & byteToBeChanged, int bitPosition), (override));
    MOCK_METHOD(void, print, (int value), (override));
    MOCK_METHOD(void, print, (char *value), (override));
    MOCK_METHOD(void, println, (int value), (override));
    MOCK_METHOD(void, println, (char *value), (override));
    MOCK_METHOD(void, delayFor, (int milliseconds), (override));
};

#endif