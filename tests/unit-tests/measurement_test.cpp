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

TEST(LongitudeTest, ConstructorWorksFromString)
{
    {
        Longitude lon{"10"};
        EXPECT_FLOAT_EQ(lon.DecimalDegrees(), double{10});
    }
    {
        Longitude lon{"-0.02"};
        EXPECT_FLOAT_EQ(lon.DecimalDegrees(), double{-0.02});
    }
}

TEST(LongitudeTest, TooLargeDDFromString)
{
    EXPECT_THROW(Longitude{"180.0001"}, std::exception);
}

TEST(LongitudeTest, TooSmallDDFromString)
{
    EXPECT_THROW(Longitude{"-180.0001"}, std::exception);
}

TEST(LongitudeTest, BadString)
{
    EXPECT_THROW(Longitude{"abc"}, std::exception);
    EXPECT_THROW(Longitude{"1+1"}, std::exception);
    EXPECT_THROW(Longitude{"..."}, std::exception);
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

TEST(LatitudeTest, ConstructorWorksFromString)
{
    {
        Latitude lon{"10"};
        EXPECT_FLOAT_EQ(lon.DecimalDegrees(), double{10});
    }
    {
        Latitude lon{"-0.02"};
        EXPECT_FLOAT_EQ(lon.DecimalDegrees(), double{-0.02});
    }
}

TEST(LatitudeTest, TooLargeDDFromString)
{
    EXPECT_THROW(Latitude{"90.0001"}, std::exception);
}

TEST(LatitudeTest, TooSmallDDFromString)
{
    EXPECT_THROW(Latitude{"-90.0001"}, std::exception);
}

TEST(LatitudeTest, BadString)
{
    EXPECT_THROW(Latitude{"abc"}, std::exception);
    EXPECT_THROW(Latitude{"1+1"}, std::exception);
    EXPECT_THROW(Latitude{"..."}, std::exception);
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

TEST(DoseRateTest, ConstructorWorksFromString)
{
    DoseRate doseRate{"10.01"};
    EXPECT_FLOAT_EQ(doseRate.MicroSvPerHour(), double{10.01});
}

TEST(DoseRateTest, TooSmallFromString)
{
    EXPECT_THROW(DoseRate{"-0.0001"}, std::exception);
}

TEST(DoseRateTest, NaN)
{
    EXPECT_THROW(DoseRate{NAN}, std::exception);
}

TEST(DoseRateTest, BadString)
{
    EXPECT_THROW(DoseRate{"abc"}, std::exception);
    EXPECT_THROW(DoseRate{"1+1"}, std::exception);
    EXPECT_THROW(DoseRate{"..."}, std::exception);
}
