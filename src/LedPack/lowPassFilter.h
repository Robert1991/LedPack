#ifndef lowPassFilter_h
#define lowPassFilter_h

const int INTEGER_BASED_LOWPASSFILTER_TAP_NUM = 23;
const int FLOAT_BASED_LOWPASSFILTER_TAP_NUM = 21;

class ILowPassFilter {
  public:
    unsigned int last_index;
  
    virtual void put(int input) = 0;
    virtual int get() = 0;
};

class IntegerBasedLowPassFilter : public ILowPassFilter {
  public:
    static int filter_taps[INTEGER_BASED_LOWPASSFILTER_TAP_NUM];
    int history[INTEGER_BASED_LOWPASSFILTER_TAP_NUM];
    IntegerBasedLowPassFilter();
    void put(int input);
    virtual int get();
};

class FloatBasedLowPassFilter : public ILowPassFilter {
  public:
    static float filter_taps[FLOAT_BASED_LOWPASSFILTER_TAP_NUM];
    int history[FLOAT_BASED_LOWPASSFILTER_TAP_NUM];
    FloatBasedLowPassFilter();
    void put(int input);
    virtual int get();
};

#endif
