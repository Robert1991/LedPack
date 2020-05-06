#include "gtest/gtest.h"
#include "microphone.h"
#include "arduinoWrapper.h"
#include "mockClasses.h"
#include "gmock/gmock.h"

using ::testing::Exactly;
using ::testing::Mock;
using ::testing::Return;

const int MICROPHONE_ANALOG_PIN = 5;
const int MICROPHONE_DIGITAL_PIN = 10;

Microphone *microphone;

class MicrophoneTest : public ::testing::Test
{
protected:
    static Microphone* CreateMicrophoneWith(TestArduinoEnvironment *testArduinoEnv) {
        return new Microphone(testArduinoEnv, MICROPHONE_ANALOG_PIN, MICROPHONE_DIGITAL_PIN);
    }

    void TearDown() override
    {
        delete microphone;
    }
};

TEST_F(MicrophoneTest, testThatTheMicrophoneInitializesTheDigitalPinToInputModeWhenItIsInitialized)
{
    TestArduinoEnvironment testArduinoEnv;
    EXPECT_CALL(testArduinoEnv, setPinToInputMode(MICROPHONE_DIGITAL_PIN)).Times(Exactly(1));
    microphone = CreateMicrophoneWith(&testArduinoEnv);
    microphone -> init();
}

TEST_F(MicrophoneTest, testThatTheMicrophoneReadsTheAnalogPinAndPassesTheValueToItsClientWhenTheMicrophoneFrequenceIsRead)
{
    TestArduinoEnvironment testArduinoEnv;
    EXPECT_CALL(testArduinoEnv, readAnalogFrom(MICROPHONE_ANALOG_PIN)).Times(Exactly(1))
                                                                       .WillOnce(Return(25));
    microphone = CreateMicrophoneWith(&testArduinoEnv);
    ASSERT_EQ(microphone->readAnalog(), 25);
}

TEST_F(MicrophoneTest, testThatTheMicrophoneReadsTheDigitalPinAndPassesTheValueToItsClientWhenTheDigitalSwitchOnTheMicrophoneIsRead)
{
    TestArduinoEnvironment testArduinoEnv;
    EXPECT_CALL(testArduinoEnv, readDigitalFrom(MICROPHONE_DIGITAL_PIN)).Times(Exactly(1))
                                                                         .WillOnce(Return(0));
    microphone = CreateMicrophoneWith(&testArduinoEnv);
    ASSERT_EQ(microphone->readDigital(), 0);
}