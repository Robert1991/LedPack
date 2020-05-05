#ifndef lowPassSampler_h
#define lowPassSampler_h

#include "microphone.h"
#include "lowPassFilter.h"
#include "arduinoWrapper.h"

const int MINIMUM_PEAK = 0;
const int MAXIMUM_PEAK = 1023;

class LowPassSampler {
  private:
    Microphone *microphone;
    ILowPassFilter *lowPassFilter;
    IStandardFunctions *stdFunctions;
    int applyToVolumneIterations;

    int calledInCurrentIteration = 0;
    int minPeak = 1023;
    int maxPeak = 0;

    void applyToVolumeIfNecassary();
    int readPeakFrom(int samples);
  
  public:
    LowPassSampler(Microphone *microphone, ILowPassFilter *lowPassFilter, IStandardFunctions *stdFunctions, int applyToVolumneIterations);
    void initializeMicrophone();
    int read(int samplingRate);
    float detectFilterFrequency(int samples);
    
};

#endif
