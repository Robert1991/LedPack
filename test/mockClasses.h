#ifndef mockClasses_h
#define mockClasses_h

#include <chrono>
#include "gmock/gmock.h"
#include "arduinoWrapper.h"
#include "shiftregister.h"
#include "gyroscope.h"
#include "ledHeart.h"
#include "movementShow.h"
#include "microphone.h"
#include "lowPassFilter.h"
#include "lightShow.h"

using namespace std::chrono;

class FakeArduinoEnvironment : public IArduinoWrapper
{
public:
    void beginSerial(int baudRate)
    {
    }

    void setPinToInputMode(int pinNumber)
    {
    }

    void setPinToOutputMode(int pinNumber)
    {
    }

    int readAnalogFrom(int pinNumber)
    {
        return pinNumber;
    }

    int readDigitalFrom(int pinNumber)
    {
        return pinNumber;
    }

    void writeToAnalog(int pinNumber, int value)
    {
    }

    void writeToDigital(int pinNumber, int value)
    {
    }

    void writeHIGHToDigital(int pinNumber)
    {
    }

    void writeLOWToDigital(int pinNumber)
    {
    }

    void shift(int dataPin, int cloakPin, byte value)
    {
    }

    void setBit(byte &byteToBeChanged, int bitPosition)
    {
        (byteToBeChanged) |= (1UL << (bitPosition));
    }

    void clearBit(byte &byteToBeChanged, int bitPosition)
    {
        (byteToBeChanged) &= ~(1UL << (bitPosition));
    }

    void print(int value)
    {
    }

    void print(char *value)
    {
    }

    void println(int value)
    {
    }

    void println(char *value)
    {
    }

    void delayFor(int milliseconds)
    {
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
        ON_CALL(*this, setBit).WillByDefault([this](byte &byteToBeChanged, int bitPosition) {
            return fakeEnv.setBit(byteToBeChanged, bitPosition);
        });

        ON_CALL(*this, clearBit).WillByDefault([this](byte &byteToBeChanged, int bitPosition) {
            return fakeEnv.clearBit(byteToBeChanged, bitPosition);
        });
    }

private:
    FakeArduinoEnvironment fakeEnv;
};

// Namespace functions
class STDNamespaceFunctions : public IStandardFunctions
{
public:
    long maxValue(long firstValue, long secondValue)
    {
        return std::max(firstValue, secondValue);
    }

    long minValue(long firstValue, long secondValue)
    {
        return std::min(firstValue, secondValue);
    }

    long mapValue(long value, long inLower, long inUpper, long outLower, long outUpper)
    {
        if (inLower == inUpper) {
            return outUpper;
        }

        return (value - inLower) * (outUpper - outLower) / (inUpper - inLower) + outLower;
    }

    long getCurrentMilliseconds()
    {
        milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        return ms.count();
    }

    int nextRandomInt()
    {
        return std::rand();
    }

    int nextRandomIntInBounds(int lower, int upper) 
    {
        return lower + std::rand() % ( upper - lower + 1 );
    }
};

class IStandardFunctionsMock : public IStandardFunctions
{
public:
    MOCK_METHOD(long, maxValue, (long firstValue, long secondValue), (override));
    MOCK_METHOD(long, minValue, (long firstValue, long secondValue), (override));
    MOCK_METHOD(long, mapValue, (long value, long inLower, long inUpper, long outLower, long outUpper), (override));
    MOCK_METHOD(long, getCurrentMilliseconds, (), (override));
    MOCK_METHOD(int, nextRandomInt, (), (override));
    MOCK_METHOD(int, nextRandomIntInBounds, (int lower, int upper), (override));
};

// LedHeart

class LedHeartMock : public LedHeart 
{
public:
    MOCK_METHOD(void, initialize, (), ());

    MOCK_METHOD(void, turnOnAll, (), (override));

    MOCK_METHOD(void, turnOffAll, (), (override));

    MOCK_METHOD(void, turnOn, (int ledIndex), (override));

    MOCK_METHOD(void, turnOff, (int ledIndex), (override));

    MOCK_METHOD(void, turnLevelOn, (int level), ());

    MOCK_METHOD(void, turnLevelOff, (int level), ());

    MOCK_METHOD(void, turnColumnOn, (int column), ());

    MOCK_METHOD(void, turnColumnOff, (int column), ());

    MOCK_METHOD(void, toggleBrightness, (int brightness), (override));

    MOCK_METHOD(void, turnOnRandomly, (int minLedsTurnedOn), (override));
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

class ShiftRegisterMock : public LedShiftRegister
{
public:
    MOCK_METHOD(void, turnOn, (int ledIndex), (override));
    MOCK_METHOD(void, turnOff, (int ledIndex), (override));
    MOCK_METHOD(void, turnOffAll, (), (override));
    MOCK_METHOD(void, turnOnAll, (), (override));
    MOCK_METHOD(void, turnOnLevel, (int level), (override));
    MOCK_METHOD(void, turnOffLevel, (int level), (override));
    MOCK_METHOD(void, turnOnColumn, (int column), (override));
    MOCK_METHOD(void, turnOffColumn, (int column), (override));
    MOCK_METHOD(void, toggleBrightness, (byte value), (override));
    MOCK_METHOD(void, initializePins, (), (override));
};


// Gyroscope
class GyroscopeMock : public Gyroscope {
public:
    MOCK_METHOD(AccelerationMeasurementVector, measureAcceleration, (), ());
    MOCK_METHOD(void, wakeUp, (), ());
};

class DummyAccelerationRatioMapper : public IAccelerationRatioMapper {
    public:
        DummyAccelerationRatioMapper() {};
        virtual int mapToInt(float accelerationRatio) {
            return (int)(accelerationRatio * 100);
        }
};


// Wire
class WireMock : public GyroscopeWire {
  public:
    MOCK_METHOD(void, initialize, (), (override));
    MOCK_METHOD(void, requestMeasurement, (), (override));
    MOCK_METHOD(int, readNextRegister, (), (override));
};


// Movement show
class MovementShowActionMapperMock : public MovementShowActionMapper {
public:
    MovementShowActionMapperMock() : MovementShowActionMapper(nullptr, nullptr)
    {
        
    }
    ~MovementShowActionMapperMock() {}

    MOCK_METHOD(void, mapToHeart, (AccerlationVectorDifference *difference), ());
};


// Microphone
class MicrophoneMock : public Microphone 
{
public:
    MOCK_METHOD(void, init, (), (override));
    MOCK_METHOD(int, readAnalog, (), (override));
    MOCK_METHOD(int, readDigital, (), (override));
};

// LowPassFilter
class LowPassFilterMock : public ILowPassFilter {
  public:
    MOCK_METHOD(void, put, (int input), (override));
    MOCK_METHOD(int, get, (), (override));
};


// LightShow
class LightShowExecutionContainerIteratorMock : public LightShowExecutionContainerIterator {
  public:
    MOCK_METHOD(void, executeNextStepOn, (LedHeart* heart), (override));
    MOCK_METHOD(void, reset, (), (override));
};

#endif