#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "microphone.h"
#include "arduinoWrapper.h"
#include "mockClasses.h"

using ::testing::Exactly;
using ::testing::Mock;
using ::testing::Return;

class MicrophoneTest : public ::testing::Test
{
public:
    static const int MICROPHONE_ANALOG_PIN = 5;
    static const int MICROPHONE_DIGITAL_PIN = 10;

    
    TestArduinoEnvironment testArduinoEnv;
    Microphone microphone;

protected:
    void SetUp() override 
    {
        microphone = Microphone(&testArduinoEnv, MICROPHONE_ANALOG_PIN, MICROPHONE_DIGITAL_PIN);
    }

    void TearDown() override
    {
        Mock::VerifyAndClearExpectations(&testArduinoEnv);
    }
};

TEST_F(MicrophoneTest, testThatTheMicrophoneInitializesTheDigitalPinToInputModeWhenItIsInitialized)
{
    EXPECT_CALL(testArduinoEnv, setPinToInputMode(MICROPHONE_DIGITAL_PIN)).Times(Exactly(1));
    microphone.init();
}

TEST_F(MicrophoneTest, testThatTheMicrophoneReadsTheAnalogPinAndPassesTheValueToItsClientWhenTheMicrophoneFrequenceIsRead)
{
    EXPECT_CALL(testArduinoEnv, readAnalogFrom(MICROPHONE_ANALOG_PIN)).Times(Exactly(1))
                                                                       .WillOnce(Return(25));
    ASSERT_EQ(microphone.readAnalog(), 25);
}

TEST_F(MicrophoneTest, testThatTheMicrophoneReadsTheDigitalPinAndPassesTheValueToItsClientWhenTheDigitalSwitchOnTheMicrophoneIsRead)
{
    EXPECT_CALL(testArduinoEnv, readDigitalFrom(MICROPHONE_DIGITAL_PIN)).Times(Exactly(1))
                                                                         .WillOnce(Return(0));
    ASSERT_EQ(microphone.readDigital(), 0);
}