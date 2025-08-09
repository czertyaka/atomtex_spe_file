#pragma once

#include <string>
#include <string_view>
#include <filesystem>
#include <istream>
#include <array>
#include <cstddef>

namespace atomtex_spe_file
{

class Utf16leFile
{
public:
    constexpr static std::array<std::byte, 2> BOM{std::byte{0xFF},
        std::byte{0xFE}};

    Utf16leFile(std::istream& input, std::string_view path);
    Utf16leFile(const std::filesystem::path& path);

    std::u16string_view Content() const;

private:
    void Read(std::istream& input);

    std::u16string content_;
    std::string path_;
};

} // namespace atomtex_spe_file
