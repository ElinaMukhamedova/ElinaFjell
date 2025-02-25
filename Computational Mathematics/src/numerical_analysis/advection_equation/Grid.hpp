#pragma once

#include <array>
#include <vector>


///////////////////////////////////////////////////////////////
///    Grid for advection equation:                         ///
///        u.partial_diff(t) + u.partial_diff(x) = 0        ///
///        u(x, 0) = u0(x),     0 <= x <= L                 ///
///        u(0, t) = u(L, t),   0 <= t <= T                 ///
///////////////////////////////////////////////////////////////
struct Grid {
    static constexpr double L = 20;
    static constexpr double T = 18;
    static constexpr double h_x = 0.5;
    static constexpr int N_x = 41;

    double Courant;
    double h_t;

    std::array<double, 41> initialCondition;

    Grid(double const Co, std::array<double, 41> const& u0)
        : Courant(Co), h_t(Co*h_x), initialCondition(u0) {}
    
    std::vector<std::array<double, 41>> solveUpwind() const;
};

