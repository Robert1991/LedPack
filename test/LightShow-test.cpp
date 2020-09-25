#include "lightShow.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mockClasses.h"

using ::testing::Eq;
using ::testing::Exactly;
using ::testing::Mock;
using ::testing::Sequence;

const int EXPECTED_DELAY_INTERVAL = 10;
const int EXPECTED_BRIGHTNESS_FACTOR = 125;

class TestLightShowExecutionContainer : public LightShowExecutionContainer {
 private:
  static const int EXECUTION_COUNT = 3;

 public:
  TestLightShowExecutionContainer(IArduinoWrapper* testArduinoEnv, int delay)
      : LightShowExecutionContainer(testArduinoEnv, EXECUTION_COUNT, delay, 0) {}

  void executeNextStepOn(LedHeart* heart) {
    if (currentExecution == 0) {
      brightnessFactor = 10;
    } else if (currentExecution == 1) {
      brightnessFactor = 20;
    } else if (currentExecution == 2) {
      brightnessFactor = 30;
    }
  }
};

class LightShowTest : public ::testing::Test {
 public:
  LedHeartMock ledHeartMock;
  LightShowExecutionContainerIteratorMock lightShowExecutionContainerIteratorMock;
  TestArduinoEnvironment arduinoEnvMock;
  TestLightShowExecutionContainer* lightShowExecutionContainer;

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
  void SetUp() override { lightShowExecutionContainer = new TestLightShowExecutionContainer(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL); }
  void TearDown() override {
    Mock::VerifyAndClearExpectations(&lightShowExecutionContainerIteratorMock);
    Mock::VerifyAndClearExpectations(&ledHeartMock);
    Mock::VerifyAndClearExpectations(&arduinoEnvMock);
  }
};

TEST_F(LightShowTest,
       testThatTheLightShowExecutionContainerPlaysTheSequenceWhichWasInitializedUntilItsOverAndThenTakesTheLastSequenceBrightnessAndDelayAsDefault) {
  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL)));
  }

  lightShowExecutionContainer->executeOn(&ledHeartMock);
  ASSERT_TRUE(lightShowExecutionContainer->hasAnotherExecution());
  lightShowExecutionContainer->executeOn(&ledHeartMock);
  ASSERT_TRUE(lightShowExecutionContainer->hasAnotherExecution());
  lightShowExecutionContainer->executeOn(&ledHeartMock);
  ASSERT_FALSE(lightShowExecutionContainer->hasAnotherExecution());
  lightShowExecutionContainer->executeOn(&ledHeartMock);
}

TEST_F(LightShowTest, testThatTheLightShowExecutionContainerIteratorRepeatsTheGivenSequenceWhenTheLightShowReachesItsEnd) {
  LightShowExecutionContainer* containers[1] = {new TestLightShowExecutionContainer(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL)};
  auto iterator = LightShowExecutionContainerIterator(containers, 1);

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL)));
  }
  executeLightShowExecutionContainerIterator(&iterator, 6);
}

TEST_F(LightShowTest, testThatTheLightShowExecutionContainerIteratorRestartsTheLightShowWhenResetted) {
  int firstContainerDelay = 10;
  int secondContainerDelay = 20;
  LightShowExecutionContainer* containers[2] = {new TestLightShowExecutionContainer(&arduinoEnvMock, firstContainerDelay),
                                                new TestLightShowExecutionContainer(&arduinoEnvMock, secondContainerDelay)};
  auto iterator = LightShowExecutionContainerIterator(containers, 2);

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(firstContainerDelay)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(firstContainerDelay)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(firstContainerDelay)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(secondContainerDelay)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(firstContainerDelay)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(firstContainerDelay)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(firstContainerDelay)));
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(secondContainerDelay)));
  }

  executeLightShowExecutionContainerIterator(&iterator, 4);
  iterator.reset();
  executeLightShowExecutionContainerIterator(&iterator, 4);
}

TEST_F(LightShowTest, testThatTheLightShowSequencerApplysTheNextSequenceStepToTheHeartWhenCalled) {
  EXPECT_CALL(lightShowExecutionContainerIteratorMock, executeNextStepOn(&ledHeartMock)).Times(3);
  auto sequencer = LightShowSequencer(&ledHeartMock, &lightShowExecutionContainerIteratorMock);
  sequencer.executeIteration();
  sequencer.executeIteration();
  sequencer.executeIteration();
}

TEST_F(LightShowTest, testThatTheSequentialLedActivationExecutionFirstTurnsOfAllLedsOnTheBoardAndThenTurnsOnAllLedsFromAGivenStartIndex) {
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

TEST_F(LightShowTest,
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
    LightShowTest,
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
    LightShowTest,
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

TEST_F(
    LightShowTest,
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

TEST_F(LightShowTest, testThatTheRandomHeartBlinkExecutionAlwaysTurnsOffTheHeartInTheFirstIteration_CheckForResetWhenOn) {
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