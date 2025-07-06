#pragma once

#include <string>
#include <istream>

#include "atomtex_spe_file/measurement.hpp"

namespace atomtex_spe_file
{

class SpeFile
{
public:
    SpeFile(std::istream& input);
    Measurement ReadMeasurement() const;

private:
    std::string content_;
};

} // namespace atomtex_spe_file
