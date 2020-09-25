#include "lowPassSampler.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mockClasses.h"

using ::testing::_;
using ::testing::Eq;
using ::testing::Exactly;
using ::testing::Mock;
using ::testing::Return;

class LowPassSamplerTest : public ::testing::Test {
 public:
  MicrophoneMock microphoneMock;
  LowPassFilterMock lowPassFilterMock;
  IStandardFunctions *stdFunctions;
  IStandardFunctionsMock stdFunctionsMock;

  LowPassSampler *lowPassSampler;
  LowPassSampler *lowPassSamplerWithStdFunctionMock;

  int APPLY_TO_VOLUME_ITERATIONS = 3;

 protected:
  void SetUp() override {
    stdFunctions = new STDNamespaceFunctions();
    lowPassSampler = new LowPassSampler(&microphoneMock, &lowPassFilterMock, stdFunctions, APPLY_TO_VOLUME_ITERATIONS);
    lowPassSamplerWithStdFunctionMock = new LowPassSampler(&microphoneMock, &lowPassFilterMock, &stdFunctionsMock, APPLY_TO_VOLUME_ITERATIONS);
  }

  void TearDown() override {
    Mock::VerifyAndClearExpectations(&microphoneMock);
    Mock::VerifyAndClearExpectations(&lowPassFilterMock);
    Mock::VerifyAndClearExpectations(&stdFunctionsMock);
  }
};

TEST_F(LowPassSamplerTest, testThatTheLowPassSamplerInitializesItsMicrophone) {
  EXPECT_CALL(microphoneMock, init()).Times(Exactly(1));
  lowPassSampler->initializeMicrophone();
}

TEST_F(LowPassSamplerTest, testThatTheLowPassSamplerReadsNSamplesFromTheMicrophoneAndHandsThemOverToTheFilterToDetectTheFilteredValue) {
  EXPECT_CALL(microphoneMock, readAnalog()).Times(Exactly(3)).WillRepeatedly(Return(11));
  EXPECT_CALL(lowPassFilterMock, put(Eq(11))).Times(Exactly(3));
  // Max peak is initialized with 0
  // Min peak is initialized with 1023
  EXPECT_CALL(lowPassFilterMock, get()).Times(Exactly(3)).WillOnce(Return(15)).WillOnce(Return(100)).WillOnce(Return(50));
  // Max peak and min peak will be 100 after that
  EXPECT_THAT(lowPassSampler->read(3), Eq(stdFunctions->mapValue(100, 100, 100, MINIMUM_PEAK, MAXIMUM_PEAK)));
}

TEST_F(LowPassSamplerTest, testThatTheLowPassSamplerRepeatedlyMapTheDetectedValuesToItsMinAndMaxPeakToDetectTheFilteredValues) {
  EXPECT_CALL(microphoneMock, readAnalog()).Times(Exactly(6)).WillRepeatedly(Return(11));
  EXPECT_CALL(lowPassFilterMock, put(Eq(11))).Times(Exactly(6));
  // Max peak is initialized with 0
  // Min peak is initialized with 1023
  EXPECT_CALL(lowPassFilterMock, get())
      .Times(Exactly(6))
      // First interation
      .WillOnce(Return(15))
      .WillOnce(Return(100))
      .WillOnce(Return(50))
      // Second interation
      .WillOnce(Return(15))
      .WillOnce(Return(250))
      .WillOnce(Return(50));
  // Max peak and min peak will be 100 after that
  EXPECT_THAT(lowPassSampler->read(3), Eq(stdFunctions->mapValue(100, 100, 100, MINIMUM_PEAK, MAXIMUM_PEAK)));
  // Min peak will still be 100 and max peak will be 250 after that
  EXPECT_THAT(lowPassSampler->read(3), Eq(stdFunctions->mapValue(250, 100, 250, MINIMUM_PEAK, MAXIMUM_PEAK)));
}

TEST_F(LowPassSamplerTest, testThatTheLowPassSamplerAppliesToTheVolumeByResettingMinAndMaxPeakAfterTheApplyVolumeIterationsAreMet) {
  EXPECT_CALL(microphoneMock, readAnalog()).Times(Exactly(9)).WillRepeatedly(Return(11));
  EXPECT_CALL(lowPassFilterMock, put(Eq(11))).Times(Exactly(9));
  // Max peak is initialized with 0
  // Min peak is initialized with 1023
  EXPECT_CALL(lowPassFilterMock, get())
      .Times(Exactly(9))
      // First iteration
      .WillOnce(Return(15))
      .WillOnce(Return(100))
      .WillOnce(Return(50))
      // Second iteration
      .WillOnce(Return(15))
      .WillOnce(Return(250))
      .WillOnce(Return(50))
      // Third iteration
      .WillOnce(Return(15))
      .WillOnce(Return(300))
      .WillOnce(Return(50));
  // Max peak and min peak will be 100 after that
  EXPECT_THAT(lowPassSampler->read(3), Eq(stdFunctions->mapValue(100, 100, 100, MINIMUM_PEAK, MAXIMUM_PEAK)));
  // Min peak will still be 100 and max peak will be 250 after that
  EXPECT_THAT(lowPassSampler->read(3), Eq(stdFunctions->mapValue(250, 100, 250, MINIMUM_PEAK, MAXIMUM_PEAK)));
  // Min peak and max peak will be reset in this iteration
  // The detected min and max peak will again be the value of the measured peak
  EXPECT_THAT(lowPassSampler->read(3), Eq(stdFunctions->mapValue(300, 300, 300, MINIMUM_PEAK, MAXIMUM_PEAK)));
}

TEST_F(LowPassSamplerTest,
       testThatTheLowPassSamplerDetectsTheFilterFrequencyByReadingTheGivenSamplesFromTheMicrophoneWhileMeasuringTheTimeDifferenceForThat) {
  int START_TIME_MILLIS = 1000;
  int ENDTIME_TIME_MILLIS = 2000;
  int SAMPLES = 10;

  EXPECT_CALL(microphoneMock, readAnalog()).Times(Exactly(SAMPLES)).WillRepeatedly(Return(11));
  EXPECT_CALL(lowPassFilterMock, put(Eq(11))).Times(Exactly(SAMPLES));
  EXPECT_CALL(lowPassFilterMock, get()).Times(Exactly(SAMPLES)).WillRepeatedly(Return(15));
  EXPECT_CALL(stdFunctionsMock, getCurrentMilliseconds()).Times(Exactly(2)).WillOnce(Return(START_TIME_MILLIS)).WillOnce(Return(ENDTIME_TIME_MILLIS));
  EXPECT_CALL(stdFunctionsMock, maxValue(_, _)).Times(Exactly(10)).WillRepeatedly(Return(15));

  EXPECT_THAT(lowPassSamplerWithStdFunctionMock->detectFilterFrequency(SAMPLES), Eq((SAMPLES * 1000) / (ENDTIME_TIME_MILLIS - START_TIME_MILLIS)));
}