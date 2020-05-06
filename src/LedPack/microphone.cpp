#include "microphone.h"

Microphone::Microphone(IArduinoWrapper *arduinoEnv, int microphonePin, int digitalInputPin) {
  this -> microphonePin = microphonePin;
  this -> digitalInputPin = digitalInputPin;
  this -> arduinoEnv = arduinoEnv;
}

void Microphone::init() {
  arduinoEnv -> setPinToInputMode(digitalInputPin);
}

int Microphone::readAnalog() {
  return arduinoEnv -> readAnalogFrom(microphonePin);
}

int Microphone::readDigital() {
  return arduinoEnv -> readDigitalFrom(digitalInputPin);
}
