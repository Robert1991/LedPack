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
    if (currentExecution == 1) {
      brightnessFactor = 10;
    } else if (currentExecution == 2) {
      brightnessFactor = 20;
    } else if (currentExecution == 3) {
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

// LightShowExecutionContainer tests
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

// LightShowSequencer tests
TEST_F(LightShowTest, testThatTheLightShowSequencerApplysTheNextSequenceStepToTheHeartWhenCalled) {
  EXPECT_CALL(lightShowExecutionContainerIteratorMock, executeNextStepOn(&ledHeartMock)).Times(3);
  auto sequencer = LightShowSequencer(&ledHeartMock, &lightShowExecutionContainerIteratorMock);
  sequencer.executeIteration();
  sequencer.executeIteration();
  sequencer.executeIteration();
}

// LightShowExecutionContainerRepeater tests
TEST_F(LightShowTest, testThatTheLightShowExecutionContainerRepeaterRepeatesTheGivenExecutionTheGivenNumberOfTimes) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(4));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(4));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
  }

  auto randomLedBlink = RandomHeartBlinkExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 1, 5);
  auto repeater = LightShowExecutionContainerRepeater(&arduinoEnvMock, &randomLedBlink, 2);
  executeLightShowExecutionContainer(&repeater, 4);
  ASSERT_FALSE(repeater.hasAnotherExecution());
}

TEST_F(LightShowTest, testThatTheLightShowExecutionContainerRepeaterRepeatesTheGivenExecutionTheGivenNumberOfTimes_TestWithDelayDecreaseFactor) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(4));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
  }

  auto randomLedBlink = RandomHeartBlinkExecution(&arduinoEnvMock, 200, EXPECTED_BRIGHTNESS_FACTOR, 1, 5);
  auto repeater = LightShowExecutionContainerRepeater(&arduinoEnvMock, &randomLedBlink, 2, 0.5);
  executeLightShowExecutionContainer(&repeater, 4);
  ASSERT_FALSE(repeater.hasAnotherExecution());
}

TEST_F(
    LightShowTest,
    testThatTheLightShowExecutionContainerRepeaterRepeatesTheGivenExecutionTheGivenNumberOfTimes_TestWithDelayDecreaseFactor_WhenResettingTheContainer) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(6));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
  }

  auto randomLedBlink = RandomHeartBlinkExecution(&arduinoEnvMock, 200, EXPECTED_BRIGHTNESS_FACTOR, 1, 5);
  auto repeater = LightShowExecutionContainerRepeater(&arduinoEnvMock, &randomLedBlink, 2, 0.5);
  executeLightShowExecutionContainer(&repeater, 3);
  repeater.reset();
  executeLightShowExecutionContainer(&repeater, 3);
}

// LightShowExecutionContainerSequence tests
TEST_F(LightShowTest, testExecutingALightShowExecutionContainerSequenceAndCheckThatTheContainersAreIterated) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(8));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(8));

  using ::testing::InSequence;
  {
    InSequence seq;
    // Random Heart blink execution
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));

    // Global Heart Blink execution
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnAll());
  }

  auto containerSequence =
      (new LightShowExecutionContainerSequence(2))
          ->addContainer(new RandomHeartBlinkExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 1, 5))
          ->addContainer(new GlobalHeartBlinkExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 3));

  executeLightShowExecutionContainer(containerSequence, 8);
  ASSERT_FALSE(containerSequence->hasAnotherExecution());
}

TEST_F(LightShowTest, testResettingTheLightShowExecutionContainerSequenceWhenExecutingIt2Times) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(12));
  EXPECT_CALL(arduinoEnvMock, delayFor(Eq(EXPECTED_DELAY_INTERVAL))).Times(Exactly(12));

  using ::testing::InSequence;
  {
    InSequence seq;
    // 1
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnAll());

    // 2
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));

    // Reset

    // 1
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnAll());

    // 2
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
  }

  auto containerSequence =
      (new LightShowExecutionContainerSequence(2, 2))
          ->addContainer(new RandomHeartBlinkExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 1, 5))
          ->addContainer(new GlobalHeartBlinkExecution(&arduinoEnvMock, EXPECTED_DELAY_INTERVAL, EXPECTED_BRIGHTNESS_FACTOR, 1));

  executeLightShowExecutionContainer(containerSequence, 6);
  containerSequence->reset();
  executeLightShowExecutionContainer(containerSequence, 6);
}

TEST_F(LightShowTest, testLightShowExecutionContainerSequenceWithDelayFactorAndNormalLightShowExecution) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(8));

  using ::testing::InSequence;
  {
    InSequence seq;

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(50)));
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(50)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
  }

  auto containerSequence = (new LightShowExecutionContainerSequence(2, 2, 0.5))
                               ->addContainer(new GlobalHeartBlinkExecution(&arduinoEnvMock, 100, EXPECTED_BRIGHTNESS_FACTOR, 1))
                               ->addContainer(new GlobalHeartBlinkExecution(&arduinoEnvMock, 200, EXPECTED_BRIGHTNESS_FACTOR, 1));

  executeLightShowExecutionContainer(containerSequence, 8);
}

TEST_F(LightShowTest, testLightShowExecutionContainerSequenceWithDelayFactorAndNormalLightShowExecutionWhenResettingTheContainer) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(8));

  using ::testing::InSequence;
  {
    InSequence seq;

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
  }

  auto containerSequence = (new LightShowExecutionContainerSequence(2, 2, 0.5))
                               ->addContainer(new GlobalHeartBlinkExecution(&arduinoEnvMock, 200, EXPECTED_BRIGHTNESS_FACTOR, 1))
                               ->addContainer(new GlobalHeartBlinkExecution(&arduinoEnvMock, 100, EXPECTED_BRIGHTNESS_FACTOR, 1));

  executeLightShowExecutionContainer(containerSequence, 6);
  containerSequence->reset();
  executeLightShowExecutionContainer(containerSequence, 2);
}

TEST_F(LightShowTest, testLightShowExecutionContainerSequenceWithDelayFactorAndLightShowExecutionContainerRepeaterAlsoHavingDelayFactor) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(8));

  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(50)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(50)));
  }

  auto randomLedBlink = RandomHeartBlinkExecution(&arduinoEnvMock, 200, EXPECTED_BRIGHTNESS_FACTOR, 1, 5);
  auto repeater = LightShowExecutionContainerRepeater(&arduinoEnvMock, &randomLedBlink, 2, 0.5);

  auto containerSequence = (new LightShowExecutionContainerSequence(1, 2, 0.5))->addContainer(&repeater);

  executeLightShowExecutionContainer(containerSequence, 8);
}

TEST_F(LightShowTest,
       testLightShowExecutionContainerSequenceWithDelayFactorAndLightShowExecutionContainerRepeaterAlsoHavingDelayFactor_WhenResettingTheSequence) {
  EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(EXPECTED_BRIGHTNESS_FACTOR))).Times(Exactly(16));

  using ::testing::InSequence;
  {
    InSequence seq;
    // 1
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(50)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(50)));

    // 2
    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, turnOffAll());
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(50)));
    EXPECT_CALL(ledHeartMock, turnOnRandomly(Eq(5)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(50)));
  }

  auto randomLedBlink = RandomHeartBlinkExecution(&arduinoEnvMock, 200, EXPECTED_BRIGHTNESS_FACTOR, 1, 5);
  auto repeater = LightShowExecutionContainerRepeater(&arduinoEnvMock, &randomLedBlink, 2, 0.5);

  auto containerSequence = (new LightShowExecutionContainerSequence(1, 2, 0.5))->addContainer(&repeater);

  executeLightShowExecutionContainer(containerSequence, 8);
  containerSequence->reset();
  executeLightShowExecutionContainer(containerSequence, 8);
}

TEST_F(LightShowTest, testLightShowExecutionContainerSequenceWithDelayFactorAndANestedLightShowExecutionContainerSequence) {
  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    // 2
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
  }

  auto randomLedBlink = TestLightShowExecutionContainer(&arduinoEnvMock, 400);
  auto nestedSequence = (new LightShowExecutionContainerSequence(1, 2, 0.5))->addContainer(&randomLedBlink);
  auto containerSequence = (new LightShowExecutionContainerSequence(1, 2, 0.5))->addContainer(nestedSequence);
  executeLightShowExecutionContainer(containerSequence, 12);
}

TEST_F(LightShowTest, testLightShowExecutionContainerSequenceWithDelayFactorAndANestedLightShowExecutionContainerSequenceWhenResettingTheSequence) {
  using ::testing::InSequence;
  {
    InSequence seq;
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    // 2
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
    // reset
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(200)));

    // 2
    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(400)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(10)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(20)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));

    EXPECT_CALL(ledHeartMock, toggleBrightness(Eq(30)));
    EXPECT_CALL(arduinoEnvMock, delayFor(Eq(100)));
  }

  auto randomLedBlink = TestLightShowExecutionContainer(&arduinoEnvMock, 400);
  auto nestedSequence = (new LightShowExecutionContainerSequence(1, 2, 0.5))->addContainer(&randomLedBlink);
  auto containerSequence = (new LightShowExecutionContainerSequence(1, 2, 0.5))->addContainer(nestedSequence);
  executeLightShowExecutionContainer(containerSequence, 12);
  containerSequence->reset();
  executeLightShowExecutionContainer(containerSequence, 12);
}