#pragma once

#include <string_view>

namespace atomtex_spe_file
{

class Longitude
{
public:
    explicit Longitude(const double dd);
    explicit Longitude(std::string_view dd);
    double DecimalDegrees() const noexcept;

private:
    static void validate(const double dd);
    double dd_;
};

class Latitude
{
public:
    explicit Latitude(const double dd);
    explicit Latitude(std::string_view dd);
    double DecimalDegrees() const noexcept;

private:
    static void validate(const double dd);
    double dd_;
};

struct Point
{
    Latitude lat;
    Longitude lon;
};

class DoseRate
{
public:
    explicit DoseRate(const double microSvPerHour);
    explicit DoseRate(std::string_view microSvPerHour);
    double MicroSvPerHour() const noexcept;

private:
    static void validate(const double microSvPerHour);
    double microSvPerHour_;
};

struct Measurement
{
    Point point;
    DoseRate doseRate;
};

} // namespace atomtex_spe_file
