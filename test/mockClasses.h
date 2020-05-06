#ifndef mockClasses_h
#define mockClasses_h

#include "gmock/gmock.h"
#include "arduinoWrapper.h"
#include "shiftregister.h"

class FakeArduinoEnvironment : public IArduinoWrapper {
    public:
        void beginSerial(int baudRate) {

        }

        void setPinToInputMode(int pinNumber) {

        }

        void setPinToOutputMode(int pinNumber) {

        }

        int readAnalogFrom(int pinNumber) {
            return pinNumber;
        }

        int readDigitalFrom(int pinNumber) {
            return pinNumber;
        }

        void writeToAnalog(int pinNumber, int value)  {

        }

        void writeToDigital(int pinNumber, int value) {

        }

        void writeHIGHToDigital(int pinNumber) {

        }

        void writeLOWToDigital(int pinNumber) {

        }

        void shift(int dataPin, int cloakPin, byte value) {

        }

        void setBit(byte & byteToBeChanged, int bitPosition) {
            (byteToBeChanged) |= (1UL << (bitPosition));
        }

        void clearBit(byte& byteToBeChanged, int bitPosition) {
            (byteToBeChanged)  &= ~(1UL << (bitPosition));
        }
    
        void print(int value) {

        }

        void print(char* value) {

        }

        void println(int value) {

        }

        void println(char* value) {

        }

        void delayFor(int milliseconds) {

        }
};

// Arduino env
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

    void DelegateToFake()
    {
        ON_CALL(*this, setBit).WillByDefault([this](byte & byteToBeChanged, int bitPosition) {
            return fakeEnv.setBit(byteToBeChanged, bitPosition);
        });

        ON_CALL(*this, clearBit).WillByDefault([this](byte & byteToBeChanged, int bitPosition) {
            return fakeEnv.clearBit(byteToBeChanged, bitPosition);
        });
    }

    private:
        FakeArduinoEnvironment fakeEnv;
};

// Shiftregister
class LevelActivatorMock : public LevelActivator
{
public:
    MOCK_METHOD(void, turnOnLevel, (int level, Led *leds), (override));
    MOCK_METHOD(void, turnOffLevel, (int level, Led *leds), (override));
};

class ColumnActivatorMock : public ColumnActivator
{
public:
    MOCK_METHOD(void, turnOnColumn, (int column, Led *leds), (override));
    MOCK_METHOD(void, turnOffColumn, (int column, Led *leds), (override));
};

#endif