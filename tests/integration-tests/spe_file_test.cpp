#include <gtest/gtest.h>

#include "atomtex_spe_file/spe_file.hpp"
#include "atomtex_spe_file/utf16le_file.hpp"
#include "atomtex_spe_file/measurement.hpp"


TEST(SpeFileTest, ReadExample)
{
    using namespace atomtex_spe_file;

    const auto path{"example.spe"};
    const Utf16leFile file{path};
    const SpeFile spe{file.Content(), path};
    const auto measurement{spe.Read()};

    EXPECT_EQ(measurement.doseRate.MicroSvPerHour(), 13.275690);
    EXPECT_EQ(measurement.point.lat.DecimalDegrees(), -45.959225);
    EXPECT_EQ(measurement.point.lon.DecimalDegrees(), 31.060719);
}
