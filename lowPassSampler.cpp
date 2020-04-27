#include "lowPassSampler.h"

LowPassSampler::LowPassSampler(Microphone *microphone, ILowPassFilter *lowPassFilter, int applyToVolumneIterations) {
  this -> microphone = microphone;
  this -> lowPassFilter = lowPassFilter;
  this -> applyToVolumneIterations = applyToVolumneIterations;
}

int LowPassSampler::read(int samplingRate) {
  applyToVolumeIfNecassary();
  int peak = readPeakFrom(samplingRate);
  maxPeak = max(maxPeak, peak);
  minPeak = min(minPeak, peak);
  return map(peak, minPeak, maxPeak, MINIMUM_PEAK, MAXIMUM_PEAK);
}

int LowPassSampler::readPeakFrom(int samples) {
  int peak = 0;
  for (int k = 0; k < samples; k++) {
    int val = this -> microphone -> readAnalog();
    this -> lowPassFilter ->put(val);

    int filtered = lowPassFilter->get();
    peak = max(peak, filtered);
  }
  return peak;
}

void LowPassSampler::applyToVolumeIfNecassary() {
  this -> calledInCurrentIteration++;

  if (this -> calledInCurrentIteration == this -> applyToVolumneIterations) {
    this -> maxPeak = 0;
    this -> minPeak = 1023;
    calledInCurrentIteration = 0;
  }
}
