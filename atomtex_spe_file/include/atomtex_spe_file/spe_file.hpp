#pragma once

#include <string>
#include <istream>
#include <filesystem>
#include <string_view>

#include "atomtex_spe_file/measurement.hpp"

namespace atomtex_spe_file
{

class SpeFile
{
public:
    SpeFile(std::istream& input, std::string_view path);
    SpeFile(const std::filesystem::path& path);
    Measurement ReadMeasurement() const;

private:
    void Create(std::istream& input);

    std::string content_;
    std::string path_;
};

} // namespace atomtex_spe_file
