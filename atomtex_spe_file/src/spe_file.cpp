/*
 * Relying on line numbers is fragile yet fast.
 */

#include <istream>
#include <stdexcept>
#include <iterator>
#include <cstddef>
#include <span>

#include "atomtex_spe_file/spe_file.hpp"

namespace atomtex_spe_file
{

namespace
{

constexpr std::size_t DOSE_RATE_LINE{3105};
constexpr std::size_t LONGITUDE_LINE{3123};
constexpr std::size_t LATITUDE_LINE{3124};
constexpr std::size_t TOTAL_LINES{3130};

template<class Iterator>
    requires(std::bidirectional_iterator<Iterator>)
class Lines
{
public:
    Lines(Iterator& iter, std::span<std::size_t> numbers);

private:
};

} // namespace

SpeFile::SpeFile(std::istream& input)
{
    if (!input)
    {
        constexpr auto error{"Bad input stream"};
        throw std::invalid_argument(error);
    }

    content_.assign({std::istreambuf_iterator<char>(input), {}});
}

Measurement SpeFile::ReadMeasurement() const
{
    if (content_.empty())
    {
        constexpr auto error{"Empty input"};
        throw std::invalid_argument(error);
    }

    // TODO
    return Measurement{{Latitude{0}, Longitude{0}}, DoseRate{0}};
}

} // namespace atomtex_spe_file
