#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "lightShow.h"
#include "mockClasses.h"

using ::testing::Eq;
using ::testing::Exactly;
using ::testing::Mock;
using ::testing::Sequence;

const int EXPECTED_DELAY_INTERVAL = 10;
const int EXPECTED_BRIGHTNESS_FACTOR = 125;

class LightShowEffectsTest : public ::testing::Test {
 public:
  LedHeartMock ledHeartMock;
  TestArduinoEnvironment arduinoEnvMock;

  void executeLightShowExecutionContainer(LightShowExecutionContainer* executionContainer, int times) {
    for (int i = 0; i < times; i++) {
      executionContainer->executeOn(&ledHeartMock);
    }
  }

  void executeLightShowExecutionContainerIterator(LightShowExecutionContainerIterator* iterator, int times) {
    for (int i = 0; i < times; i++) {
      iterator->executeNextStepOn(&ledHeartMock);
    }
  }

 protected:
  void SetUp() override {}
  void TearDown() override {
    Mock::VerifyAndClearExpectations(&ledHeartMock);
    Mock::VerifyAndClearExpectations(&arduinoEnvMock);
  }
};

// GlobalHeartBlinkExecution tests
TEST_F(LightShowEffectsTest, testThatTheGlobalHeartBlinkExecutionFirstTurnsOffAllLedsAndThenTurnsOnAllLedsTheGivenNumberOfTimes) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(6));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(6));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnAll());
  }

  auto globalHeartBlink = GlobalHeartBlinkExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 3);
  executeLightShowExecutionContainer(&globalHeartBlink, 6);
}

// SequentialLedActivationExecution tests
TEST_F(LightShowEffectsTest, testThatTheSequentialLedActivationExecutionFirstTurnsOfAllLedsOnTheBoardAndThenTurnsOnAllLedsFromAGivenStartIndex) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(15));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(15));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOn(Eq(2)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(3)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(4)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(5)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(6)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(7)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(8)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(9)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(10)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(11)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(12)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(13)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(0)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(1)));
  }

  auto sequentialExecution = SequentialLedActivationExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 2);
  executeLightShowExecutionContainer(&sequentialExecution, 15);
}

TEST_F(LightShowEffectsTest,
       testThatTheSequentialLedActivationExecutionFirstTurnsOfAllLedsOnTheBoardAndThenTurnsOnAllLedsFromAGivenStartIndex_TestSpinningToTheLeftSide) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(15));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(15));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOn(Eq(2)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(1)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(0)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(13)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(12)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(11)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(10)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(9)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(8)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(7)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(6)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(5)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(4)));
    EXPECT_CALL(ledHeartMock, turnOn(Eq(3)));
  }

  auto sequentialExecution = SequentialLedActivationExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 2, false, true);
  executeLightShowExecutionContainer(&sequentialExecution, 15);
}

TEST_F(
    LightShowEffectsTest,
    testThatTheSequentialLedActivationExecutionFirstTurnsOfAllLedsOnTheBoardAndThenTurnsOnAllLedsFromAGivenStartIndexAndTurnsTheFormerLedOfWhenTheOptionIsSet) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(5));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(5));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());

    EXPECT_CALL(ledHeartMock, turnOn(Eq(0)));
    EXPECT_CALL(ledHeartMock, turnOff(Eq(13)));

    EXPECT_CALL(ledHeartMock, turnOn(Eq(1)));
    EXPECT_CALL(ledHeartMock, turnOff(Eq(0)));

    EXPECT_CALL(ledHeartMock, turnOn(Eq(2)));
    EXPECT_CALL(ledHeartMock, turnOff(Eq(1)));

    EXPECT_CALL(ledHeartMock, turnOn(Eq(3)));
    EXPECT_CALL(ledHeartMock, turnOff(Eq(2)));
  }

  auto sequentialExecution = SequentialLedActivationExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 0, true, false);
  executeLightShowExecutionContainer(&sequentialExecution, 5);
}

TEST_F(
    LightShowEffectsTest,
    testThatTheSequentialLedActivationExecutionFirstTurnsOfAllLedsOnTheBoardAndThenTurnsOnAllLedsFromAGivenStartIndexAndTurnsTheFormerLedOfWhenTheOptionIsSet_TestSpinningLeft) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(5));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(5));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());

    EXPECT_CALL(ledHeartMock, turnOn(Eq(0)));
    EXPECT_CALL(ledHeartMock, turnOff(Eq(1)));

    EXPECT_CALL(ledHeartMock, turnOn(Eq(13)));
    EXPECT_CALL(ledHeartMock, turnOff(Eq(0)));

    EXPECT_CALL(ledHeartMock, turnOn(Eq(12)));
    EXPECT_CALL(ledHeartMock, turnOff(Eq(13)));

    EXPECT_CALL(ledHeartMock, turnOn(Eq(11)));
    EXPECT_CALL(ledHeartMock, turnOff(Eq(12)));
  }

  auto sequentialExecution = SequentialLedActivationExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 0, true, true);
  executeLightShowExecutionContainer(&sequentialExecution, 5);
}

// RandomHeartBlinkExecution tests
TEST_F(
    LightShowEffectsTest,
    testThatTheRandomHeartBlinkExecutionFirstTurnsOffAllLedsAndThenTurnsThenOnRandomlyWithTheGivenMinimumCount_CheckThatAfterThatTheLedsAreTurnedOfAgain) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(6));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(6));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
  }

  auto randomLedBlink = RandomHeartBlinkExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 3, 5);
  executeLightShowExecutionContainer(&randomLedBlink, 6);
}

TEST_F(LightShowEffectsTest, testThatTheRandomHeartBlinkExecutionAlwaysTurnsOffTheHeartInTheFirstIteration_CheckForResetWhenOn) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(2));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(2));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOffAll());
  }

  auto randomLedBlink = RandomHeartBlinkExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 3, 5);
  // off
  randomLedBlink.executeOn(&ledHeartMock);
  // the next step would be on again
  randomLedBlink.reset();
  randomLedBlink.executeOn(&ledHeartMock);
}

// SequentialRowActivator tests
TEST_F(LightShowEffectsTest, testThatTheSequentialRowActivatorActivatesAllRowsFromAGivenStartIndex) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(6));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(6));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(2)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(3)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(4)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(5)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(6)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(1)));
  }

  auto sequentialRowActivation =
      (SequentialRowActivator::createUpwardsMovingRowActivator(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR))
          ->withStartIndex(2)
          ->turnOffPrevious(true);
  executeLightShowExecutionContainer(sequentialRowActivation, 6);
}

TEST_F(LightShowEffectsTest, testThatTheSequentialRowActivatorTurnsOnTheRowsOnTheHeartSequentiallyAndDoesNotTurnThemOffWhenGivenTheSetting) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(6));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(6));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(2)));
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(3)));
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(4)));
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(5)));
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(6)));
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(1)));
  }

  auto sequentialRowActivation =
      (SequentialRowActivator::createUpwardsMovingRowActivator(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR))
          ->withStartIndex(2)
          ->turnOffPrevious(false);
  executeLightShowExecutionContainer(sequentialRowActivation, 6);
}

TEST_F(LightShowEffectsTest, testThatTheSequentialRowActivatorActivatesAllRowsFromAGivenStartIndex_DowndwardsDirection) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(6));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(6));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(2)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(1)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(6)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(5)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(4)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnLevelOn(Eq(3)));
  }

  auto sequentialRowActivation =
      (SequentialRowActivator::createDownwardsMovingRowActivator(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR))
          ->withStartIndex(2)
          ->turnOffPrevious(true);
  executeLightShowExecutionContainer(sequentialRowActivation, 6);
}
