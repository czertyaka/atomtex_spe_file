#include <stdexcept>
#include <format>
#include <cmath>

#include "atomtex_spe_file/measurement.hpp"

namespace atomtex_spe_file
{

Longitude::Longitude(const double dd) : dd_(dd)
{
    if (dd_ < -180 || dd_ > 180 || std::isnan(dd_))
    {
        const auto error{
            std::format("Invalid decimal degrees for longitude: {}", dd_)};
        throw std::range_error(error);
    }
}

double Longitude::DecimalDegrees() const noexcept
{
    return dd_;
}

Latitude::Latitude(const double dd) : dd_(dd)
{
    if (dd_ < -90 || dd_ > 90 || std::isnan(dd_))
    {
        const auto error{
            std::format("Invalid decimal degrees for latitude: {}", dd_)};
        throw std::range_error(error);
    }
}

double Latitude::DecimalDegrees() const noexcept
{
    return dd_;
}

DoseRate::DoseRate(const double microSvPerHour)
    : microSvPerHour_(microSvPerHour)
{
    if (microSvPerHour_ < 0 || std::isnan(microSvPerHour))
    {
        const auto error{std::format("Invalid dose rate: {}", microSvPerHour_)};
        throw std::range_error(error);
    }
}

double DoseRate::MicroSvPerHour() const noexcept
{
    return microSvPerHour_;
}

} // namespace atomtex_spe_file
