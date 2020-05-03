#include "gyroscope.h"
#include "microphone.h"
#include "lowPassFilter.h"
#include "lowPassSampler.h"
#include "shiftregister.h"
#include "Wire.h"
#include "ledHeart.h"
#include "lightShow.h"

// LED heart actor initialization
Led ledsOnShiftRegister1[7] = {
  Led(1, 6),
  Led(2, 0),
  Led(3, 1),
  Led(4, 2),
  Led(5, 3),
  Led(6, 4),
  Led(7, 5)
};

Led ledsOnShiftRegister2[7] = {
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
LightShow lightShow = LightShow(ledHeart);


// Gyroscope
class LedBrightnessAccelerationRatioMapper : public IAccelerationRatioMapper {
  private:
    const int MAX_BRIGHTNESS = 255;
    int ledBrightnessMagnifier;
    int magnifierThreshold;

  public:
    LedBrightnessAccelerationRatioMapper(int ledBrightnessMagnifier, int magnifierThreshold) {
      this -> ledBrightnessMagnifier = ledBrightnessMagnifier;
      this -> magnifierThreshold = magnifierThreshold;
    }

    virtual int mapToInt(float accelerationRatio) {
      int brightness =  accelerationRatio * MAX_BRIGHTNESS;

      if (brightness > magnifierThreshold) {
        brightness = brightness + ledBrightnessMagnifier;
      }

      if (brightness > MAX_BRIGHTNESS) {
        return MAX_BRIGHTNESS;
      } else {
        return brightness;
      }
    }
};

const float changeLightsThreshold = 12000.0;
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
Gyroscope gyroScope = Gyroscope(MPU_ADDR);
IAccelerationRatioMapper *accelerationBrightnessMapper = new LedBrightnessAccelerationRatioMapper(35, 10);
AccelerationMeasurementVector currentAcceleration = AccelerationMeasurementVector::defaultVector();
AccelerationMeasurementVector formerAcceleration = AccelerationMeasurementVector::defaultVector();

// Bass filter initialization
const int MICROPHONE_ANALOG_INPUT_PIN = A1;
const int MICROPHONE_ENABLED_PIN = 12;
const int samplesN = 25;
const int applyToVolumneIterations = 1000;
LowPassSampler* floatLowPassSampler = new LowPassSampler(new Microphone(MICROPHONE_ANALOG_INPUT_PIN, MICROPHONE_ENABLED_PIN),
    new FloatBasedLowPassFilter(),
    applyToVolumneIterations);

void setup()
{
  Serial.begin(9600);
  floatLowPassSampler -> initializeMicrophone();
  ledHeart -> initialize();
  gyroScope.wakeUp();

  currentAcceleration = gyroScope.measureAcceleration();
  formerAcceleration = gyroScope.measureAcceleration();

  ledHeart -> turnOnAll();
}

void loop()
{
  //bassFilterShow();
  //movementShow();
  lightShow.execute();
}

void bassFilterShow() {
  int lvl = floatLowPassSampler -> read(samplesN);
  float brightnessFactor = lvl / 1023.0;

  if (lvl > 550) {
    ledHeart -> turnOnRandomly(2);
  }

  ledHeart -> toggleBrightness((brightnessFactor * 255));
}

void movementShow() {
  currentAcceleration = gyroScope.measureAcceleration();
  AccerlationVectorDifference vectorDifference = currentAcceleration.euclideanDistanceTo2(formerAcceleration);
  ledHeart -> toggleBrightness(vectorDifference.mapAccelerationRatioTo(accelerationBrightnessMapper));

  if (vectorDifference.overThreshold(changeLightsThreshold)) {
    ledHeart -> turnOnRandomly(4);
  }

  delay(10);
  formerAcceleration = currentAcceleration;
}
