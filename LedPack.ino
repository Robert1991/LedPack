#include "gyroscope.h"
#include "microphone.h"
#include "lowPassFilter.h"
#include "lowPassSampler.h"
#include "shiftregister.h"
#include "Wire.h"
#include "ledHeart.h"
#include "lightShow.h"
#include "movementShow.h"
#include "bassfilterShow.h"

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
LedShiftRegister *shiftRegister1 = new LedShiftRegister(shiftRegister1Pins, ledsOnShiftRegister1, new LeftRegisterLevelActivator(), new LeftRegisterColumnActivator());
LedShiftRegister *shiftRegister2 = new LedShiftRegister(shiftRegister2Pins, ledsOnShiftRegister2, new RightRegisterLevelActivator(), new RightRegisterColumnActivator());
LedHeart *ledHeart = new LedHeart(shiftRegister1, shiftRegister2);


// Light show initialization
LightShow lightShow = LightShow(ledHeart);

// Movement show initialization
const float changeLightsThreshold = 12000.0;
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
Gyroscope *gyroscope = new Gyroscope(MPU_ADDR);
MovementShow movementShow = MovementShow(ledHeart, gyroscope, changeLightsThreshold);


// Bass filter initialization
const int MICROPHONE_ANALOG_INPUT_PIN = A1;
const int MICROPHONE_ENABLED_PIN = 12;
const int applyToVolumneIterations = 1000;
LowPassSampler* floatLowPassSampler = new LowPassSampler(new Microphone(MICROPHONE_ANALOG_INPUT_PIN, MICROPHONE_ENABLED_PIN),
    new FloatBasedLowPassFilter(),
    applyToVolumneIterations);
const int samplesN = 25;
BassFilterShow bassFilterShow = BassFilterShow(ledHeart, floatLowPassSampler, samplesN);


void setup()
{
  Serial.begin(9600);
  floatLowPassSampler -> initializeMicrophone();
  ledHeart -> initialize();
  movementShow.initialize();
  ledHeart -> turnOnAll();
}

void loop()
{
  bassFilterShow.executeIteration(550);
  //movementShow.executeIteration();
  //lightShow.execute();
}
