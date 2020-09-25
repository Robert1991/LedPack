#include "lowPassSampler.h"

LowPassSampler::LowPassSampler(Microphone *microphone, ILowPassFilter *lowPassFilter, IStandardFunctions *stdFunctions,
                               int applyToVolumneIterations) {
  this->microphone = microphone;
  this->lowPassFilter = lowPassFilter;
  this->applyToVolumneIterations = applyToVolumneIterations;
  this->stdFunctions = stdFunctions;
}

void LowPassSampler::initializeMicrophone() { this->microphone->init(); }

int LowPassSampler::read(int samplingRate) {
  applyToVolumeIfNecassary();
  int peak = readPeakFrom(samplingRate);
  maxPeak = stdFunctions->maxValue(maxPeak, peak);
  minPeak = stdFunctions->minValue(minPeak, peak);
  return stdFunctions->mapValue(peak, minPeak, maxPeak, MINIMUM_PEAK, MAXIMUM_PEAK);
}

int LowPassSampler::readPeakFrom(int samples) {
  int peak = 0;
  for (int k = 0; k < samples; k++) {
    int val = this->microphone->readAnalog();
    this->lowPassFilter->put(val);
    int filtered = lowPassFilter->get();
    peak = stdFunctions->maxValue(peak, filtered);
  }
  return peak;
}

void LowPassSampler::applyToVolumeIfNecassary() {
  this->calledInCurrentIteration++;

  if (this->calledInCurrentIteration == this->applyToVolumneIterations) {
    this->maxPeak = 0;
    this->minPeak = 1023;
    calledInCurrentIteration = 0;
  }
}

float LowPassSampler::detectFilterFrequency(int samples) {
  long start = stdFunctions->getCurrentMilliseconds();
  readPeakFrom(samples);
  long end = stdFunctions->getCurrentMilliseconds();
  float freq = ((float)samples * (float)1000) / ((float)end - (float)start);
  return freq;
}
