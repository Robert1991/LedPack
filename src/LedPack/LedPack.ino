#include "gyroscope.h"
#include "microphone.h"
#include "lowPassFilter.h"
#include "lowPassSampler.h"
#include "shiftregister.h"
#include "ledHeart.h"
#include "lightShow.h"
#include "movementShow.h"
#include "bassfilterShow.h"
#include "arduinoWrapper.h"

class ArduinoWrapper : public IArduinoWrapper {
  public:
    void beginSerial(int baudRate) {
      Serial.begin(baudRate);
    }

    void setPinToInputMode(int pinNumber) {
      pinMode(pinNumber, INPUT);
    }

    void setPinToOutputMode(int pinNumber) {
      pinMode(pinNumber, OUTPUT);
    }

    int readAnalogFrom(int pinNumber) {
      return analogRead(pinNumber);
    }

    int readDigitalFrom(int pinNumber) {
      return digitalRead(pinNumber);
    }

    void writeToAnalog(int pinNumber, int value) {
      analogWrite(pinNumber, value);
    }

    void writeToDigital(int pinNumber, int value) {
      digitalWrite(pinNumber, value);
    }

    void writeHIGHToDigital(int pinNumber) {
      digitalWrite(pinNumber, HIGH);
    }

    void writeLOWToDigital(int pinNumber) {
      digitalWrite(pinNumber, LOW);
    }

    void setBit(byte& byteToBeChanged, int bitPosition) {
      bitSet(byteToBeChanged, bitPosition);
    }

    void clearBit(byte& byteToBeChanged, int bitPosition) {
      bitClear(byteToBeChanged, bitPosition);
    }

    void shift(int dataPin, int clockPin, byte shiftRegisterByte) {
      shiftOut(dataPin, clockPin, LSBFIRST, shiftRegisterByte);
    }

    void delayFor(int milliseconds) {
      delay(milliseconds);
    }

    void print(int value) {
      Serial.print(value);
    }

    void print(char* value) {
      Serial.print(value);
    }

    void println(int value) {
      Serial.println(value);
    }

    void println(char* value) {
      Serial.println(value);
    }
};

class ArduinoStandardFunctions : public IStandardFunctions {
  public:
    long maxValue(long firstValue, long secondValue) {
      return max(firstValue, secondValue);
    }

    long minValue(long firstValue, long secondValue) {
      return min(firstValue, secondValue);
    }

    long mapValue(long value, long inLower, long inUpper, long outLower, long outUpper) {
      return map(value, inLower, inUpper, outLower, outUpper);
    }

    long getCurrentMilliseconds() {
      return millis();
    }

    int nextRandomInt() {
      return rand();
    }

    int nextRandomIntInBounds(int lower, int upper) 
    {
        return lower + rand() % ( upper - lower + 1 );
    }
};

ArduinoWrapper *arduinoEnv = new ArduinoWrapper();
IStandardFunctions *arduinoStdFunctions = new ArduinoStandardFunctions();

// LED heart actor initialization
const Led ledsOnShiftRegister1[7] = {
  Led(1, 6),
  Led(2, 0),
  Led(3, 1),
  Led(4, 2),
  Led(5, 3),
  Led(6, 4),
  Led(7, 5)
};

const Led ledsOnShiftRegister2[7] = {
  Led(1, 2),
  Led(2, 1),
  Led(3, 0),
  Led(4, 3),
  Led(5, 4),
  Led(6, 5),
  Led(7, 6)
};

LedShiftRegisterPins shiftRegister1Pins = { 10, 11, 8, 9 };
LedShiftRegisterPins shiftRegister2Pins = { 5, 6, 4, 3 };
LedShiftRegister *shiftRegister1 = new LedShiftRegister(shiftRegister1Pins, ledsOnShiftRegister1, new LeftRegisterLevelActivator(), new LeftRegisterColumnActivator(), arduinoEnv);
LedShiftRegister *shiftRegister2 = new LedShiftRegister(shiftRegister2Pins, ledsOnShiftRegister2, new RightRegisterLevelActivator(), new RightRegisterColumnActivator(), arduinoEnv);
LedHeart *ledHeart = new LedHeart(arduinoStdFunctions, shiftRegister1, shiftRegister2);


// Light show initialization
LightShow lightShow = LightShow(arduinoEnv, ledHeart);

// Movement show initialization
const float changeLightsThreshold = 12000.0;
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
Gyroscope *gyroscope = new Gyroscope(MPU_ADDR);
MovementShow movementShow = MovementShow(ledHeart, gyroscope, arduinoEnv, changeLightsThreshold);


// Bass filter initialization
const int MICROPHONE_ANALOG_INPUT_PIN = A1;
const int MICROPHONE_ENABLED_PIN = 12;
const int applyToVolumneIterations = 1000;
LowPassSampler* floatLowPassSampler = new LowPassSampler(new Microphone(arduinoEnv, MICROPHONE_ANALOG_INPUT_PIN, MICROPHONE_ENABLED_PIN),
    new FloatBasedLowPassFilter(),
    arduinoStdFunctions,
    applyToVolumneIterations);
const int samplesN = 25;
BassFilterShow bassFilterShow = BassFilterShow(ledHeart, floatLowPassSampler, samplesN);


// Arduino functions
void setup()
{
  arduinoEnv -> beginSerial(9600);

  floatLowPassSampler -> initializeMicrophone();
  ledHeart -> initialize();
  movementShow.initialize();
  ledHeart -> turnOnAll();
}

void loop()
{
  //bassFilterShow.executeIteration(550);
  movementShow.executeIteration();
  //lightShow.execute();
}
