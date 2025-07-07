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
