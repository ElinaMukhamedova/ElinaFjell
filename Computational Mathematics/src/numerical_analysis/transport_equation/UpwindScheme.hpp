#pragma once

#include <array>


///////////////////////////////////////////////////////////////
///    First order upwind scheme for transport equation:    ///
///        u.partial_diff(t) + u.partial_diff(x) = 0        ///
///////////////////////////////////////////////////////////////
struct Grid {
    static constexpr double L = 20;
    static constexpr double T = 18;
    static constexpr double h_x = 0.5;
    static constexpr int N_x = 41;

    double Courant;
    double h_t;

    std::array<double, 41> initialCondition;

    Grid(double const& C, std::array<double, 41> const& u0)
        : Courant(C), h_t(C*h_x), initialCondition(u0) {}
};