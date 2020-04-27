#include "gyroscope.h"
#include "microphone.h"
#include "lowPassFilter.h"
#include "lowPassSampler.h"
#include "shiftregister.h"
#include "Wire.h"


bool alreadyTurnedOn(int *turnedOn, int arrayLength, int ledIndex) {
  for (int i = 0; i < arrayLength; i++) {
    if (turnedOn[i] == ledIndex) {
      return true;
    }
  }

  return false;
}


void turnOnLedsOnHeartRandomly(int minLedsTurnedOn) {
  if (minLedsTurnedOn <= 14) {
    int numberOfLedsTurnedOn = minLedsTurnedOn + ( rand() % ( 14 - minLedsTurnedOn + 1 ) );
    int turnedOnLeds[numberOfLedsTurnedOn];

    for (int i = 0; i < numberOfLedsTurnedOn; i++) {
      int nextLed = rand() % 14;
      while (alreadyTurnedOn(turnedOnLeds, numberOfLedsTurnedOn, nextLed)) {
        nextLed = rand() % 14;
      }
      turnedOnLeds[i] = nextLed;
    }

    shiftRegister1.turnOffAll();
    shiftRegister2.turnOffAll();

    for (int i = 0; i < sizeof(turnedOnLeds); i++) {
      int turnedOnLedIndex = turnedOnLeds[i];

      if (turnedOnLedIndex >= 7) {
        shiftRegister2.turnOn(turnedOnLedIndex - 7);
      } else {
        shiftRegister1.turnOn(turnedOnLedIndex);
      }
    }
  }
}


// LED heart actor initialization
class Register1ColumnActivator : public ColumnActivator {
  public:
    void turnOnColumn(int column, Led *leds) {
      if (column == 1) {
        leds[4].turnOn();
      } else if (column == 2) {
        leds[3].turnOn();
        leds[2].turnOn();
        leds[5].turnOn();
      } else if (column == 3) {
        leds[1].turnOn();
        leds[6].turnOn();
      } else {
        leds[0].turnOn();
      }
    }

    void turnOffColumn(int column, Led *leds) {
      if (column == 1) {
        leds[4].turnOff();
      } else if (column == 2) {
        leds[3].turnOff();
        leds[2].turnOff();
        leds[5].turnOff();
      } else if (column == 3) {
        leds[1].turnOff();
        leds[6].turnOff();
      } else {
        leds[0].turnOff();
      }
    }
};

class Register2ColumnActivator : public ColumnActivator {
  public:
    void turnOnColumn(int column, Led *leds) {
      if (column == 1) {
        leds[3].turnOn();
      } else if (column == 2) {
        leds[2].turnOn();
        leds[4].turnOn();
        leds[5].turnOn();
      } else if (column == 3) {
        leds[1].turnOn();
        leds[6].turnOn();
      } else {
        leds[0].turnOn();
      }
    }

    void turnOffColumn(int column, Led *leds) {
      if (column == 1) {
        leds[3].turnOff();
      } else if (column == 2) {
        leds[2].turnOff();
        leds[4].turnOff();
        leds[5].turnOff();
      } else if (column == 3) {
        leds[1].turnOff();
        leds[6].turnOff();
      } else {
        leds[0].turnOff();
      }
    }
};


class Register1LevelActivator : public LevelActivator {
  public:
    void turnOnLevel(int level, Led *leds) {
      if (level == 1) {
        leds[0].turnOn();
      } else if (level == 2) {
        leds[1].turnOn();
      } else if (level == 3) {
        leds[2].turnOn();
      } else if (level == 4) {
        leds[3].turnOn();
      } else if (level == 5) {
        leds[4].turnOn();
        leds[6].turnOn();
      } else {
        leds[5].turnOn();
      }
    }

    void turnOffLevel(int level, Led *leds) {
      if (level == 1) {
        leds[0].turnOff();
      } else if (level == 2) {
        leds[1].turnOff();
      } else if (level == 3) {
        leds[2].turnOff();
      } else if (level == 4) {
        leds[3].turnOff();
      } else if (level == 5) {
        leds[4].turnOff();
        leds[6].turnOff();
      } else {
        leds[5].turnOff();
      }
    }
};


class Register2LevelActivator : public LevelActivator {
  public:
    void turnOnLevel(int level, Led *leds) {
      if (level == 1) {
        // there is no level 1 for this
      } else if (level == 2) {
        leds[6].turnOn();
      } else if (level == 3) {
        leds[5].turnOn();
      } else if (level == 4) {
        leds[0].turnOn();
        leds[4].turnOn();
      } else if (level == 5) {
        leds[1].turnOn();
        leds[3].turnOn();
      } else {
        leds[2].turnOn();
      }
    }

    void turnOffLevel(int level, Led *leds) {
      if (level == 1) {
        // there is no level 1 for this
      } else if (level == 2) {
        leds[6].turnOff();
      } else if (level == 3) {
        leds[5].turnOff();
      } else if (level == 4) {
        leds[0].turnOff();
        leds[4].turnOff();
      } else if (level == 5) {
        leds[1].turnOff();
        leds[3].turnOff();
      } else {
        leds[2].turnOff();
      }
    }
};

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
LedShiftRegister shiftRegister1 = LedShiftRegister(shiftRegister1Pins, ledsOnShiftRegister1, new Register1LevelActivator(), new Register1ColumnActivator());
LedShiftRegister shiftRegister2 = LedShiftRegister(shiftRegister2Pins, ledsOnShiftRegister2, new Register2LevelActivator(), new Register2ColumnActivator());

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

Microphone* microphone = new Microphone(MICROPHONE_ANALOG_INPUT_PIN, MICROPHONE_ENABLED_PIN);
ILowPassFilter* lowPassFilter = new IntegerBasedLowPassFilter();
LowPassSampler* lowPassSampler = new LowPassSampler(microphone, lowPassFilter, 1000);

void setup()
{
  microphone -> init();

  shiftRegister1.initializePins();
  shiftRegister2.initializePins();

  gyroScope.wakeUp();

  currentAcceleration = gyroScope.measureAcceleration();
  formerAcceleration = gyroScope.measureAcceleration();

  shiftRegister1.turnOnAll();
  shiftRegister2.turnOnAll();
  
  Serial.begin(9600);
}

void loop()
{
  bassFilterShow();
  //movementShow();
  //soundShow();
  //lightShow();
}

void bassFilterShow() {
  int lvl = lowPassSampler -> read(samplesN);
  if (lvl > 750) {
    float brightnessFactor = lvl / 1023.0;
    shiftRegister1.toggleBrightness((brightnessFactor * 255));
    shiftRegister2.toggleBrightness((brightnessFactor * 255));
    turnOnLedsOnHeartRandomly(7);
  } else {
    shiftRegister1.toggleBrightness(0);
    shiftRegister2.toggleBrightness(0);
  }
}

void movementShow() {
  currentAcceleration = gyroScope.measureAcceleration();
  AccerlationVectorDifference vectorDifference = currentAcceleration.euclideanDistanceTo2(formerAcceleration);
  shiftRegister1.toggleBrightness(vectorDifference.mapAccelerationRatioTo(accelerationBrightnessMapper));
  shiftRegister2.toggleBrightness(vectorDifference.mapAccelerationRatioTo(accelerationBrightnessMapper));

  if (vectorDifference.overThreshold(changeLightsThreshold)) {
    turnOnLedsOnHeartRandomly(4);
  }

  delay(10);
  formerAcceleration = currentAcceleration;

}

void soundShow() {
  int Analog;
  int Digital;

  Analog = microphone -> readAnalog();
  Digital = microphone -> readDigital();

  if (Digital == HIGH) {
    shiftRegister1.turnOnAll();
    shiftRegister2.turnOnAll();
    delay(10);
  } else {
    shiftRegister1.turnOffAll();
    shiftRegister2.turnOffAll();
  }
}

void lightShow() {
  // Turns on all leds in a row
  for (int led = 0; led < 7; led++) {
    shiftRegister1.turnOn(led);
    delay(50);
  }

  for (int led = 0; led < 7; led++) {
    shiftRegister2.turnOn(led);
    delay(50);
  }
  //

  // Let the leds blink 6 times
  for (int i = 0; i < 6; i++) {
    shiftRegister1.turnOnAll();
    shiftRegister2.turnOnAll();
    delay(500);
    shiftRegister1.turnOffAll();
    shiftRegister2.turnOffAll();
    delay(100);
  }
  //

  // Turns on all leds in ascending id order
  for (int j = 0; j < 4; j++) {
    for (int led = 0; led < 7; led++) {
      shiftRegister1.turnOn(led);
      shiftRegister2.turnOn(led);
      delay(50);
    }

    shiftRegister1.turnOffAll();
    shiftRegister2.turnOffAll();

    for (int led = 6; led >= 0; led--) {
      shiftRegister1.turnOn(led);
      shiftRegister2.turnOn(led);
      delay(50);
    }

    shiftRegister1.turnOffAll();
    shiftRegister2.turnOffAll();
  }
  //

  // turns the levels on/off seperatley
  for (int i = 0; i < 3; i++) {
    for (int j = 1; j <= MAX_LEVEL; j++) {
      shiftRegister1.turnOnLevel(j);
      shiftRegister2.turnOnLevel(j);
      delay(100);
    }

    for (int i = 1; i <= MAX_LEVEL; i++) {
      shiftRegister1.turnOffLevel(i);
      shiftRegister2.turnOffLevel(i);
      delay(100);
    }
  }

  for (int times = 0; times < 3; times++) {
    for (int i = 0; i < 7; i++) {
      if (i >= 1) {
        shiftRegister1.turnOff(i - 1);
        shiftRegister2.turnOff(i - 1);
      }

      shiftRegister1.turnOn(i);
      shiftRegister2.turnOn(i);

      delay(50);
    }

    shiftRegister1.turnOffAll();
    shiftRegister2.turnOffAll();
  }

  for (int times = 0; times < 3; times++) {
    for (int i = 6; i >= 0; i--) {
      if (i < 6) {
        shiftRegister1.turnOff(i + 1);
        shiftRegister2.turnOff(i + 1);
      }

      shiftRegister1.turnOn(i);
      shiftRegister2.turnOn(i);

      delay(50);
    }

    shiftRegister1.turnOffAll();
    shiftRegister2.turnOffAll();
  }



  for (int times = 0; times < 5; times++) {
    for (int i = 1; i <= MAX_LEVEL; i++) {
      if (i >= 1) {
        shiftRegister1.turnOffLevel(i - 1);
        shiftRegister2.turnOffLevel(i - 1);
      }

      shiftRegister1.turnOnLevel(i);
      shiftRegister2.turnOnLevel(i);

      delay(50);
    }

    shiftRegister1.turnOffAll();
    shiftRegister2.turnOffAll();
  }

  for (int i = 1; i <= MAX_LEVEL; i++) {
    shiftRegister1.turnOnLevel(i);
    shiftRegister2.turnOnLevel(i);
    delay(125);
  }

  delay(250);

  for (int i = MAX_LEVEL; i >= 1; i--) {
    shiftRegister1.turnOffLevel(i);
    shiftRegister2.turnOffLevel(i);
    delay(50);
  }

  for (int j = 0; j < 5; j++) {
    for (int i = 1; i <= MAX_COLUMNS; i++) {
      if (i >= 2) {
        shiftRegister1.turnOffColumn(i - 1);
        shiftRegister2.turnOffColumn(i - 1);
      }

      shiftRegister1.turnOnColumn(i);
      shiftRegister2.turnOnColumn(i);
      delay(75);
    }

    shiftRegister1.turnOffAll();
    shiftRegister2.turnOffAll();
  }

  for (int j = 0; j < 5; j++) {
    for (int i = 1; i <= MAX_COLUMNS; i++) {
      shiftRegister1.turnOnColumn(i);
      shiftRegister2.turnOnColumn(i);
      delay(75);
    }

    shiftRegister1.turnOffAll();
    shiftRegister2.turnOffAll();
  }

  for (int i = 1; i <= MAX_COLUMNS; i++) {
    shiftRegister1.turnOnColumn(i);
    shiftRegister2.turnOnColumn(i);
    delay(200);
  }

  delay(500);

  for (int j = MAX_COLUMNS; j > 0; j--) {
    shiftRegister1.turnOffColumn(j);
    shiftRegister2.turnOffColumn(j);
    delay(125);
  }

  shiftRegister1.turnOffAll();
  shiftRegister2.turnOffAll();

  for (int j = 1; j <= MAX_COLUMNS; j++) {
    if (j > 1) {
      shiftRegister2.turnOffColumn(j - 1);
    }
    shiftRegister2.turnOnColumn(j);
    delay(75);
  }

  shiftRegister2.turnOffAll();

  for (int j = MAX_COLUMNS; j > 0; j--) {
    if (j < MAX_COLUMNS) {
      shiftRegister1.turnOffColumn(j + 1);
    }
    shiftRegister1.turnOnColumn(j);
    delay(75);
  }

  shiftRegister1.turnOffAll();

  for (int j = 1; j <= MAX_COLUMNS; j++) {
    if (j > 0) {
      shiftRegister1.turnOffColumn(j - 1);
    }
    shiftRegister1.turnOnColumn(j);
    delay(75);
  }

  shiftRegister1.turnOffAll();

  for (int j = MAX_COLUMNS; j > 0; j--) {
    if (j < MAX_COLUMNS) {
      shiftRegister2.turnOffColumn(j + 1);
    }
    shiftRegister2.turnOnColumn(j);
    delay(75);
  }

  shiftRegister2.turnOffAll();
}
