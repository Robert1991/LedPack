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
    AccelerationMeasurementVector reference = AccelerationMeasurementVector(1, 2, 3);
    EXPECT_CALL(wireMock, requestMeasurement()).Times(Exactly(1));
    EXPECT_CALL(wireMock, readNextRegister()).Times(Exactly(3))
                                             .WillOnce(Return(reference.accX))
                                             .WillOnce(Return(reference.accY))
                                             .WillOnce(Return(reference.accZ));
    ASSERT_EQ(gyroscope.measureAcceleration(), reference);
}

TEST_F(GyroscopeTest, testThatToAccelerationMeasurementVectorIsEqualToItSelf) 
{
    AccelerationMeasurementVector vector = AccelerationMeasurementVector(1, 2, 3);
    ASSERT_EQ(vector, vector);
}

TEST_F(GyroscopeTest, testThatToAccelerationMeasurementVectorsAreEqualWhenTheyHaveTheSameAccerlerations) 
{
    AccelerationMeasurementVector vector1 = AccelerationMeasurementVector(1, 2, 3);
    AccelerationMeasurementVector vector2 = AccelerationMeasurementVector(1, 2, 3);
    ASSERT_EQ(vector1, vector2);
}

TEST_F(GyroscopeTest, testThatToAccelerationMeasurementVectorsAreNotEqualWhenTheyDontHaveTheSameAccerlerations) 
{
    AccelerationMeasurementVector vector1 = AccelerationMeasurementVector(1, 2, 3);
    AccelerationMeasurementVector vector2 = AccelerationMeasurementVector(3, 2, 1);
    ASSERT_NE(vector1, vector2);
}

TEST_F(GyroscopeTest, testCalculatingTheEucledianDistanceBetween2AccelerationMeasurements) 
{
    AccelerationMeasurementVector firstVector = AccelerationMeasurementVector(1, 1, 1);
    AccelerationMeasurementVector secondVector = AccelerationMeasurementVector(2, 2, 2);
    // sqrt ((1-2)^2 + (1-2)^2 + (1-2)^2) = sqrt(3)
    EXPECT_THAT(firstVector.euclideanDistanceTo(secondVector), Eq(AccerlationVectorDifference(sqrt(3.00f), 
                                                                                              sqrt(3.00f) / MAX_ACCELERATION_VECTOR_DIFFERENCE)));
}

TEST_F(GyroscopeTest, testThatTheEucledianDistanceIsZeroWhenCalculatingItToTheSameVector) 
{
    AccelerationMeasurementVector vector = AccelerationMeasurementVector(1, 1, 1);
    EXPECT_THAT(vector.euclideanDistanceTo(vector), Eq(AccerlationVectorDifference(0.00f, 0.00f)));
}

TEST_F(GyroscopeTest, testThatTheAccerlationVectorDifferenceDelegatesTheAccelerationDifferenceRatioToTheGivenAccelerationRatioMapper) 
{
    AccelerationMeasurementVector vector = AccelerationMeasurementVector(1, 1, 1);
    EXPECT_THAT(vector.euclideanDistanceTo(vector)
                      .mapAccelerationRatioTo(new DummyAccelerationRatioMapper()), Eq(0));
}

TEST_F(GyroscopeTest, testThatTheGivenRawInputIsOnlyOverTheThresholdWhenItIsGreaterThanTheDetectedThreshold) 
{
    AccelerationMeasurementVector firstVector = AccelerationMeasurementVector(1, 1, 1);
    AccelerationMeasurementVector secondVector = AccelerationMeasurementVector(2, 2, 2);
    EXPECT_THAT(firstVector.euclideanDistanceTo(secondVector)
                           .overThreshold(sqrt(3.00f)), Eq(false));
}

TEST_F(GyroscopeTest, testThatTheVectorDifferenceIsNotOverTheThresholdWhenTheGivenThresholdIsGreaterThanTheRawVectorDifference) 
{
    AccelerationMeasurementVector firstVector = AccelerationMeasurementVector(1, 1, 1);
    AccelerationMeasurementVector secondVector = AccelerationMeasurementVector(2, 2, 2);
    EXPECT_THAT(firstVector.euclideanDistanceTo(secondVector)
                           .overThreshold(3), Eq(false));
}

TEST_F(GyroscopeTest, testThatTheVectorDifferenceIsOverTheThresholdWhenTheGivenThresholdIsSmallerThanTheRawVectorDifference) 
{
    AccelerationMeasurementVector firstVector = AccelerationMeasurementVector(1, 1, 1);
    AccelerationMeasurementVector secondVector = AccelerationMeasurementVector(2, 2, 2);
    EXPECT_THAT(firstVector.euclideanDistanceTo(secondVector)
                           .overThreshold(sqrt(2.00f)), Eq(true));
}