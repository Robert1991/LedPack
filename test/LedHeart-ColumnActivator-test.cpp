#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ledHeart.h"

class LedHeartColumnActivatorTest : public ::testing::Test
{
public:
    LeftRegisterColumnActivator leftRegisterColumnActivator = LeftRegisterColumnActivator();
    RightRegisterColumnActivator rightRegisterColumnActivator = RightRegisterColumnActivator();

    Led LEDS_ON_SHIFT_REGISTER[MAX_SHIFT_REGISTER_LED_COUNT] = {
        // Led number, position on shift register
        Led(1, 0),
        Led(2, 1),
        Led(3, 2),
        Led(4, 3),
        Led(5, 4),
        Led(6, 5),
        Led(7, 6)};

    Led allLedsTurnedOff[MAX_SHIFT_REGISTER_LED_COUNT];
    Led allLedsTurnedOn[MAX_SHIFT_REGISTER_LED_COUNT];

protected:
    void SetUp() override
    {
        for (int i = 0; i < MAX_SHIFT_REGISTER_LED_COUNT; i++)
        {
            allLedsTurnedOff[i] = Led(LEDS_ON_SHIFT_REGISTER[i].shiftRegisterPosition, i);
            allLedsTurnedOn[i] = Led(LEDS_ON_SHIFT_REGISTER[i].shiftRegisterPosition, i);
            allLedsTurnedOn[i].on = true;
        }
    }
};

// LeftRegisterColumnActivator - Turn On

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testThatNothingIsChangedOnTheLedStatusWhenANegativColumnIsGivenAndTheColumnIsRequestedToBeTurnedOn)
{
    leftRegisterColumnActivator.turnOnColumn(-1, allLedsTurnedOff);
    ASSERT_FALSE(allLedsTurnedOff[0].on);
    ASSERT_FALSE(allLedsTurnedOff[1].on);
    ASSERT_FALSE(allLedsTurnedOff[2].on);
    ASSERT_FALSE(allLedsTurnedOff[3].on);
    ASSERT_FALSE(allLedsTurnedOff[4].on);
    ASSERT_FALSE(allLedsTurnedOff[5].on);
    ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testThatNothingIsChangedOnTheLedStatusWhenAColumnIsGivenWhichsExceedsTheMaxLevelCountAndTheColumnIsRequestedToBeTurnedOn)
{
    leftRegisterColumnActivator.turnOnColumn(MAX_COLUMNS + 1, allLedsTurnedOff);
    ASSERT_FALSE(allLedsTurnedOff[0].on);
    ASSERT_FALSE(allLedsTurnedOff[1].on);
    ASSERT_FALSE(allLedsTurnedOff[2].on);
    ASSERT_FALSE(allLedsTurnedOff[3].on);
    ASSERT_FALSE(allLedsTurnedOff[4].on);
    ASSERT_FALSE(allLedsTurnedOff[5].on);
    ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testTurningOnTheFirstColumn)
{
    leftRegisterColumnActivator.turnOnColumn(1, allLedsTurnedOff);
    ASSERT_FALSE(allLedsTurnedOff[0].on);
    ASSERT_FALSE(allLedsTurnedOff[1].on);
    ASSERT_FALSE(allLedsTurnedOff[2].on);
    ASSERT_FALSE(allLedsTurnedOff[3].on);
    ASSERT_TRUE(allLedsTurnedOff[4].on);
    ASSERT_FALSE(allLedsTurnedOff[5].on);
    ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testTurningOnTheSecondColumn)
{
    leftRegisterColumnActivator.turnOnColumn(2, allLedsTurnedOff);
    ASSERT_FALSE(allLedsTurnedOff[0].on);
    ASSERT_FALSE(allLedsTurnedOff[1].on);
    ASSERT_TRUE(allLedsTurnedOff[2].on);
    ASSERT_TRUE(allLedsTurnedOff[3].on);
    ASSERT_FALSE(allLedsTurnedOff[4].on);
    ASSERT_TRUE(allLedsTurnedOff[5].on);
    ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testTurningOnTheThirdColumn)
{
    leftRegisterColumnActivator.turnOnColumn(3, allLedsTurnedOff);
    ASSERT_FALSE(allLedsTurnedOff[0].on);
    ASSERT_TRUE(allLedsTurnedOff[1].on);
    ASSERT_FALSE(allLedsTurnedOff[2].on);
    ASSERT_FALSE(allLedsTurnedOff[3].on);
    ASSERT_FALSE(allLedsTurnedOff[4].on);
    ASSERT_FALSE(allLedsTurnedOff[5].on);
    ASSERT_TRUE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testTurningOnTheFouthColumn)
{
    leftRegisterColumnActivator.turnOnColumn(4, allLedsTurnedOff);
    ASSERT_TRUE(allLedsTurnedOff[0].on);
    ASSERT_FALSE(allLedsTurnedOff[1].on);
    ASSERT_FALSE(allLedsTurnedOff[2].on);
    ASSERT_FALSE(allLedsTurnedOff[3].on);
    ASSERT_FALSE(allLedsTurnedOff[4].on);
    ASSERT_FALSE(allLedsTurnedOff[5].on);
    ASSERT_FALSE(allLedsTurnedOff[6].on);
}

// LeftRegisterColumnActivator - Turn Off
TEST_F(LedHeartColumnActivatorTest, LeftRegister_testThatNothingIsChangedOnTheLedStatusWhenANegativColumnIsGivenAndTheColumnIsRequestedToBeTurnedOff)
{
    leftRegisterColumnActivator.turnOffColumn(-1, allLedsTurnedOn);
    ASSERT_TRUE(allLedsTurnedOn[0].on);
    ASSERT_TRUE(allLedsTurnedOn[1].on);
    ASSERT_TRUE(allLedsTurnedOn[2].on);
    ASSERT_TRUE(allLedsTurnedOn[3].on);
    ASSERT_TRUE(allLedsTurnedOn[4].on);
    ASSERT_TRUE(allLedsTurnedOn[5].on);
    ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testThatNothingIsChangedOnTheLedStatusWhenAColumnIsGivenWhichsExceedsTheMaxLevelCountAndTheColumnIsRequestedToBeTurnedOff)
{
    leftRegisterColumnActivator.turnOffColumn(MAX_COLUMNS + 1, allLedsTurnedOn);
    ASSERT_TRUE(allLedsTurnedOn[0].on);
    ASSERT_TRUE(allLedsTurnedOn[1].on);
    ASSERT_TRUE(allLedsTurnedOn[2].on);
    ASSERT_TRUE(allLedsTurnedOn[3].on);
    ASSERT_TRUE(allLedsTurnedOn[4].on);
    ASSERT_TRUE(allLedsTurnedOn[5].on);
    ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testTurningOffTheFirstColumn)
{
    leftRegisterColumnActivator.turnOffColumn(1, allLedsTurnedOn);
    ASSERT_TRUE(allLedsTurnedOn[0].on);
    ASSERT_TRUE(allLedsTurnedOn[1].on);
    ASSERT_TRUE(allLedsTurnedOn[2].on);
    ASSERT_TRUE(allLedsTurnedOn[3].on);
    ASSERT_FALSE(allLedsTurnedOn[4].on);
    ASSERT_TRUE(allLedsTurnedOn[5].on);
    ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testTurningOffTheSecondColumn)
{
    leftRegisterColumnActivator.turnOffColumn(2, allLedsTurnedOn);
    ASSERT_TRUE(allLedsTurnedOn[0].on);
    ASSERT_TRUE(allLedsTurnedOn[1].on);
    ASSERT_FALSE(allLedsTurnedOn[2].on);
    ASSERT_FALSE(allLedsTurnedOn[3].on);
    ASSERT_TRUE(allLedsTurnedOn[4].on);
    ASSERT_FALSE(allLedsTurnedOn[5].on);
    ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testTurningOffTheThirdColumn)
{
    leftRegisterColumnActivator.turnOffColumn(3, allLedsTurnedOn);
    ASSERT_TRUE(allLedsTurnedOn[0].on);
    ASSERT_FALSE(allLedsTurnedOn[1].on);
    ASSERT_TRUE(allLedsTurnedOn[2].on);
    ASSERT_TRUE(allLedsTurnedOn[3].on);
    ASSERT_TRUE(allLedsTurnedOn[4].on);
    ASSERT_TRUE(allLedsTurnedOn[5].on);
    ASSERT_FALSE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartColumnActivatorTest, LeftRegister_testTurningOffTheFouthColumn)
{
    leftRegisterColumnActivator.turnOffColumn(4, allLedsTurnedOn);
    ASSERT_FALSE(allLedsTurnedOn[0].on);
    ASSERT_TRUE(allLedsTurnedOn[1].on);
    ASSERT_TRUE(allLedsTurnedOn[2].on);
    ASSERT_TRUE(allLedsTurnedOn[3].on);
    ASSERT_TRUE(allLedsTurnedOn[4].on);
    ASSERT_TRUE(allLedsTurnedOn[5].on);
    ASSERT_TRUE(allLedsTurnedOn[6].on);
}

// RightRegisterColumnActivator - Turn On

TEST_F(LedHeartColumnActivatorTest, RightRegister_testThatNothingIsChangedOnTheLedStatusWhenANegativColumnIsGivenAndTheColumnIsRequestedToBeTurnedOn)
{
    rightRegisterColumnActivator.turnOnColumn(-1, allLedsTurnedOff);
    ASSERT_FALSE(allLedsTurnedOff[0].on);
    ASSERT_FALSE(allLedsTurnedOff[1].on);
    ASSERT_FALSE(allLedsTurnedOff[2].on);
    ASSERT_FALSE(allLedsTurnedOff[3].on);
    ASSERT_FALSE(allLedsTurnedOff[4].on);
    ASSERT_FALSE(allLedsTurnedOff[5].on);
    ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartColumnActivatorTest, RightRegister_testThatNothingIsChangedOnTheLedStatusWhenALevelIsGivenWhichsExceedsTheMaxColumnCountAndTheColumnIsRequestedToBeTurnedOn)
{
    rightRegisterColumnActivator.turnOnColumn(MAX_COLUMNS + 1, allLedsTurnedOff);
    ASSERT_FALSE(allLedsTurnedOff[0].on);
    ASSERT_FALSE(allLedsTurnedOff[1].on);
    ASSERT_FALSE(allLedsTurnedOff[2].on);
    ASSERT_FALSE(allLedsTurnedOff[3].on);
    ASSERT_FALSE(allLedsTurnedOff[4].on);
    ASSERT_FALSE(allLedsTurnedOff[5].on);
    ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartColumnActivatorTest, RightRegister_testTurningOnTheFirstColumn)
{
    rightRegisterColumnActivator.turnOnColumn(1, allLedsTurnedOff);
    ASSERT_FALSE(allLedsTurnedOff[0].on);
    ASSERT_FALSE(allLedsTurnedOff[1].on);
    ASSERT_FALSE(allLedsTurnedOff[2].on);
    ASSERT_TRUE(allLedsTurnedOff[3].on);
    ASSERT_FALSE(allLedsTurnedOff[4].on);
    ASSERT_FALSE(allLedsTurnedOff[5].on);
    ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartColumnActivatorTest, RightRegister_testTurningOnTheSecondColumn)
{
    rightRegisterColumnActivator.turnOnColumn(2, allLedsTurnedOff);
    ASSERT_FALSE(allLedsTurnedOff[0].on);
    ASSERT_FALSE(allLedsTurnedOff[1].on);
    ASSERT_TRUE(allLedsTurnedOff[2].on);
    ASSERT_FALSE(allLedsTurnedOff[3].on);
    ASSERT_TRUE(allLedsTurnedOff[4].on);
    ASSERT_TRUE(allLedsTurnedOff[5].on);
    ASSERT_FALSE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartColumnActivatorTest, RightRegister_testTurningOnTheThird)
{
    rightRegisterColumnActivator.turnOnColumn(3, allLedsTurnedOff);
    ASSERT_FALSE(allLedsTurnedOff[0].on);
    ASSERT_TRUE(allLedsTurnedOff[1].on);
    ASSERT_FALSE(allLedsTurnedOff[2].on);
    ASSERT_FALSE(allLedsTurnedOff[3].on);
    ASSERT_FALSE(allLedsTurnedOff[4].on);
    ASSERT_FALSE(allLedsTurnedOff[5].on);
    ASSERT_TRUE(allLedsTurnedOff[6].on);
}

TEST_F(LedHeartColumnActivatorTest, RightRegister_testTurningOnTheFouthLevelOfTheLeftShiftRegistercolumnActivator)
{
    rightRegisterColumnActivator.turnOnColumn(4, allLedsTurnedOff);
    ASSERT_TRUE(allLedsTurnedOff[0].on);
    ASSERT_FALSE(allLedsTurnedOff[1].on);
    ASSERT_FALSE(allLedsTurnedOff[2].on);
    ASSERT_FALSE(allLedsTurnedOff[3].on);
    ASSERT_FALSE(allLedsTurnedOff[4].on);
    ASSERT_FALSE(allLedsTurnedOff[5].on);
    ASSERT_FALSE(allLedsTurnedOff[6].on);
}

// RightRegisterColumnActivator - Turn Off
TEST_F(LedHeartColumnActivatorTest, RightRegister_testThatNothingIsChangedOnTheLedStatusWhenANegativColumnIsGivenAndTheColumnIsRequestedToBeTurnedOff)
{
    rightRegisterColumnActivator.turnOffColumn(-1, allLedsTurnedOn);
    ASSERT_TRUE(allLedsTurnedOn[0].on);
    ASSERT_TRUE(allLedsTurnedOn[1].on);
    ASSERT_TRUE(allLedsTurnedOn[2].on);
    ASSERT_TRUE(allLedsTurnedOn[3].on);
    ASSERT_TRUE(allLedsTurnedOn[4].on);
    ASSERT_TRUE(allLedsTurnedOn[5].on);
    ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartColumnActivatorTest, RightRegister_testThatNothingIsChangedOnTheLedStatusWhenAColumnIsGivenWhichsExceedsTheMaxLevelCountAndTheColumnIsRequestedToBeTurnedOff)
{
    rightRegisterColumnActivator.turnOffColumn(MAX_LEVEL + 1, allLedsTurnedOn);
    ASSERT_TRUE(allLedsTurnedOn[0].on);
    ASSERT_TRUE(allLedsTurnedOn[1].on);
    ASSERT_TRUE(allLedsTurnedOn[2].on);
    ASSERT_TRUE(allLedsTurnedOn[3].on);
    ASSERT_TRUE(allLedsTurnedOn[4].on);
    ASSERT_TRUE(allLedsTurnedOn[5].on);
    ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartColumnActivatorTest, RightRegister_testTurningOffTheFirstColumn)
{
    rightRegisterColumnActivator.turnOffColumn(1, allLedsTurnedOn);
    ASSERT_TRUE(allLedsTurnedOn[0].on);
    ASSERT_TRUE(allLedsTurnedOn[1].on);
    ASSERT_TRUE(allLedsTurnedOn[2].on);
    ASSERT_FALSE(allLedsTurnedOn[3].on);
    ASSERT_TRUE(allLedsTurnedOn[4].on);
    ASSERT_TRUE(allLedsTurnedOn[5].on);
    ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartColumnActivatorTest, RightRegister_testTurningOffTheSecondColumn)
{
    rightRegisterColumnActivator.turnOffColumn(2, allLedsTurnedOn);
    ASSERT_TRUE(allLedsTurnedOn[0].on);
    ASSERT_TRUE(allLedsTurnedOn[1].on);
    ASSERT_FALSE(allLedsTurnedOn[2].on);
    ASSERT_TRUE(allLedsTurnedOn[3].on);
    ASSERT_FALSE(allLedsTurnedOn[4].on);
    ASSERT_FALSE(allLedsTurnedOn[5].on);
    ASSERT_TRUE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartColumnActivatorTest, RightRegister_testTurningOffTheThirdColumn)
{
    rightRegisterColumnActivator.turnOffColumn(3, allLedsTurnedOn);
    ASSERT_TRUE(allLedsTurnedOn[0].on);
    ASSERT_FALSE(allLedsTurnedOn[1].on);
    ASSERT_TRUE(allLedsTurnedOn[2].on);
    ASSERT_TRUE(allLedsTurnedOn[3].on);
    ASSERT_TRUE(allLedsTurnedOn[4].on);
    ASSERT_TRUE(allLedsTurnedOn[5].on);
    ASSERT_FALSE(allLedsTurnedOn[6].on);
}

TEST_F(LedHeartColumnActivatorTest, RightRegister_testTurningOffTheFouthColumn)
{
    rightRegisterColumnActivator.turnOffColumn(4, allLedsTurnedOn);
    ASSERT_FALSE(allLedsTurnedOn[0].on);
    ASSERT_TRUE(allLedsTurnedOn[1].on);
    ASSERT_TRUE(allLedsTurnedOn[2].on);
    ASSERT_TRUE(allLedsTurnedOn[3].on);
    ASSERT_TRUE(allLedsTurnedOn[4].on);
    ASSERT_TRUE(allLedsTurnedOn[5].on);
    ASSERT_TRUE(allLedsTurnedOn[6].on);
}
