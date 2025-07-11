#include <stdexcept>
#include <format>
#include <cmath>
#include <concepts>
#include <string_view>
#include <charconv>
#include <algorithm>

#include "atomtex_spe_file/measurement.hpp"

namespace atomtex_spe_file
{

namespace
{

template<std::floating_point T, class F>
    requires std::is_invocable_v<F, T>
class FloatFromString
{
public:
    FloatFromString(std::string_view s, const F& validate) : value_(NAN)
    {
        static std::string_view allowed{".0123456789"};
        const auto allSymbolsAllowed =
            std::ranges::all_of(s.cbegin() + 1, s.end(), [](char c)
                { return allowed.find(c) != std::string_view::npos; });
        if (!allSymbolsAllowed)
        {
            const auto error{std::format(
                "Failed to convert '{}' to floating point type", s)};
            throw std::invalid_argument(error);
        }

        const auto convResult{std::from_chars(s.data(), (s.data() + s.size()),
            value_, std::chars_format::fixed)};
        const auto ec = std::make_error_code(convResult.ec);
        if (ec)
        {
            const auto error{std::format(
                "Failed to convert '{}' to floating point type", s)};
            throw std::invalid_argument(error);
        }
        validate(value_);
    }
    T operator*() const
    {
        return value_;
    }

private:
    T value_;
};

} // namespace

Longitude::Longitude(const double dd) : dd_(dd)
{
    validate(dd_);
}

Longitude::Longitude(std::string_view dd)
{
    const FloatFromString<double, decltype(validate)> ffs{dd, validate};
    dd_ = *ffs;
}

double Longitude::DecimalDegrees() const noexcept
{
    return dd_;
}

// static
void Longitude::validate(const double dd)
{
    if (dd < -180 || dd > 180 || std::isnan(dd))
    {
        const auto error{
            std::format("Invalid decimal degrees for longitude: {}", dd)};
        throw std::range_error(error);
    }
}

Latitude::Latitude(const double dd) : dd_(dd)
{
    validate(dd_);
}

Latitude::Latitude(std::string_view dd)
{
    const FloatFromString<double, decltype(validate)> ffs{dd, validate};
    dd_ = *ffs;
}

double Latitude::DecimalDegrees() const noexcept
{
    return dd_;
}

// static
void Latitude::validate(const double dd)
{
    if (dd < -90 || dd > 90 || std::isnan(dd))
    {
        const auto error{
            std::format("Invalid decimal degrees for latitude: {}", dd)};
        throw std::range_error(error);
    }
}

DoseRate::DoseRate(const double microSvPerHour)
    : microSvPerHour_(microSvPerHour)
{
    validate(microSvPerHour_);
}

DoseRate::DoseRate(std::string_view microSvPerHour)
{
    const FloatFromString<double, decltype(validate)> ffs{microSvPerHour,
        validate};
    microSvPerHour_ = *ffs;
}

double DoseRate::MicroSvPerHour() const noexcept
{
    return microSvPerHour_;
}

// static
void DoseRate::validate(const double microSvPerHour)
{
    if (microSvPerHour < 0 || std::isnan(microSvPerHour))
    {
        const auto error{std::format("Invalid dose rate: {}", microSvPerHour)};
        throw std::range_error(error);
    }
}

} // namespace atomtex_spe_file
