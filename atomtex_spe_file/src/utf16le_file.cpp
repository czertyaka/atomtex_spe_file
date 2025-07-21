#include <fstream>
#include <format>
#include <string_view>
#include <filesystem>
#include <stdexcept>
#include <ios>

#include "utf16le_file.hpp"

namespace atomtex_spe_file
{

Utf16leFile::Utf16leFile(std::istream& input, std::string_view path)
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

    std::ifstream ifs{path, std::ios_base::binary};
    Read(ifs);
}

std::u16string_view Utf16leFile::Content() const
{
    return content_;
}

void Utf16leFile::Read(std::istream& input)
{
    if (!input)
    {
        const auto error{std::format("Can not open {}", path_)};
        throw std::runtime_error(error);
    }

    // check Byte Order Marker (BOM):
    // https://en.wikipedia.org/wiki/UTF-16#Byte-order_encoding_schemes
    std::array<std::byte, 2> _BOM = {};
    if (!input.read(reinterpret_cast<char*>(&_BOM[0]), sizeof(_BOM)))
    {
        const auto error{std::format("File {} does not have BOM", path_)};
        throw std::runtime_error(error);
    }
    else if ((_BOM[0] ^ BOM[0]) != std::byte{} ||
             (_BOM[1] ^ BOM[1]) != std::byte{})
    {
        const auto error{
            std::format("File {} has wrong byte order marker", path_)};
        throw std::runtime_error(error);
    }

    const auto pos{input.tellg()};
    input.seekg(0, std::ios::end);
    const auto size{input.tellg() - pos};
    if (size % 2 != 0)
    {
        const auto error{std::format(
            "File {} is malformed UTF-16 wise, it has {} bytes", path_, size)};
        throw std::runtime_error(error);
    }
    content_.resize(size / 2);
    input.seekg(pos);

    if (!input.read(reinterpret_cast<char*>(&content_[0]), size))
    {
        const auto error{std::format("Failed to read file {}", path_)};
        throw std::runtime_error(error);
    }
}

} // namespace atomtex_spe_file
