#pragma once

#include <string>
#include <string_view>
#include <filesystem>
#include <istream>
#include <cstdint>

namespace atomtex_spe_file
{

class Utf16leFile
{
public:
    using InputStream = std::basic_istream<std::uint8_t>;

    Utf16leFile(InputStream& input, std::string_view path);
    Utf16leFile(const std::filesystem::path& path);

    std::u16string_view Content() const;

private:
    void Read(InputStream& input);

    std::u16string content_;
    std::string path_;
};

} // namespace atomtex_spe_file
