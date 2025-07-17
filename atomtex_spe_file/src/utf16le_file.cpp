#include <fstream>
#include <format>
#include <string_view>
#include <filesystem>
#include <stdexcept>
#include <ios>
#include <cstdint>
#include <vector>
#include <functional>

#include "utf16le_file.hpp"

namespace atomtex_spe_file
{

Utf16leFile::Utf16leFile(Utf16leFile::InputStream& input, std::string_view path)
    : path_(path)
{
    Read(input);
}

Utf16leFile::Utf16leFile(const std::filesystem::path& path) : path_(path)
{
    if (!std::filesystem::is_regular_file(path))
    {
        const auto error{std::format("{} is not a file", path_)};
        throw std::invalid_argument(error);
    }

    std::basic_ifstream<std::uint8_t> ifs{path, std::ios_base::binary};
    Read(ifs);
}

std::u16string_view Utf16leFile::Content() const
{
    return content_;
}

void Utf16leFile::Read(Utf16leFile::InputStream& input)
{
    if (!input)
    {
        const auto error{std::format("Can not open {}", path_)};
        throw std::runtime_error(error);
    }

    std::istreambuf_iterator<std::uint8_t> it{input};

    // check BOM:
    // https://en.wikipedia.org/wiki/UTF-16#Byte-order_encoding_schemes
    if (*(it++) != 0xFF || *(it++) != 0xFE)
    {
        const auto error{
            std::format("File {} misses BOM or does not have one", path_)};
        throw std::runtime_error(error);
    }

    std::vector<std::uint8_t> buffer(it, {});
    if (buffer.size() % 2 != 0)
    {
        const auto error{std::format("File {} is malformed", path_)};
    }

    content_.reserve(buffer.size() / 2);
    for (std::size_t i{0}; i < buffer.size(); i += 2)
    {
        using CodeUnit = decltype(content_)::value_type;
        const auto codeUnit{std::bit_or<CodeUnit>{}(CodeUnit{buffer[i]},
            CodeUnit{buffer[i + 1]} << 8)};
        content_.push_back(codeUnit);
    }
}

} // namespace atomtex_spe_file
