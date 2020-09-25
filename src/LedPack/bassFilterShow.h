#ifndef bassfilterShow_h
#define bassfilterShow_h

#include "ledHeart.h"
#include "lowPassSampler.h"

class BassFilterShow {
 private:
  LowPassSampler *lowPassSampler;
  LedHeart *ledHeart;
  int lowPassFilterSamples;

 public:
  BassFilterShow(LedHeart *ledHeart, LowPassSampler *lowPassSampler, int lowPassFilterSamples = 25);

  void executeIteration(int changeLightsThresHold);
};

#endif
