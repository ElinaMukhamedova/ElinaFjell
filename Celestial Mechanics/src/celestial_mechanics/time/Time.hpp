#pragma once

#include <compare>
#include <vector>
#include <cmath>
#include <sofa.h>
#include "celestial_mechanics/Exception.hpp"

enum class Scale{UTC = 0, UT1 = 1, TAI = 2, TT = 3, TCG = 4, TDB = 5};

template <Scale scale>
class Time {
    double jdInt_;
    double jdFrac_;

    public:
        Time(double jd1 = 0, double jd2 = 0) noexcept;

        Time static fromJD(double jd) noexcept;
        Time static fromMJD(double mjd) noexcept;

        Time static fromCalendar(int year, int month, int day, int hours, int minutes, double seconds);
        
        double jdInt() const noexcept;
        double jdFrac() const noexcept;
        double jd() const noexcept;
        double mjd() const noexcept;

        auto operator<=>(const Time& other) const noexcept = default;
};

template <Scale scale>
Time<scale>::Time(double jd1, double jd2) noexcept{
    double jd1_int;
    double jd1_frac = std::modf(jd1, &jd1_int);
    double jd2_int;
    double jd2_frac = std::modf(jd2, &jd2_int);
    jdFrac_ = std::modf(jd1_frac + jd2_frac, &jdInt_);
    jdInt_ += jd1_int + jd2_int;
    if (jdFrac_ > 0.5) {
        jdFrac_ -= 1;
        jdInt_ += 1;
    }
    if (jdFrac_ <= -0.5) {
        jdFrac_ += 1;
        jdInt_ -= 1;
    }
}

template <Scale scale>
Time<scale> Time<scale>::fromJD(double jd) noexcept {
    double jd_int;
    double jd_frac = std::modf(jd, &jd_int);
    return Time<scale>(jd_int, jd_frac);
}

template <Scale scale>
Time<scale> Time<scale>::fromMJD(double mjd) noexcept {
    double mjd_int;
    double mjd_frac = std::modf(mjd, &mjd_int);
    return Time<scale>(mjd_int + 2400000, mjd_frac + .5);
}

template <Scale scale>
Time<scale> Time<scale>::fromCalendar(int year, int month, int day, int hours, int minutes, double seconds) {
    double djm0;
    double djm;
    int result_iauCal2jd = iauCal2jd(year, month, day, &djm0, &djm);
    switch (result_iauCal2jd) {
        case -1:
            throw Exception("bad year (JD not computed)");
        case -2:
            throw Exception("bad month (JD not computed)");
        case -3:
            throw Exception("bad day (JD not computed)");
    }
    double days;
    int result_iauTf2d = iauTf2d('+', hours, minutes, seconds, &days);
    switch (result_iauTf2d) {
        case 1:
            throw Exception("hours outside range 0-23 (JD not computed)");
        case 2:
            throw Exception("minutes outside range 0-59 (JD not computed)");
        case 3:
            throw Exception("seconds outside range 0-59.999... (JD not computed)");
        default:
            return djm0 + djm + days;
    }
}

template <Scale scale>
double Time<scale>::jdInt() const noexcept {
    return jdInt_;
}

template <Scale scale>
double Time<scale>::jdFrac() const noexcept {
    return jdFrac_;
}

template <Scale scale>
double Time<scale>::jd() const noexcept {
    return jdInt_ + jdFrac_;
}

template <Scale scale>
double Time<scale>::mjd() const noexcept {
    double mjd_int = jdInt_ - 2400000;
    double mjd_frac = jdFrac_ - .5;
    return mjd_int + mjd_frac;
}

template<Scale scale>
double operator-(const Time<scale>& first, const Time<scale>& second) noexcept {
    double const delta_int = first.jdInt() - second.jdInt();
    double const delta_frac = first.jdFrac() - second.jdFrac();
    double const res1 = delta_int * 86400;
    double const res2 = delta_frac * 86400;
    return res1 + res2;
}

template<Scale scale>
Time<scale> operator-(const Time<scale>& point, double delta) noexcept {
    return Time<scale>::fromMJD(point.mjd() - delta / 86400);
}

template<Scale scale>
Time<scale> operator+(const Time<scale>& point, double delta) noexcept {
    double delta_int;
    double delta_frac = std::modf(delta / 86400, &delta_int);
    return Time<scale>(point.jdInt() + delta_int, point.jdFrac() + delta_frac);
}