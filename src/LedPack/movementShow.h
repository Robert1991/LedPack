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
    LedBrightnessAccelerationRatioMapper(int ledBrightnessMagnifier, int magnifierThreshold);
    int mapToInt(float accelerationRatio);
};

class LedCountOnHeartAccerlertionRationMapper : public IAccelerationRatioMapper {
  private:
    const int MAX_LEDS = HEART_LED_COUNT;

  public:
    LedCountOnHeartAccerlertionRationMapper();
    int mapToInt(float accelerationRatio);
};

class MovementShowActionMapper {
  protected:
    IAccelerationRatioMapper *ratioMapper;
    LedHeart *ledHeart;
  
  public:
    MovementShowActionMapper(LedHeart *heart, IAccelerationRatioMapper *ratioMapper);
    virtual void mapToHeart(AccerlationVectorDifference *difference) = 0;
};

class MovementShow {
  private:
    AccelerationMeasurementVector currentAcceleration;
    AccelerationMeasurementVector formerAcceleration;

    Gyroscope *gyroscope;
    
  public:
    MovementShow(Gyroscope *gyroscope);
    void initialize();
    void executeIterationWith(MovementShowActionMapper *ledHeartActionMapper);
};

class RandomLedBlinkMovementShow : public MovementShowActionMapper {
  private:
    IArduinoWrapper *arduinoEnv;
    float changeLightsThreshold;

  public:
    RandomLedBlinkMovementShow(LedHeart *heart, LedBrightnessAccelerationRatioMapper *ratioMapper, IArduinoWrapper *wrapper, float changeLightsThreshold);
    void mapToHeart(AccerlationVectorDifference *difference);
};

class SequentialHeartTurnOnShow : public MovementShowActionMapper {
  private:
    IArduinoWrapper *arduinoEnv;
    float changeLightsThreshold;

  public:
    SequentialHeartTurnOnShow(LedHeart *heart, IArduinoWrapper *wrapper);
    void mapToHeart(AccerlationVectorDifference *difference);
};

#endif
