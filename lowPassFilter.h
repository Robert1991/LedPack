#ifndef lowPassFilter_h
#define lowPassFilter_h

const int LOWPASSFILTER_TAP_NUM = 21;

class ILowPassFilter {
  public:
    int history[LOWPASSFILTER_TAP_NUM];
    unsigned int last_index;
  
    LowPassFilter2();
    virtual void put(int& input) = 0;
    virtual int get() = 0;
};

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 1700 Hz

fixed point precision: 16 bits

* 0 Hz - 200 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 300 Hz - 850 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

class IntegerBasedLowPassFilter : public ILowPassFilter {
  public:
    static int filter_taps[LOWPASSFILTER_TAP_NUM];
    IntegerBasedLowPassFilter();
    void put(int& input);
    virtual int get();
};

#endif
