#include <exception>
#include <cmath>

#include <gtest/gtest.h>

#include "atomtex_spe_file/measurement.hpp"

using namespace atomtex_spe_file;

TEST(LongitudeTest, ConstructorWorks)
{
    Longitude lon{10};
    EXPECT_FLOAT_EQ(lon.DecimalDegrees(), double{10});
}

TEST(LongitudeTest, TooLargeDD)
{
    EXPECT_THROW(Longitude{180.0001}, std::exception);
}

TEST(LongitudeTest, TooSmallDD)
{
    EXPECT_THROW(Longitude{-180.0001}, std::exception);
}

TEST(LongitudeTest, NaN)
{
    EXPECT_THROW(Longitude{NAN}, std::exception);
}

TEST(LatitudeTest, ConstructorWorks)
{
    Latitude lat{10};
    EXPECT_FLOAT_EQ(lat.DecimalDegrees(), double{10});
}

TEST(LatitudeTest, TooLargeDD)
{
    EXPECT_THROW(Latitude{90.0001}, std::exception);
}

TEST(LatitudeTest, TooSmallDD)
{
    EXPECT_THROW(Latitude{-90.0001}, std::exception);
}

TEST(LatitudeTest, NaN)
{
    EXPECT_THROW(Latitude{NAN}, std::exception);
}

TEST(DoseRateTest, ConstructorWorks)
{
    DoseRate doseRate{10};
    EXPECT_FLOAT_EQ(doseRate.MicroSvPerHour(), double{10});
}

TEST(DoseRateTest, TooSmallMicroSvPerHour)
{
    EXPECT_THROW(DoseRate{-0.0001}, std::exception);
}

TEST(DoseRateTest, NaN)
{
    EXPECT_THROW(DoseRate{NAN}, std::exception);
}
