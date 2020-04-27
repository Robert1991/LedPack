#include "lowPassFilter.h"

int IntegerBasedLowPassFilter::filter_taps[LOWPASSFILTER_TAP_NUM] = {
  286,
  160,
  -305,
  -1183,
  -2063,
  -2224,
  -1020,
  1639,
  5045,
  7919,
  9044,
  7919,
  5045,
  1639,
  -1020,
  -2224,
  -2063,
  -1183,
  -305,
  160,
  286
};

IntegerBasedLowPassFilter::IntegerBasedLowPassFilter() {
  int i;
  for(i = 0; i < LOWPASSFILTER_TAP_NUM; ++i)
    this->history[i] = 0;
  this->last_index = 0;
}

void IntegerBasedLowPassFilter::put(int& input) {
  this->history[this->last_index++] = input;
  if(this->last_index == LOWPASSFILTER_TAP_NUM)
    this->last_index = 0;
}

int IntegerBasedLowPassFilter::get() {
  long long acc = 0;
  int index = this->last_index, i;
  for(i = 0; i < LOWPASSFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : LOWPASSFILTER_TAP_NUM-1;
    acc += (long long)this->history[index] * filter_taps[i];
  };
  return acc >> 16;
}
