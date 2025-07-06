#pragma once

namespace atomtex_spe_file
{

class Longitude
{
public:
    explicit Longitude(const double dd);
    double DecimalDegrees() const noexcept;

private:
    double dd_;
};

class Latitude
{
public:
    explicit Latitude(const double dd);
    double DecimalDegrees() const noexcept;

private:
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
    double MicroSvPerHour() const noexcept;

private:
    double microSvPerHour_;
};

struct Measurement
{
    Point point;
    DoseRate doseRate;
};

} // namespace atomtex_spe_file
