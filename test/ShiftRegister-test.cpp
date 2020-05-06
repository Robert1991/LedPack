#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "arduinoWrapper.h"
#include "mockClasses.h"
#include "shiftregister.h"

using ::testing::_;
using ::testing::AllOf;
using ::testing::Exactly;
using ::testing::Ne;
using ::testing::Mock;
using ::testing::NotNull;

class ShiftRegisterTest : public ::testing::Test
{
public:
    static const int LATCH_PIN = 10;
    static const int CLOCK_PIN = 11;
    static const int DATA_PIN = 12;
    static const int OUTPUT_ENABLE_PIN = 13;

    Led LEDS_ON_SHIFT_REGISTER[MAX_SHIFT_REGISTER_LED_COUNT] = {
        // Led number, position on shift register
        Led(1, 0),
        Led(2, 1),
        Led(3, 2),
        Led(4, 3),
        Led(5, 4),
        Led(6, 5),
        Led(7, 6)};

    Led ledsOnShiftRegister[MAX_SHIFT_REGISTER_LED_COUNT];

    const LedShiftRegisterPins SHIFT_REGISTER_PINS = {LATCH_PIN, CLOCK_PIN, DATA_PIN, OUTPUT_ENABLE_PIN};

    TestArduinoEnvironment testArduinoEnv;
    ColumnActivatorMock columnActivatorMock;
    LevelActivatorMock levelActivatorMock;

    LedShiftRegister shiftRegister;

protected:
    void SetUp() override
    {
        for (int i = 0; i < MAX_SHIFT_REGISTER_LED_COUNT; i++)
        {
            ledsOnShiftRegister[i] = Led(LEDS_ON_SHIFT_REGISTER[i].shiftRegisterPosition, i);
        }
        testArduinoEnv.DelegateToFake();
        shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, &levelActivatorMock, &columnActivatorMock, &testArduinoEnv);
    }

    void TearDown() override
    {
        Mock::VerifyAndClearExpectations(&testArduinoEnv);
        Mock::VerifyAndClearExpectations(&columnActivatorMock);
        Mock::VerifyAndClearExpectations(&levelActivatorMock);
    }
};

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterTurnsOnTheLedWithTheGivenIndexByTheBoardNumberAndUpdatesTheShiftRegisterInTheRightWay)
{
    EXPECT_CALL(testArduinoEnv, clearBit(_, Ne(2))).Times(Exactly(6));
    EXPECT_CALL(testArduinoEnv, setBit(_, 2)).Times(Exactly(1));

    // Byte gets written to shift register
    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    // Resulting bit string: 0010000 -> 4
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, (byte)4)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    shiftRegister.turnOn(2);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterSendsTheSameBitAgainWhenTheSameLEDIsTurnedOnTwice)
{
    EXPECT_CALL(testArduinoEnv, clearBit(_, Ne(3))).Times(Exactly(12));
    EXPECT_CALL(testArduinoEnv, setBit(_, 3)).Times(Exactly(2));

    // Byte gets written to shift register
    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(2));
    // Resulting bit string: 0001000 -> 8
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, (byte)8)).Times(Exactly(2));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(2));

    shiftRegister.turnOn(3);
    shiftRegister.turnOn(3);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterNeverSendsAnyBitStringUpdateWhenALedWithANegativeIndexIsTurnedOn)
{
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, setBit(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOn(-1);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterNeverSendsAnyBitStringUpdateWhenALedWithAnIndexGreaterThan7IsTurnedOn)
{
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, setBit(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOn(MAX_SHIFT_REGISTER_LED_COUNT + 1);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterWillTurnOffAFormerlyTurnedOnLEDWhenItIsTurnedOfAfterwards)
{
    EXPECT_CALL(testArduinoEnv, clearBit(_, Ne(1))).Times(Exactly(12));
    EXPECT_CALL(testArduinoEnv, clearBit(_, 1)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, setBit(_, 1)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(2));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, 2)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, 0)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(2));

    shiftRegister.turnOn(1);
    shiftRegister.turnOff(1);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterWillSendTheSameBitStringToTheRegisterAgainWhenAFormallyTurnedOffLedIsTurnedOffAgain)
{
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(14));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(2));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, 0)).Times(Exactly(2));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(2));

    shiftRegister.turnOff(1);
    shiftRegister.turnOff(1);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterNeverUpdatesTheBitStringWhenALedIsTurnedOffUsingANegativeLedIndex)
{
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, setBit(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOff(-1);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterNeverUpdatesTheBitStringWhenALedIsTurnedOffUsingALedIndexGreaterThan7)
{
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, setBit(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOff(MAX_SHIFT_REGISTER_LED_COUNT + 1);
}

TEST_F(ShiftRegisterTest, testThatAFormallyTurnedOnLedStaysTurnedOnWhenAnOperationOnAnOtherLedIsPerformedAfterwards)
{
    EXPECT_CALL(testArduinoEnv, clearBit(_, AllOf(Ne(0), Ne(3)))).Times(Exactly(10));
    EXPECT_CALL(testArduinoEnv, setBit(_, 3)).Times(Exactly(2));
    EXPECT_CALL(testArduinoEnv, clearBit(_, 0)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, setBit(_, 0)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(2));
    // sending --> 0001000
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, (byte)8)).Times(Exactly(1));
    // sending --> 1001000
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, (byte)9)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(2));

    shiftRegister.turnOn(3);
    shiftRegister.turnOn(0);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterTurnsOffAllLedsByUpdatingTheBitStringTo0WhenAllLedsShouldBeTurnedOff)
{
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(7));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    // sending --> 0000000
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, (byte)0)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    shiftRegister.turnOffAll();
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterTurnsOnAllLedsByUpdatingTheBitStringToMaxValueWhenAllLedsShouldBeTurnedOn)
{
    EXPECT_CALL(testArduinoEnv, setBit(_, _)).Times(Exactly(7));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    // sending --> 1111111
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, (byte)(127))).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    shiftRegister.turnOnAll();
}

TEST_F(ShiftRegisterTest, testThatTheLedBrightnessIsToggledByWritingTheCorrespondingValueToTheOutputEnablePin)
{
    byte toggledBrightness = (byte)50;
    EXPECT_CALL(testArduinoEnv, writeToAnalog(OUTPUT_ENABLE_PIN, 255 - toggledBrightness)).Times(Exactly(1));

    shiftRegister.toggleBrightness(toggledBrightness);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterInitializesAllShiftRegisterPinsInOutputModeOnTheBoardWhenInitialized)
{
    EXPECT_CALL(testArduinoEnv, setPinToOutputMode(CLOCK_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, setPinToOutputMode(DATA_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, setPinToOutputMode(LATCH_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, setPinToOutputMode(OUTPUT_ENABLE_PIN)).Times(Exactly(1));

    shiftRegister.initializePins();
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorTurnsOnAGivenLevelWhenItIsInSpecificBoundsAlsoCheckThatTheShiftRegisterUpdatesTheShiftRegisterBitAfterALevelWasTurnedOn)
{
    int turnedOn = 4;
    // the level activator is a mock in this case and does not directly change the led status
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(7));
    EXPECT_CALL(levelActivatorMock, turnOnLevel(turnedOn, _)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    shiftRegister.turnOnLevel(turnedOn);
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorNeverTurnsOnALevelWhenTheGivenLevelIndexIsNegative)
{
    EXPECT_CALL(levelActivatorMock, turnOnLevel(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOnLevel(-1);
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorNeverTurnsOnALevelWhenTheGivenLevelIndexIsExceedsTheCountOfLevelsOnTheBoard)
{
    EXPECT_CALL(levelActivatorMock, turnOnLevel(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOnLevel(10);
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorTurnsOffAGivenLevelWhenItIsInSpecificBoundsAlsoCheckThatTheShiftRegisterUpdatesTheShiftRegisterBitAfterALevelWasTurnedOff)
{
    int turnedOn = MAX_LEVEL;
    // the level activator is a mock in this case and does not directly change the led status
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(7));
    EXPECT_CALL(levelActivatorMock, turnOffLevel(turnedOn, _)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    shiftRegister.turnOffLevel(turnedOn);
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorNeverTurnsOffALevelWhenTheGivenLevelIndexIsNegative)
{
    EXPECT_CALL(levelActivatorMock, turnOffLevel(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOffLevel(-1);
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorNeverTurnsOffALevelWhenTheGivenLevelIndexIsExceedsTheCountOfLevelsOnTheBoard)
{
    int turnedOn = MAX_LEVEL + 1;
    EXPECT_CALL(levelActivatorMock, turnOffLevel(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOffLevel(turnedOn);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorTurnsOnAGivenColumnWhenItIsInSpecificBoundsAlsoCheckThatTheShiftRegisterUpdatesTheShiftRegisterBitAfterAColumnWasTurnedOn)
{
    int turnedOn = MAX_COLUMNS;
    // the column activator is a mock in this case and does not directly change the led status
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(7));
    EXPECT_CALL(columnActivatorMock, turnOnColumn(turnedOn, _)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    shiftRegister.turnOnColumn(turnedOn);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorNeverTurnsOnAColumnWhenTheColumnIndexIsNegativeAlsoCheckThatTheShiftRegisterBitIsNotUpdatedInThisCase)
{
    EXPECT_CALL(columnActivatorMock, turnOnColumn(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

     shiftRegister.turnOnColumn(-1);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorNeverTurnsOnAColumnWhenTheColumnIndexExceedsTheMaximumColumnCountOnTheRegister)
{
    int turnedOnColumn = MAX_COLUMNS + 1;

    EXPECT_CALL(columnActivatorMock, turnOnColumn(turnedOnColumn, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOnColumn(turnedOnColumn);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorTurnsOffAGivenColumnWhenItIsInSpecificBoundsAlsoCheckThatTheShiftRegisterUpdatesTheShiftRegisterBitAfterAColumnWasTurnedOff)
{
    int turnedOff = MAX_COLUMNS;
    // the column activator is a mock in this case and does not directly change the led status
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(7));
    EXPECT_CALL(columnActivatorMock, turnOffColumn(turnedOff, _)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    shiftRegister.turnOffColumn(turnedOff);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorNeverTurnsOffAColumnWhenTheColumnIndexIsNegativeAlsoCheckThatTheShiftRegisterBitIsNotUpdatedInThisCase)
{
    EXPECT_CALL(columnActivatorMock, turnOffColumn(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOffColumn(-1);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorNeverTurnsOffAColumnWhenTheColumnIndexExceedsTheMaximumColumnCountOnTheRegister)
{
    int turnedOnColumn = MAX_COLUMNS + 1;

    EXPECT_CALL(columnActivatorMock, turnOffColumn(turnedOnColumn, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    shiftRegister.turnOffColumn(turnedOnColumn);
}
