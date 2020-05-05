#include "bassfilterShow.h"


BassFilterShow::BassFilterShow(LedHeart *ledHeart, LowPassSampler *lowPassSampler, int lowPassFilterSamples = 25) {
  this -> ledHeart = ledHeart;
  this -> lowPassSampler = lowPassSampler;
  this -> lowPassFilterSamples = lowPassFilterSamples;
}

void BassFilterShow::executeIteration(int changeLightsThresHold) {
  int lvl = lowPassSampler -> read(lowPassFilterSamples);
  float brightnessFactor = lvl / 1023.0;

  if (lvl > changeLightsThresHold) {
    ledHeart -> turnOnRandomly(2);
  }

  ledHeart -> toggleBrightness((brightnessFactor * 255));
}
