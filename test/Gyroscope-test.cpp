#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gyroscope.h"
#include "mockClasses.h"

using ::testing::Mock;
using ::testing::Return;
using ::testing::Exactly;
using ::testing::Eq;

class GyroscopeTest : public ::testing::Test
{
    public:
        WireMock wireMock;
        Gyroscope gyroscope;


    protected:
        void SetUp() override 
        {
            gyroscope = Gyroscope(&wireMock);
        }

        void TearDown() override 
        {
            Mock::VerifyAndClearExpectations(&wireMock);
        }
};


TEST_F(GyroscopeTest, testThatTheWireToTheGyroscopeGetsInitializedWhenTheGyroscopeIsInitialized)
{
    EXPECT_CALL(wireMock, initialize()).Times(Exactly(1));
    gyroscope.wakeUp();
}

TEST_F(GyroscopeTest, testThatTheGyroscopeFirstRequestsTheMeasurementOfTheWireAndThenReadsTheNextThreeRegistersInOrderToDetermineTheAccelerationVector)
{
    auto reference = AccelerationMeasurementVector(1, 2, 3);
    EXPECT_CALL(wireMock, requestMeasurement()).Times(Exactly(1));
    EXPECT_CALL(wireMock, readNextRegister()).Times(Exactly(3))
                                             .WillOnce(Return(reference.accX))
                                             .WillOnce(Return(reference.accY))
                                             .WillOnce(Return(reference.accZ));
    ASSERT_EQ(gyroscope.measureAcceleration(), reference);
}

TEST_F(GyroscopeTest, testThatToAccelerationMeasurementVectorIsEqualToItSelf) 
{
    auto vector = AccelerationMeasurementVector(1, 2, 3);
    ASSERT_EQ(vector, vector);
}

TEST_F(GyroscopeTest, testThatToAccelerationMeasurementVectorsAreEqualWhenTheyHaveTheSameAccerlerations) 
{
    ASSERT_EQ(AccelerationMeasurementVector(1, 2, 3), AccelerationMeasurementVector(1, 2, 3));
}

TEST_F(GyroscopeTest, testThatToAccelerationMeasurementVectorsAreNotEqualWhenTheyDontHaveTheSameAccerlerations) 
{
    ASSERT_NE(AccelerationMeasurementVector(1, 2, 3), AccelerationMeasurementVector(3, 2, 1));
}

TEST_F(GyroscopeTest, testCalculatingTheEucledianDistanceBetween2AccelerationMeasurements) 
{
    // sqrt ((1-2)^2 + (1-2)^2 + (1-2)^2) = sqrt(3)
    EXPECT_THAT(AccelerationMeasurementVector(1, 1, 1).euclideanDistanceTo(AccelerationMeasurementVector(2, 2, 2)), 
                Eq(AccerlationVectorDifference(sqrt(3.00f), sqrt(3.00f) / MAX_ACCELERATION_VECTOR_DIFFERENCE)));
}

TEST_F(GyroscopeTest, testThatTheEucledianDistanceIsZeroWhenCalculatingItToTheSameVector) 
{
    auto vector = AccelerationMeasurementVector(1, 1, 1);
    EXPECT_THAT(vector.euclideanDistanceTo(vector), Eq(AccerlationVectorDifference(0.00f, 0.00f)));
}

TEST_F(GyroscopeTest, testThatTheAccerlationVectorDifferenceDelegatesTheAccelerationDifferenceRatioToTheGivenAccelerationRatioMapper) 
{
    auto vector = AccelerationMeasurementVector(1, 1, 1);
    EXPECT_THAT(vector.euclideanDistanceTo(vector)
                      .mapAccelerationRatioTo(new DummyAccelerationRatioMapper()), Eq(0));
}

TEST_F(GyroscopeTest, testThatTheGivenRawInputIsOnlyOverTheThresholdWhenItIsGreaterThanTheDetectedThreshold) 
{
    EXPECT_THAT(AccelerationMeasurementVector(1, 1, 1).euclideanDistanceTo(AccelerationMeasurementVector(2, 2, 2))
                                                      .overThreshold(sqrt(3.00f)), 
                Eq(false));
}

TEST_F(GyroscopeTest, testThatTheVectorDifferenceIsNotOverTheThresholdWhenTheGivenThresholdIsGreaterThanTheRawVectorDifference) 
{
    EXPECT_THAT(AccelerationMeasurementVector(1, 1, 1).euclideanDistanceTo(AccelerationMeasurementVector(2, 2, 2))
                                                      .overThreshold(sqrt(3)), 
                Eq(false));
}

TEST_F(GyroscopeTest, testThatTheVectorDifferenceIsOverTheThresholdWhenTheGivenThresholdIsSmallerThanTheRawVectorDifference) 
{
    EXPECT_THAT(AccelerationMeasurementVector(1, 1, 1).euclideanDistanceTo(AccelerationMeasurementVector(2, 2, 2))
                                                      .overThreshold(sqrt(2.00f)), 
                Eq(true));
}