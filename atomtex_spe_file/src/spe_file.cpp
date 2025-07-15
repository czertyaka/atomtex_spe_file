/*
 * Relying on line numbers is fragile yet fast.
 */

#include <istream>
#include <stdexcept>
#include <iterator>
#include <cstddef>
#include <format>
#include <string_view>
#include <filesystem>
#include <fstream>

#include "atomtex_spe_file/spe_file.hpp"
#include "lines.hpp"

namespace atomtex_spe_file
{

namespace
{

constexpr std::size_t DOSE_RATE_LINE{3105};
constexpr std::size_t LONGITUDE_LINE{3123};
constexpr std::size_t LATITUDE_LINE{3124};
constexpr std::size_t TOTAL_LINES{3130};

} // namespace

SpeFile::SpeFile(std::istream& input, std::string_view path) : path_(path)
{
    Create(input);
}

SpeFile::SpeFile(const std::filesystem::path& path) : path_(path)
{
    if (!std::filesystem::is_regular_file(path))
    {
        const auto error{std::format("{} is not a file", path_)};
        throw std::invalid_argument(error);
    }

    std::ifstream ifs{path};
    Create(ifs);
}

Measurement SpeFile::ReadMeasurement() const
{
    if (content_.empty())
    {
        const auto error{std::format("{} is empty", path_)};
        throw std::invalid_argument(error);
    }

    using Iterator = decltype(content_)::const_reverse_iterator;
    Lines<Iterator> lines{content_.crbegin(), content_.crend()};

    auto readLine = [&lines, this](const std::size_t number)
    {
        const std::size_t offset = TOTAL_LINES - number;
        const auto line = lines[number];
        if (!line)
        {
            const auto error{std::format("SPE file {} does not have {} line",
                path_, number)};
            throw std::runtime_error(error);
        }
        return *line;
    };

    return Measurement{Latitude{readLine(LATITUDE_LINE)},
        Longitude{readLine(LONGITUDE_LINE)},
        DoseRate{readLine(DOSE_RATE_LINE)}};
}

void SpeFile::Create(std::istream& input)
{
    if (!input)
    {
        const auto error{std::format("Can not open {}", path_)};
        throw std::invalid_argument(error);
    }

    content_.assign({std::istreambuf_iterator<char>(input), {}});
}

} // namespace atomtex_spe_file
