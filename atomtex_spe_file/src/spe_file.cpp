/*
 * Relying on line numbers is fragile yet fast.
 */

#include <stdexcept>
#include <cstddef>
#include <format>
#include <string_view>
#include <ranges>

#include "atomtex_spe_file/spe_file.hpp"
#include "lines.hpp"

namespace atomtex_spe_file
{

namespace
{

constexpr std::size_t DOSE_RATE_LINE{3105};
constexpr std::size_t LONGITUDE_LINE{3123};
constexpr std::size_t LATITUDE_LINE{3124};
constexpr std::size_t TOTAL_LINES{3131};

} // namespace

SpeFile::SpeFile(std::u16string_view content, std::string_view name)
    : content_(content), name_(name)
{
}

Measurement SpeFile::Read() const
{
    if (measurment_)
    {
        return *measurment_;
    }

    if (content_.empty())
    {
        const auto error{std::format("SPE file {} is empty", name_)};
        throw std::invalid_argument(error);
    }

    using Iterator = decltype(content_)::const_reverse_iterator;
    Lines<Iterator> lines{content_.crbegin(), content_.crend()};

    // we already know that lines we want are closer to the file end
    // hence, rv::reverse
    auto readLine = [&lines, this](const std::size_t number)
    {
        const std::size_t offset = TOTAL_LINES - number;
        const auto line = lines[offset];
        if (!line)
        {
            const auto error{std::format("SPE file {} does not have {} line",
                name_, number)};
            throw std::runtime_error(error);
        }
        return *line;
    };

    auto utf16leToASCII = [this](auto&& character)
    {
        if (char16_t{0xFF00} & character)
        {
            const auto error{std::format(
                "SPE file {} contains invalid ASCII character", name_)};
            throw std::runtime_error(error);
        }
        return static_cast<char>(character);
    };

    const auto latStr = std::string(std::from_range_t{},
        readLine(LATITUDE_LINE) | std::views::transform(utf16leToASCII));
    const Latitude lat{latStr};

    const auto lonStr = std::string(std::from_range_t{},
        readLine(LONGITUDE_LINE) | std::views::transform(utf16leToASCII));
    const Longitude lon{lonStr};

    const auto doseRateStr = std::string(std::from_range_t{},
        readLine(DOSE_RATE_LINE) | std::views::transform(utf16leToASCII));
    const DoseRate doseRate{doseRateStr};

    return Measurement{Point{lat, lon}, doseRate};
}

} // namespace atomtex_spe_file
