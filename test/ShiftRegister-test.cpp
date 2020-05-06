#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "arduinoWrapper.h"
#include "mockClasses.h"
#include "shiftregister.h"

using ::testing::_;
using ::testing::AllOf;
using ::testing::Exactly;
using ::testing::Ne;
using ::testing::NotNull;

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

const int LATCH_PIN = 10;
const int CLOCK_PIN = 11;
const int DATA_PIN = 12;
const int OUTPUT_ENABLE_PIN = 13;

const LedShiftRegisterPins SHIFT_REGISTER_PINS = {LATCH_PIN, CLOCK_PIN, DATA_PIN, OUTPUT_ENABLE_PIN};

class ShiftRegisterTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        for (int i = 0; i < MAX_SHIFT_REGISTER_LED_COUNT; i++)
        {
            ledsOnShiftRegister[i] = LEDS_ON_SHIFT_REGISTER[i];
        }
    }
};

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterTurnsOnTheLedWithTheGivenIndexByTheBoardNumberAndUpdatesTheShiftRegisterInTheRightWay)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

    EXPECT_CALL(testArduinoEnv, clearBit(_, Ne(2))).Times(Exactly(6));
    EXPECT_CALL(testArduinoEnv, setBit(_, 2)).Times(Exactly(1));

    // Byte gets written to shift register
    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    // Resulting bit string: 0010000 -> 4
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, (byte)4)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOn(2);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterSendsTheSameBitAgainWhenTheSameLEDIsTurnedOnTwice)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

    EXPECT_CALL(testArduinoEnv, clearBit(_, Ne(3))).Times(Exactly(12));
    EXPECT_CALL(testArduinoEnv, setBit(_, 3)).Times(Exactly(2));

    // Byte gets written to shift register
    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(2));
    // Resulting bit string: 0001000 -> 8
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, (byte)8)).Times(Exactly(2));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(2));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOn(3);
    shiftRegister.turnOn(3);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterNeverSendsAnyBitStringUpdateWhenALedWithANegativeIndexIsTurnedOn)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, setBit(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOn(-1);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterNeverSendsAnyBitStringUpdateWhenALedWithAnIndexGreaterThan7IsTurnedOn)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, setBit(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOn(MAX_SHIFT_REGISTER_LED_COUNT + 1);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterWillTurnOffAFormerlyTurnedOnLEDWhenItIsTurnedOfAfterwards)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

    EXPECT_CALL(testArduinoEnv, clearBit(_, Ne(1))).Times(Exactly(12));
    EXPECT_CALL(testArduinoEnv, clearBit(_, 1)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, setBit(_, 1)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(2));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, 2)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, 0)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(2));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOn(1);
    shiftRegister.turnOff(1);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterWillSendTheSameBitStringToTheRegisterAgainWhenAFormallyTurnedOffLedIsTurnedOffAgain)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(14));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(2));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, 0)).Times(Exactly(2));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(2));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOff(1);
    shiftRegister.turnOff(1);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterNeverUpdatesTheBitStringWhenALedIsTurnedOffUsingANegativeLedIndex)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, setBit(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOff(-1);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterNeverUpdatesTheBitStringWhenALedIsTurnedOffUsingALedIndexGreaterThan7)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, setBit(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOff(MAX_SHIFT_REGISTER_LED_COUNT + 1);
}

TEST_F(ShiftRegisterTest, testThatAFormallyTurnedOnLedStaysTurnedOnWhenAnOperationOnAnOtherLedIsPerformedAfterwards)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

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

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOn(3);
    shiftRegister.turnOn(0);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterTurnsOffAllLedsByUpdatingTheBitStringTo0WhenAllLedsShouldBeTurnedOff)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(7));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    // sending --> 0000000
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, (byte)0)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOffAll();
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterTurnsOnAllLedsByUpdatingTheBitStringToMaxValueWhenAllLedsShouldBeTurnedOn)
{
    TestArduinoEnvironment testArduinoEnv;
    testArduinoEnv.DelegateToFake();

    EXPECT_CALL(testArduinoEnv, setBit(_, _)).Times(Exactly(7));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    // sending --> 1111111
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, (byte)(127))).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.turnOnAll();
}

TEST_F(ShiftRegisterTest, testThatTheLedBrightnessIsToggledByWritingTheCorrespondingValueToTheOutputEnablePin)
{
    TestArduinoEnvironment testArduinoEnv;

    byte toggledBrightness = (byte)50;

    EXPECT_CALL(testArduinoEnv, writeToAnalog(OUTPUT_ENABLE_PIN, 255 - toggledBrightness)).Times(Exactly(1));
    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.toggleBrightness(toggledBrightness);
}

TEST_F(ShiftRegisterTest, testThatTheShiftRegisterInitializesAllShiftRegisterPinsInOutputModeOnTheBoardWhenInitialized)
{
    TestArduinoEnvironment testArduinoEnv;

    EXPECT_CALL(testArduinoEnv, setPinToOutputMode(CLOCK_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, setPinToOutputMode(DATA_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, setPinToOutputMode(LATCH_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, setPinToOutputMode(OUTPUT_ENABLE_PIN)).Times(Exactly(1));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, nullptr, &testArduinoEnv);
    shiftRegister.initializePins();
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorTurnsOnAGivenLevelWhenItIsInSpecificBoundsAlsoCheckThatTheShiftRegisterUpdatesTheShiftRegisterBitAfterALevelWasTurnedOn)
{
    TestArduinoEnvironment testArduinoEnv;
    LevelActivatorMock levelActivatorMock;

    int turnedOn = 4;
    // the level activator is a mock in this case and does not directly change the led status
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(7));
    EXPECT_CALL(levelActivatorMock, turnOnLevel(turnedOn, _)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, &levelActivatorMock, nullptr, &testArduinoEnv);
    shiftRegister.turnOnLevel(turnedOn);
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorNeverTurnsOnALevelWhenTheGivenLevelIndexIsNegative)
{
    TestArduinoEnvironment testArduinoEnv;
    LevelActivatorMock levelActivatorMock;

    EXPECT_CALL(levelActivatorMock, turnOnLevel(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, &levelActivatorMock, nullptr, &testArduinoEnv);
    shiftRegister.turnOnLevel(-1);
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorNeverTurnsOnALevelWhenTheGivenLevelIndexIsExceedsTheCountOfLevelsOnTheBoard)
{
    TestArduinoEnvironment testArduinoEnv;
    LevelActivatorMock levelActivatorMock;

    EXPECT_CALL(levelActivatorMock, turnOnLevel(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, &levelActivatorMock, nullptr, &testArduinoEnv);
    shiftRegister.turnOnLevel(10);
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorTurnsOffAGivenLevelWhenItIsInSpecificBoundsAlsoCheckThatTheShiftRegisterUpdatesTheShiftRegisterBitAfterALevelWasTurnedOff)
{
    TestArduinoEnvironment testArduinoEnv;
    LevelActivatorMock levelActivatorMock;

    int turnedOn = MAX_LEVEL;
    // the level activator is a mock in this case and does not directly change the led status
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(7));
    EXPECT_CALL(levelActivatorMock, turnOffLevel(turnedOn, _)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, &levelActivatorMock, nullptr, &testArduinoEnv);
    shiftRegister.turnOffLevel(turnedOn);
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorNeverTurnsOffALevelWhenTheGivenLevelIndexIsNegative)
{
    TestArduinoEnvironment testArduinoEnv;
    LevelActivatorMock levelActivatorMock;

    EXPECT_CALL(levelActivatorMock, turnOffLevel(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, &levelActivatorMock, nullptr, &testArduinoEnv);
    shiftRegister.turnOffLevel(-1);
}

TEST_F(ShiftRegisterTest, testThatTheLevelActivatorNeverTurnsOffALevelWhenTheGivenLevelIndexIsExceedsTheCountOfLevelsOnTheBoard)
{
    TestArduinoEnvironment testArduinoEnv;
    LevelActivatorMock levelActivatorMock;

    int turnedOn = MAX_LEVEL + 1;
    EXPECT_CALL(levelActivatorMock, turnOffLevel(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, &levelActivatorMock, nullptr, &testArduinoEnv);
    shiftRegister.turnOffLevel(turnedOn);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorTurnsOnAGivenColumnWhenItIsInSpecificBoundsAlsoCheckThatTheShiftRegisterUpdatesTheShiftRegisterBitAfterAColumnWasTurnedOn)
{
    TestArduinoEnvironment testArduinoEnv;
    ColumnActivatorMock columnActivatorMock;

    int turnedOn = MAX_COLUMNS;
    // the column activator is a mock in this case and does not directly change the led status
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(7));
    EXPECT_CALL(columnActivatorMock, turnOnColumn(turnedOn, _)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, &columnActivatorMock, &testArduinoEnv);
    shiftRegister.turnOnColumn(turnedOn);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorNeverTurnsOnAColumnWhenTheColumnIndexIsNegativeAlsoCheckThatTheShiftRegisterBitIsNotUpdatedInThisCase)
{
    TestArduinoEnvironment testArduinoEnv;
    ColumnActivatorMock columnActivatorMock;

    EXPECT_CALL(columnActivatorMock, turnOnColumn(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, &columnActivatorMock, &testArduinoEnv);
    shiftRegister.turnOnColumn(-1);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorNeverTurnsOnAColumnWhenTheColumnIndexExceedsTheMaximumColumnCountOnTheRegister)
{
    TestArduinoEnvironment testArduinoEnv;
    ColumnActivatorMock columnActivatorMock;

    int turnedOnColumn = MAX_COLUMNS + 1;

    EXPECT_CALL(columnActivatorMock, turnOnColumn(turnedOnColumn, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, &columnActivatorMock, &testArduinoEnv);
    shiftRegister.turnOnColumn(turnedOnColumn);
}


TEST_F(ShiftRegisterTest, testThatTheColumnActivatorTurnsOffAGivenColumnWhenItIsInSpecificBoundsAlsoCheckThatTheShiftRegisterUpdatesTheShiftRegisterBitAfterAColumnWasTurnedOff)
{
    TestArduinoEnvironment testArduinoEnv;
    ColumnActivatorMock columnActivatorMock;

    int turnedOff = MAX_COLUMNS;
    // the column activator is a mock in this case and does not directly change the led status
    EXPECT_CALL(testArduinoEnv, clearBit(_, _)).Times(Exactly(7));
    EXPECT_CALL(columnActivatorMock, turnOffColumn(turnedOff, _)).Times(Exactly(1));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(1));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(1));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, &columnActivatorMock, &testArduinoEnv);
    shiftRegister.turnOffColumn(turnedOff);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorNeverTurnsOffAColumnWhenTheColumnIndexIsNegativeAlsoCheckThatTheShiftRegisterBitIsNotUpdatedInThisCase)
{
    TestArduinoEnvironment testArduinoEnv;
    ColumnActivatorMock columnActivatorMock;

    EXPECT_CALL(columnActivatorMock, turnOffColumn(_, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, &columnActivatorMock, &testArduinoEnv);
    shiftRegister.turnOffColumn(-1);
}

TEST_F(ShiftRegisterTest, testThatTheColumnActivatorNeverTurnsOffAColumnWhenTheColumnIndexExceedsTheMaximumColumnCountOnTheRegister)
{
    TestArduinoEnvironment testArduinoEnv;
    ColumnActivatorMock columnActivatorMock;

    int turnedOnColumn = MAX_COLUMNS + 1;

    EXPECT_CALL(columnActivatorMock, turnOffColumn(turnedOnColumn, _)).Times(Exactly(0));

    EXPECT_CALL(testArduinoEnv, writeLOWToDigital(LATCH_PIN)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, shift(DATA_PIN, CLOCK_PIN, _)).Times(Exactly(0));
    EXPECT_CALL(testArduinoEnv, writeHIGHToDigital(LATCH_PIN)).Times(Exactly(0));

    LedShiftRegister shiftRegister = LedShiftRegister(SHIFT_REGISTER_PINS, ledsOnShiftRegister, nullptr, &columnActivatorMock, &testArduinoEnv);
    shiftRegister.turnOffColumn(turnedOnColumn);
}
