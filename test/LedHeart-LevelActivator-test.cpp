#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ledHeart.h"

class LedHeartLevelActivatorTest : public ::testing::Test {
 public:
  Led LEDS_ON_SHIFT_REGISTER[MAX_SHIFT_REGISTER_LED_COUNT] = {// Led number, position on shift register
                                                              Led(1, 0), Led(2, 1), Led(3, 2), Led(4, 3), Led(5, 4), Led(6, 5), Led(7, 6)};

  Led allLedsTurnedOff[MAX_SHIFT_REGISTER_LED_COUNT];
  Led allLedsTurnedOn[MAX_SHIFT_REGISTER_LED_COUNT];

  LeftRegisterLevelActivator leftRegisterLevelActivator = LeftRegisterLevelActivator();
  RightRegisterLevelActivator rightRegisterLevelActivator = RightRegisterLevelActivator();

 protected:
  void SetUp() override {
    for (int i = 0; i < MAX_SHIFT_REGISTER_LED_COUNT; i++) {
      allLedsTurnedOff[i] = Led(LEDS_ON_SHIFT_REGISTER[i].shiftRegisterPosition, i);
      allLedsTurnedOn[i] = Led(LEDS_ON_SHIFT_REGISTER[i].shiftRegisterPosition, i);
      allLedsTurnedOn[i].on = true;
    }
  }
};

// LeftRegisterLevelActivator - Turn On

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testThatNothingIsChangedOnTheLedStatusWhenANegativLevelIsGivenAndTheLevelIsRequestedToBeTurnedOn) {
  leftRegisterLevelActivator.turnOnLevel(-1, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest,
       LeftRegister_testThatNothingIsChangedOnTheLedStatusWhenALevelIsGivenWhichsExceedsTheMaxLevelCountAndTheLevelIsRequestedToBeTurnedOn) {
  leftRegisterLevelActivator.turnOnLevel(MAX_LEVEL + 1, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOnTheFirstLevel) {
  leftRegisterLevelActivator.turnOnLevel(1, allLedsTurnedOff);
  ASSERT_TRUE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOnTheSecondLevel) {
  leftRegisterLevelActivator.turnOnLevel(2, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_TRUE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOnTheThirdLevel) {
  leftRegisterLevelActivator.turnOnLevel(3, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_TRUE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOnTheFouthLevel) {
  leftRegisterLevelActivator.turnOnLevel(4, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_TRUE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOnTheFifthLevel) {
  leftRegisterLevelActivator.turnOnLevel(5, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_TRUE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_TRUE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOnTheSixthLevel) {
  leftRegisterLevelActivator.turnOnLevel(6, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_TRUE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

// LeftRegisterLevelActivator - Turn Off
TEST_F(LedHeartLevelActivatorTest, LeftRegister_testThatNothingIsChangedOnTheLedStatusWhenANegativLevelIsGivenAndTheLevelIsRequestedToBeTurnedOff) {
  leftRegisterLevelActivator.turnOffLevel(-1, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest,
       LeftRegister_testThatNothingIsChangedOnTheLedStatusWhenALevelIsGivenWhichsExceedsTheMaxLevelCountAndTheLevelIsRequestedToBeTurnedOff) {
  leftRegisterLevelActivator.turnOffLevel(MAX_LEVEL + 1, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOffTheFirstLevel) {
  leftRegisterLevelActivator.turnOffLevel(1, allLedsTurnedOn);
  ASSERT_FALSE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOffTheSecondLevel) {
  leftRegisterLevelActivator.turnOffLevel(2, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_FALSE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOffTheThirdLevel) {
  leftRegisterLevelActivator.turnOffLevel(3, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_FALSE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOffTheFouthLevel) {
  leftRegisterLevelActivator.turnOffLevel(4, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_FALSE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOffTheFifthLevel) {
  leftRegisterLevelActivator.turnOffLevel(5, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_FALSE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_FALSE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, LeftRegister_testTurningOffTheSixthLevel) {
  leftRegisterLevelActivator.turnOffLevel(6, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_FALSE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

// RightRegisterLevelActivator - Turn On

TEST_F(LedHeartLevelActivatorTest, RightRegister_testThatNothingIsChangedOnTheLedStatusWhenANegativLevelIsGivenAndTheLevelIsRequestedToBeTurnedOn) {
  rightRegisterLevelActivator.turnOnLevel(-1, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest,
       RightRegister_testThatNothingIsChangedOnTheLedStatusWhenALevelIsGivenWhichsExceedsTheMaxLevelCountAndTheLevelIsRequestedToBeTurnedOn) {
  rightRegisterLevelActivator.turnOnLevel(MAX_LEVEL + 1, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOnTheFirstLevel) {
  rightRegisterLevelActivator.turnOnLevel(1, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOnTheSecondLevel) {
  rightRegisterLevelActivator.turnOnLevel(2, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_TRUE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOnTheThirdLevel) {
  rightRegisterLevelActivator.turnOnLevel(3, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_TRUE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOnTheFouthLevel) {
  rightRegisterLevelActivator.turnOnLevel(4, allLedsTurnedOff);
  ASSERT_TRUE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_TRUE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOnTheFifthLevel) {
  rightRegisterLevelActivator.turnOnLevel(5, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_TRUE(allLedsTurnedOff[1].on);
  ASSERT_FALSE(allLedsTurnedOff[2].on);
  ASSERT_TRUE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOnTheSixthLevel) {
  rightRegisterLevelActivator.turnOnLevel(6, allLedsTurnedOff);
  ASSERT_FALSE(allLedsTurnedOff[0].on);
  ASSERT_FALSE(allLedsTurnedOff[1].on);
  ASSERT_TRUE(allLedsTurnedOff[2].on);
  ASSERT_FALSE(allLedsTurnedOff[3].on);
  ASSERT_FALSE(allLedsTurnedOff[4].on);
  ASSERT_FALSE(allLedsTurnedOff[5].on);
  ASSERT_FALSE(allLedsTurnedOff[6].on);
}

// RightRegisterLevelActivator - Turn Off
TEST_F(LedHeartLevelActivatorTest, RightRegister_testThatNothingIsChangedOnTheLedStatusWhenANegativLevelIsGivenAndTheLevelIsRequestedToBeTurnedOff) {
  rightRegisterLevelActivator.turnOffLevel(-1, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest,
       RightRegister_testThatNothingIsChangedOnTheLedStatusWhenALevelIsGivenWhichsExceedsTheMaxLevelCountAndTheLevelIsRequestedToBeTurnedOff) {
  rightRegisterLevelActivator.turnOffLevel(MAX_LEVEL + 1, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOffTheFirstLevel) {
  rightRegisterLevelActivator.turnOffLevel(1, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOffTheSecondLevel) {
  rightRegisterLevelActivator.turnOffLevel(2, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_FALSE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOffTheThirdLevel) {
  rightRegisterLevelActivator.turnOffLevel(3, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_FALSE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOffTheFouthLevel) {
  rightRegisterLevelActivator.turnOffLevel(4, allLedsTurnedOn);
  ASSERT_FALSE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_FALSE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOffTheFifthLevel) {
  rightRegisterLevelActivator.turnOffLevel(5, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_FALSE(allLedsTurnedOn[1].on);
  ASSERT_TRUE(allLedsTurnedOn[2].on);
  ASSERT_FALSE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartLevelActivatorTest, RightRegister_testTurningOffTheSixthLevel) {
  rightRegisterLevelActivator.turnOffLevel(6, allLedsTurnedOn);
  ASSERT_TRUE(allLedsTurnedOn[0].on);
  ASSERT_TRUE(allLedsTurnedOn[1].on);
  ASSERT_FALSE(allLedsTurnedOn[2].on);
  ASSERT_TRUE(allLedsTurnedOn[3].on);
  ASSERT_TRUE(allLedsTurnedOn[4].on);
  ASSERT_TRUE(allLedsTurnedOn[5].on);
  ASSERT_TRUE(allLedsTurnedOn[6].on);
}
