#include "lowPassFilter.h"

int IntegerBasedLowPassFilter::filter_taps[INTEGER_BASED_LOWPASSFILTER_TAP_NUM] = {
    238, 42, -346, -1017, -1728, -2051, -1537, 35, 2473, 5159, 7256, 8049, 7256, 5159, 2473, 35, -1537, -2051, -1728, -1017, -346, 42, 238};

IntegerBasedLowPassFilter::IntegerBasedLowPassFilter() {
  int i;
  for (i = 0; i < INTEGER_BASED_LOWPASSFILTER_TAP_NUM; i++) this->history[i] = 0;
  this->last_index = 0;
}

void IntegerBasedLowPassFilter::put(int input) {
  this->history[last_index] = input;
  last_index++;
  if (this->last_index == INTEGER_BASED_LOWPASSFILTER_TAP_NUM) this->last_index = 0;
}

int IntegerBasedLowPassFilter::get() {
  long long acc = 0;
  int index = this->last_index, i;
  for (i = 0; i < INTEGER_BASED_LOWPASSFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index - 1 : INTEGER_BASED_LOWPASSFILTER_TAP_NUM - 1;
    acc += (long long)this->history[index] * filter_taps[i];
  };
  return acc >> 16;
}

float FloatBasedLowPassFilter::filter_taps[FLOAT_BASED_LOWPASSFILTER_TAP_NUM] = {
    0.010846807119614697,  0.015208111089537261, 0.008282448904887219,  -0.017383433599185996, -0.052973604369240555, -0.07315266507531389,
    -0.048940825894655864, 0.030948034234790128, 0.14561333634964552,   0.24762366755104587,   0.2884380300115189,    0.24762366755104587,
    0.14561333634964552,   0.030948034234790128, -0.048940825894655864, -0.07315266507531389,  -0.052973604369240555, -0.017383433599185996,
    0.008282448904887219,  0.015208111089537261, 0.010846807119614697};

FloatBasedLowPassFilter::FloatBasedLowPassFilter() {
  int i;
  for (i = 0; i < FLOAT_BASED_LOWPASSFILTER_TAP_NUM; ++i) this->history[i] = 0;
  this->last_index = 0;
}

void FloatBasedLowPassFilter::put(int input) {
  float filterInput = (float)input;
  this->history[last_index] = filterInput;
  last_index++;
  if (this->last_index == FLOAT_BASED_LOWPASSFILTER_TAP_NUM) this->last_index = 0;
}

int FloatBasedLowPassFilter::get() {
  float acc = 0;
  int index = this->last_index, i;
  for (i = 0; i < FLOAT_BASED_LOWPASSFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index - 1 : FLOAT_BASED_LOWPASSFILTER_TAP_NUM - 1;
    acc += (long long)this->history[index] * filter_taps[i];
  };

  int accInInt = (int)acc;
  return accInInt;
}
