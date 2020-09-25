#include "ledHeart.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mockClasses.h"

using ::testing::_;
using ::testing::AllOf;
using ::testing::Exactly;
using ::testing::Ge;
using ::testing::Le;
using ::testing::Mock;
using ::testing::Return;

class LedHeartTest : public ::testing::Test {
 public:
  ShiftRegisterMock leftShiftRegisterMock;
  ShiftRegisterMock rightShiftRegisterMock;
  STDNamespaceFunctions stdNamespaceFunctions;
  IStandardFunctionsMock stdNamespaceFunctionsMock;

  LedHeart ledHeart;
  LedHeart ledHeartWithNamespaceFunctionsMock;

 protected:
  void SetUp() override {
    ledHeart = LedHeart(&stdNamespaceFunctions, &leftShiftRegisterMock, &rightShiftRegisterMock);
    ledHeartWithNamespaceFunctionsMock = LedHeart(&stdNamespaceFunctionsMock, &leftShiftRegisterMock, &rightShiftRegisterMock);
  }

  void TearDown() override {
    Mock::VerifyAndClearExpectations(&leftShiftRegisterMock);
    Mock::VerifyAndClearExpectations(&rightShiftRegisterMock);
    Mock::VerifyAndClearExpectations(&stdNamespaceFunctionsMock);
  }
};

TEST_F(LedHeartTest, testThatTheLedHeartInitializesBothShiftRegistersWhenItIsInitialized) {
  EXPECT_CALL(leftShiftRegisterMock, initializePins()).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, initializePins()).Times(Exactly(1));
  ledHeart.initialize();
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnAllLedsOnBothShiftRegistersWhenItIsTurningOnTheWholeHeart) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnAll()).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOnAll()).Times(Exactly(1));
  ledHeart.turnOnAll();
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOffAllLedsOnBothShiftRegistersWhenItIsTurningOffTheWholeHeart) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffAll()).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOffAll()).Times(Exactly(1));
  ledHeart.turnOffAll();
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnTheLedOnTheLeftShiftRegisterWhenItsLedIndexIsSmallerThan7) {
  EXPECT_CALL(leftShiftRegisterMock, turnOn(HEART_LED_COUNT / 2 - 1)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOn(_)).Times(Exactly(0));
  ledHeart.turnOn(HEART_LED_COUNT / 2 - 1);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnTheLedOnTheRightShiftRegisterWhenItsLedIndexIsGreaterThan7) {
  int ledIndexOnRightHeart = 1;
  EXPECT_CALL(leftShiftRegisterMock, turnOn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOn(ledIndexOnRightHeart)).Times(Exactly(1));
  ledHeart.turnOn(HEART_LED_COUNT / 2 + ledIndexOnRightHeart);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOnAnythingWhenTheGivenLedIndexIsNegative) {
  EXPECT_CALL(leftShiftRegisterMock, turnOn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOn(_)).Times(Exactly(0));
  ledHeart.turnOn(-1);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOnAnythingWhenTheMaximumLedIndexIsExceeded) {
  EXPECT_CALL(leftShiftRegisterMock, turnOn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOn(_)).Times(Exactly(0));
  ledHeart.turnOn(HEART_LED_COUNT);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOffTheLedOnTheLeftShiftRegisterWhenItsLedIndexIsSmallerThan7) {
  EXPECT_CALL(leftShiftRegisterMock, turnOff(HEART_LED_COUNT / 2 - 1)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOff(_)).Times(Exactly(0));
  ledHeart.turnOff(HEART_LED_COUNT / 2 - 1);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnTheLedOffTheRightShiftRegisterWhenItsLedIndexIsGreaterThan7) {
  int ledIndexOnRightHeart = 1;
  EXPECT_CALL(leftShiftRegisterMock, turnOff(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOff(ledIndexOnRightHeart)).Times(Exactly(1));
  ledHeart.turnOff(HEART_LED_COUNT / 2 + ledIndexOnRightHeart);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOffAnythingWhenTheGivenLedIndexIsNegative) {
  EXPECT_CALL(leftShiftRegisterMock, turnOff(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOff(_)).Times(Exactly(0));
  ledHeart.turnOff(-1);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOffAnythingWhenTheMaximumLedIndexIsExceeded) {
  EXPECT_CALL(leftShiftRegisterMock, turnOff(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOff(_)).Times(Exactly(0));
  ledHeart.turnOff(HEART_LED_COUNT);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOnAnyLevelWhenTheGivenLevelIndexIsNegative) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnLevel(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOnLevel(_)).Times(Exactly(0));
  ledHeart.turnLevelOn(-1);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOnAnyLevelWhenTheGivenLevelIndexIsExceedsTheLevelCountOnTheHeart) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnLevel(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOnLevel(_)).Times(Exactly(0));
  ledHeart.turnLevelOn(HEART_LEVEL_COUNT + 1);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnAGivenLevelOnBothRegisterWhenTheGivenLevelIndexIsValid) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnLevel(3)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOnLevel(3)).Times(Exactly(1));
  ledHeart.turnLevelOn(3);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOffAnyLevelWhenTheGivenLevelIndexIsNegative) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffLevel(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOffLevel(_)).Times(Exactly(0));
  ledHeart.turnLevelOff(-1);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOffAnyLevelWhenTheGivenLevelIndexIsExceedsTheLevelCountOnTheHeart) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffLevel(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOffLevel(_)).Times(Exactly(0));
  ledHeart.turnLevelOff(HEART_LEVEL_COUNT + 1);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOffAGivenLevelOnBothRegisterWhenTheGivenLevelIndexIsValid) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffLevel(3)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOffLevel(3)).Times(Exactly(1));
  ledHeart.turnLevelOff(3);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOnAnyColumnWhenTheGivenColumnIndexIsNegative) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnColumn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOnColumn(_)).Times(Exactly(0));
  ledHeart.turnColumnOn(-1);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOnAnyColumnWhenTheGiveColumnIndexIsExceedsTheColumnCountOnTheHeart) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnColumn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOnColumn(_)).Times(Exactly(0));
  ledHeart.turnColumnOn(HEART_COLUMN_COUNT + 1);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnTheFirstColumnOnTheRightShiftRegisterWhenTheFirstColumnIsTurnedOn) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnColumn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOnColumn(1)).Times(Exactly(1));
  ledHeart.turnColumnOn(1);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnTheSecondColumnOnTheRightShiftRegisterWhenTheSecondColumnIsTurnedOn) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnColumn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOnColumn(2)).Times(Exactly(1));
  ledHeart.turnColumnOn(2);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnTheThirdColumnOnTheRightShiftRegisterWhenTheThirdColumnIsTurnedOn) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnColumn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOnColumn(3)).Times(Exactly(1));
  ledHeart.turnColumnOn(3);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnTheFourthColumnOnTheRightAndTheLeftShiftRegisterWhenTheFourthColumnIsTurnedOn) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnColumn(4)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOnColumn(4)).Times(Exactly(1));
  ledHeart.turnColumnOn(4);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnTheFifthColumnByTuringOnTheThridColumnOnTheLeftShiftRegister) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnColumn(3)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOnColumn(_)).Times(Exactly(0));
  ledHeart.turnColumnOn(5);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnTheSixthColumnByTuringOnTheSecondColumnOnTheLeftShiftRegister) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnColumn(2)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOnColumn(_)).Times(Exactly(0));
  ledHeart.turnColumnOn(6);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOnTheSeventhColumnByTuringOnTheFirstColumnOnTheLeftShiftRegister) {
  EXPECT_CALL(leftShiftRegisterMock, turnOnColumn(1)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOnColumn(_)).Times(Exactly(0));
  ledHeart.turnColumnOn(7);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOffAnyColumnWhenTheGivenColumnIndexIsNegative) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffColumn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOffColumn(_)).Times(Exactly(0));
  ledHeart.turnColumnOff(-1);
}

TEST_F(LedHeartTest, testThatTheLedHeartNeverTurnsOffAnyColumnWhenTheGivenColumnIndexIsExceedsTheColumnCountOnTheHeart) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffColumn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOffColumn(_)).Times(Exactly(0));
  ledHeart.turnColumnOff(HEART_COLUMN_COUNT + 1);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOffTheFirstColumnOnTheRightShiftRegisterWhenTheFirstColumnIsTurnedOff) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffColumn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOffColumn(1)).Times(Exactly(1));
  ledHeart.turnColumnOff(1);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOffTheSecondColumnOnTheRightShiftRegisterWhenTheSecondColumnIsTurnedOff) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffColumn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOffColumn(2)).Times(Exactly(1));
  ledHeart.turnColumnOff(2);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOffTheThirdColumnOnTheRightShiftRegisterWhenTheThirdColumnIsTurnedOff) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffColumn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOffColumn(3)).Times(Exactly(1));
  ledHeart.turnColumnOff(3);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOffTheFourthColumnOnTheRightAndTheLeftShiftRegisterWhenTheFourthColumnIsTurnedOff) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffColumn(4)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOffColumn(4)).Times(Exactly(1));
  ledHeart.turnColumnOff(4);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOffTheFifthColumnByTuringOffTheThridColumnOnTheLeftShiftRegister) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffColumn(3)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOffColumn(_)).Times(Exactly(0));
  ledHeart.turnColumnOff(5);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOffTheSixthColumnByTuringOffTheSecondColumnOnTheLeftShiftRegister) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffColumn(2)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOffColumn(_)).Times(Exactly(0));
  ledHeart.turnColumnOff(6);
}

TEST_F(LedHeartTest, testThatTheLedHeartTurnsOffTheSeventhColumnByTuringOffTheFirstColumnOnTheLeftShiftRegister) {
  EXPECT_CALL(leftShiftRegisterMock, turnOffColumn(1)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOffColumn(_)).Times(Exactly(0));
  ledHeart.turnColumnOff(7);
}

TEST_F(LedHeartTest, testThatTheLedHeartTogglesTheBrightnessOnBothShiftRegistersWhenTheGivenBrightnessValueIsValid) {
  EXPECT_CALL(leftShiftRegisterMock, toggleBrightness(50)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, toggleBrightness(50)).Times(Exactly(1));
  ledHeart.toggleBrightness(50);
}

TEST_F(LedHeartTest, testThatTheLedHeartNotTogglesTheBrightnessWhenTheGivenBrightnessValueIsNegative) {
  EXPECT_CALL(leftShiftRegisterMock, toggleBrightness(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, toggleBrightness(_)).Times(Exactly(0));
  ledHeart.toggleBrightness(-1);
}

TEST_F(LedHeartTest, testThatTheLedHeartNotTogglesTheBrightnessWhenTheGivenBrightnessValueExceedsTheMaximumBrightnessValue) {
  EXPECT_CALL(leftShiftRegisterMock, toggleBrightness(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, toggleBrightness(_)).Times(Exactly(0));
  ledHeart.toggleBrightness(HEART_MAX_BRIGHTNESS + 1);
}

TEST_F(LedHeartTest, testThatTheLedHeartWillTurnOnTheLedsOnTheHeartRandomlyWithTheGivenCountOnLedsTurnedOnAtLeast) {
  int atLeastTurnedOn = 2;
  // First time the rand int is created, the count on turned on leds is determined
  EXPECT_CALL(stdNamespaceFunctionsMock, nextRandomIntInBounds(atLeastTurnedOn, HEART_LED_COUNT))
      .Times(Exactly(1))
      .WillOnce(Return(atLeastTurnedOn + 2));
  // Next 4 times the randomly turned on leds will be distinguished randomly
  EXPECT_CALL(stdNamespaceFunctionsMock, nextRandomIntInBounds(0, HEART_LED_COUNT))
      .Times(Exactly(4))
      .WillOnce(Return(1))
      .WillOnce(Return(4))
      .WillOnce(Return(10))
      .WillOnce(Return(6));
  EXPECT_CALL(leftShiftRegisterMock, turnOffAll()).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOffAll()).Times(Exactly(1));

  EXPECT_CALL(leftShiftRegisterMock, turnOn(1)).Times(Exactly(1));
  EXPECT_CALL(leftShiftRegisterMock, turnOn(4)).Times(Exactly(1));
  EXPECT_CALL(leftShiftRegisterMock, turnOn(6)).Times(Exactly(1));

  EXPECT_CALL(rightShiftRegisterMock, turnOn(10 - HEART_LED_COUNT / 2)).Times(Exactly(1));

  ledHeartWithNamespaceFunctionsMock.turnOnRandomly(atLeastTurnedOn);
}

TEST_F(
    LedHeartTest,
    testThatTheLedHeartWillTurnOnTheLedsOnTheHeartRandomlyWithTheGivenCountOnLedsTurnedAndAlsoTestThatItCalculatesRandomNumbersUntilTheyAreUnique) {
  int atLeastTurnedOn = 2;
  // First time the rand int is created, the count on turned on leds is determined
  EXPECT_CALL(stdNamespaceFunctionsMock, nextRandomIntInBounds(atLeastTurnedOn, HEART_LED_COUNT))
      .Times(Exactly(1))
      .WillOnce(Return(atLeastTurnedOn + 2));
  // Next 4 times the randomly turned on leds will be distinguished randomly
  EXPECT_CALL(stdNamespaceFunctionsMock, nextRandomIntInBounds(0, HEART_LED_COUNT))
      .Times(Exactly(6))
      .WillOnce(Return(7))
      .WillOnce(Return(4))
      .WillOnce(Return(4))
      .WillOnce(Return(4))
      .WillOnce(Return(10))
      .WillOnce(Return(6));
  EXPECT_CALL(leftShiftRegisterMock, turnOffAll()).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOffAll()).Times(Exactly(1));

  EXPECT_CALL(leftShiftRegisterMock, turnOn(4)).Times(Exactly(1));
  EXPECT_CALL(leftShiftRegisterMock, turnOn(6)).Times(Exactly(1));

  EXPECT_CALL(rightShiftRegisterMock, turnOn(10 - HEART_LED_COUNT / 2)).Times(Exactly(1));
  EXPECT_CALL(rightShiftRegisterMock, turnOn(7 - HEART_LED_COUNT / 2)).Times(Exactly(1));

  ledHeartWithNamespaceFunctionsMock.turnOnRandomly(atLeastTurnedOn);
}

TEST_F(LedHeartTest, testThatTheLedHeartWillTurnOnNothingRandomlyWhenTheMinimumCountOfLedsTurnedOnIsNegative) {
  int atLeastTurnedOn = -1;
  // First time the rand int is created, the count on turned on leds is determined
  EXPECT_CALL(stdNamespaceFunctionsMock, nextRandomIntInBounds(atLeastTurnedOn, HEART_LED_COUNT)).Times(Exactly(0));
  // Next 4 times the randomly turned on leds will be distinguished randomly
  EXPECT_CALL(stdNamespaceFunctionsMock, nextRandomIntInBounds(0, HEART_LED_COUNT)).Times(Exactly(0));
  EXPECT_CALL(leftShiftRegisterMock, turnOffAll()).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOffAll()).Times(Exactly(0));

  EXPECT_CALL(leftShiftRegisterMock, turnOn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOn(_)).Times(Exactly(0));

  ledHeartWithNamespaceFunctionsMock.turnOnRandomly(atLeastTurnedOn);
}

TEST_F(LedHeartTest, testThatTheLedHeartWillTurnOnNothingRandomlyWhenTheMinimumCountOfLedsTurnedOnIsExceedsTheHeartLedCount) {
  int atLeastTurnedOn = HEART_LED_COUNT + 1;
  // First time the rand int is created, the count on turned on leds is determined
  EXPECT_CALL(stdNamespaceFunctionsMock, nextRandomIntInBounds(atLeastTurnedOn, HEART_LED_COUNT)).Times(Exactly(0));
  // Next 4 times the randomly turned on leds will be distinguished randomly
  EXPECT_CALL(stdNamespaceFunctionsMock, nextRandomIntInBounds(0, HEART_LED_COUNT)).Times(Exactly(0));
  EXPECT_CALL(leftShiftRegisterMock, turnOffAll()).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOffAll()).Times(Exactly(0));

  EXPECT_CALL(leftShiftRegisterMock, turnOn(_)).Times(Exactly(0));
  EXPECT_CALL(rightShiftRegisterMock, turnOn(_)).Times(Exactly(0));

  ledHeartWithNamespaceFunctionsMock.turnOnRandomly(atLeastTurnedOn);
}

TEST_F(LedHeartTest, testThatTheStandardNamespaceFunctionsGenerateARandomNumberInTheGivenBounds) {
  EXPECT_THAT(stdNamespaceFunctions.nextRandomIntInBounds(10, 20), AllOf(Ge(10), Le(20)));
  EXPECT_THAT(stdNamespaceFunctions.nextRandomIntInBounds(-5, 5), AllOf(Ge(-5), Le(5)));
  EXPECT_THAT(stdNamespaceFunctions.nextRandomIntInBounds(0, 10), AllOf(Ge(0), Le(10)));
}
