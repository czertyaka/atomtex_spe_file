/*
 * Relying on line numbers is fragile yet fast.
 */

#include <stdexcept>
#include <cstddef>
#include <format>
#include <string_view>
#include <ranges>
#include <cctype>

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

constexpr std::string_view LONGITUDE_PREFIX{"Lon= "};
constexpr std::string_view LATITUDE_PREFIX{"Lat= "};

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

    // we already know that lines we want are closer to the file end
    // hence, reverse iterator
    using Iterator = decltype(content_)::const_reverse_iterator;
    Lines<Iterator> lines{content_.crbegin(), content_.crend()};

    // *.spe files UTF-16 LE encoded for **no** reason
    // if it does contain non-ASCII character it is then malformed
    auto utf16leToASCII = [this](auto&& character) -> char
    {
        if (char16_t{0xFF00} & character)
        {
            const auto error{std::format(
                "SPE file {} contains invalid ASCII character", name_)};
            throw std::runtime_error(error);
        }
        return static_cast<char>(character);
    };

    auto printable = [](const int c) -> bool { return std::isprint(c); };

    const auto latStr =
        lines[TOTAL_LINES - LATITUDE_LINE] |
        std::views::transform(utf16leToASCII) |
        std::views::drop(LATITUDE_PREFIX.length()) | // eat up prefix
        std::views::take_while(printable) | std::ranges::to<std::string>();
    const Latitude lat{latStr};

    const auto lonStr =
        lines[TOTAL_LINES - LONGITUDE_LINE] |
        std::views::transform(utf16leToASCII) |
        std::views::drop(LONGITUDE_PREFIX.length()) | // eat up prefix
        std::views::take_while(printable) | std::ranges::to<std::string>();
    const Longitude lon{lonStr};

    const auto doseRateStr = lines[TOTAL_LINES - DOSE_RATE_LINE] |
                             std::views::transform(utf16leToASCII) |
                             std::views::take_while(printable) |
                             std::ranges::to<std::string>();
    const DoseRate doseRate{doseRateStr};

    return Measurement{Point{lat, lon}, doseRate};
}

} // namespace atomtex_spe_file
