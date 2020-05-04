#ifndef movementShow_h
#define movementShow_h

#include "gyroscope.h"
#include "ledHeart.h"
#include "arduinoWrapper.h"

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

class MovementShow {
  private:
    IAccelerationRatioMapper *accelerationBrightnessMapper;
    AccelerationMeasurementVector currentAcceleration;
    AccelerationMeasurementVector formerAcceleration;

    LedHeart *ledHeart;
    Gyroscope *gyroscope;
    IArduinoWrapper *arduinoEnv;
    float changeLightsThreshold;
    
  public:
    MovementShow(LedHeart *ledHeart, Gyroscope *gyroscope, IArduinoWrapper *arduinoEnv, float changeLightsThreshold = 12000.0);

    void initialize();

    void executeIteration();
};

#endif
