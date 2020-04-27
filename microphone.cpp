#include "microphone.h"
#include "Arduino.h"

Microphone::Microphone(int microphonePin, int digitalInputPin) {
  this -> microphonePin = microphonePin;
  this -> digitalInputPin = digitalInputPin;
}

void Microphone::init() {
  pinMode(this -> digitalInputPin, INPUT);
}

int Microphone::readAnalog() {
  return analogRead(microphonePin);
}

int Microphone::readDigital() {
  return digitalRead(digitalInputPin);
}
