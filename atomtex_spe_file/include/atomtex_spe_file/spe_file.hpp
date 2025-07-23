#pragma once

#include <string>
#include <string_view>
#include <optional>

#include "atomtex_spe_file/measurement.hpp"

namespace atomtex_spe_file
{

class SpeFile
{
public:
    SpeFile(std::u16string_view content, std::string_view name);
    Measurement Read() const;

private:
    std::u16string_view content_;
    std::string name_;
    std::optional<Measurement> measurment_;
};

} // namespace atomtex_spe_file
