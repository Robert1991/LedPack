#include "movementShow.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "gyroscope.h"
#include "mockClasses.h"

using ::testing::Eq;
using ::testing::Exactly;
using ::testing::Mock;
using ::testing::Return;

class MovementShowTest : public ::testing::Test {
 public:
  GyroscopeMock gyroscopeMock;
  MovementShow *movementShow;
  MovementShowActionMapperMock actionMapperMock;
  LedHeartMock ledHeartMock;
  TestArduinoEnvironment testArduinoEnv;

 protected:
  void SetUp() override { movementShow = new MovementShow(&gyroscopeMock); }

  void TearDown() override {
    Mock::VerifyAndClearExpectations(&gyroscopeMock);
    Mock::VerifyAndClearExpectations(&actionMapperMock);
    Mock::VerifyAndClearExpectations(&ledHeartMock);
    Mock::VerifyAndClearExpectations(&testArduinoEnv);
  }
};

TEST_F(MovementShowTest, testThatTheGyroscopeWakesUpWhenTheMovementShowIsInitialized) {
  EXPECT_CALL(gyroscopeMock, wakeUp()).Times(Exactly(1));
  // The current and former acceleration are initialized in the gyrosscope
  EXPECT_CALL(gyroscopeMock, measureAcceleration()).Times(Exactly(2));
  movementShow->initialize();
}

TEST_F(
    MovementShowTest,
    testThatTheMovementShowDetectesTheAccerlerationVectorDifferenceByCalculatingTheDifferenceBetweenTheCurrentAccerlerationAndTheFormerOneAndPassesItToTheActionMapper) {
  auto formerAccleration = AccelerationMeasurementVector(1, 1, 1);
  auto currentAccleration = AccelerationMeasurementVector(4, 4, 4);
  auto expectedDifference = formerAccleration.euclideanDistanceTo(currentAccleration);

  EXPECT_CALL(gyroscopeMock, wakeUp()).Times(Exactly(1));
  // The current and former acceleration are initialized in the gyrosscope
  EXPECT_CALL(gyroscopeMock, measureAcceleration())
      .Times(Exactly(3))
      // former and current acceleration gets detected during wake up
      .WillOnce(Return(formerAccleration))
      .WillOnce(Return(formerAccleration))
      // Current acceleration gets measured
      .WillOnce(Return(currentAccleration));
  EXPECT_CALL(actionMapperMock, mapToHeart(Eq(expectedDifference))).Times(Exactly(1));

  movementShow->initialize();
  movementShow->executeIterationWith(&actionMapperMock);
}

TEST_F(MovementShowTest, testThatTheMovementShowStoresTheCurrentAccelerationToMapTheDifferenceOfItToTheNextAccelerationToTheGivenActionMapper) {
  auto initializationAccleration = AccelerationMeasurementVector(100, 100, 100);
  auto formerAccleration = AccelerationMeasurementVector(1, 1, 1);
  auto currentAccleration = AccelerationMeasurementVector(4, 4, 4);
  auto expectedDifferenceAfterInitialization = initializationAccleration.euclideanDistanceTo(formerAccleration);
  auto expectedDifferenceAfterFirstIteration = formerAccleration.euclideanDistanceTo(currentAccleration);

  EXPECT_CALL(gyroscopeMock, wakeUp()).Times(Exactly(1));
  // The current and former acceleration are initialized in the gyrosscope
  EXPECT_CALL(gyroscopeMock, measureAcceleration())
      .Times(Exactly(4))
      // former and current acceleration gets detected during wake up
      .WillOnce(Return(initializationAccleration))
      .WillOnce(Return(initializationAccleration))
      // Current acceleration gets measured the first time (this will be the former for our calculation)
      .WillOnce(Return(formerAccleration))
      // Current acceleration gets measured the second time (this will be the current for our calculation)
      .WillOnce(Return(currentAccleration));
  EXPECT_CALL(actionMapperMock, mapToHeart(Eq(initializationAccleration.euclideanDistanceTo(formerAccleration)))).Times(Exactly(1));
  EXPECT_CALL(actionMapperMock, mapToHeart(Eq(formerAccleration.euclideanDistanceTo(currentAccleration)))).Times(Exactly(1));
  movementShow->initialize();
  movementShow->executeIterationWith(&actionMapperMock);
  movementShow->executeIterationWith(&actionMapperMock);
}

TEST_F(
    MovementShowTest,
    testThatTheRandomBlinkLedMovementShowMapsTheAcclerationRatioToTheLedBrightnessAndTurnsOnSomeLedsRandomlyWhenTheRawAccerlerationDifferenceValueLiesOverItsGivenThreshold) {
  auto ledBrightnessMapper = new LedBrightnessAccelerationRatioMapper(10, 10);
  auto randomBlinkLedActionMapper = new RandomLedBlinkMovementShow(&ledHeartMock, ledBrightnessMapper, &testArduinoEnv, 1000.0f);
  auto mappedDifference = new AccerlationVectorDifference(1500.0f, 0.20f);

  // The brightness is toggled
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(mappedDifference->mapAccelerationRatioTo(ledBrightnessMapper)))).Times(Exactly(1));
  // leds are turned on randomly on the led heart
  EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(4))).Times(Exactly(1));
  // led blink time out
  EXPECT_CALL(testArduinoEnv, delayFor(Eq(10))).Times(Exactly(1));

  randomBlinkLedActionMapper->mapToHeart(mappedDifference);
}

TEST_F(
    MovementShowTest,
    testThatTheRandomBlinkLedMovementShowMapsTheAcclerationRatioToTheLedBrightnessAndDoesNotPerformAnyOtherActionWhenTheThresholdOfTheRawAccVectorDifferenceLiesUnderTheThreshold) {
  auto ledBrightnessMapper = new LedBrightnessAccelerationRatioMapper(10, 10);
  auto randomBlinkLedActionMapper = new RandomLedBlinkMovementShow(&ledHeartMock, ledBrightnessMapper, &testArduinoEnv, 1000.0f);
  auto mappedDifference = new AccerlationVectorDifference(999.0f, 0.20f);

  // The brightness is toggled
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(mappedDifference->mapAccelerationRatioTo(ledBrightnessMapper)))).Times(Exactly(1));
  // leds are never turned on randomly on the led heart
  EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(4))).Times(Exactly(0));
  // led blink time out was never used
  EXPECT_CALL(testArduinoEnv, delayFor(Eq(10))).Times(Exactly(0));

  randomBlinkLedActionMapper->mapToHeart(mappedDifference);
}

TEST_F(MovementShowTest,
       testThatTheSequentialLedHeartTurnOnShowMapsTheAccerlerationRatioToTheNumberOfHeartsOnTheLedHeartAndTurnsThemOnWithADelayAndFullBrightness) {
  auto randomBlinkLedActionMapper = new SequentialHeartTurnOnShow(&ledHeartMock, &testArduinoEnv);
  auto mappedDifference = new AccerlationVectorDifference(999.0f, 0.50f);

  EXPECT_CALL(ledHeartMock, turnOffAll()).Times(Exactly(1));
  EXPECT_CALL(ledHeartMock, turnOn(Eq(0))).Times(Exactly(1));
  EXPECT_CALL(ledHeartMock, turnOn(Eq(1))).Times(Exactly(1));
  EXPECT_CALL(ledHeartMock, turnOn(Eq(2))).Times(Exactly(1));
  EXPECT_CALL(ledHeartMock, turnOn(Eq(3))).Times(Exactly(1));
  EXPECT_CALL(ledHeartMock, turnOn(Eq(4))).Times(Exactly(1));
  EXPECT_CALL(ledHeartMock, turnOn(Eq(5))).Times(Exactly(1));
  EXPECT_CALL(ledHeartMock, turnOn(Eq(6))).Times(Exactly(1));

  EXPECT_CALL(testArduinoEnv, delayFor(Eq(10))).Times(Exactly(1));

  randomBlinkLedActionMapper->mapToHeart(mappedDifference);
}
