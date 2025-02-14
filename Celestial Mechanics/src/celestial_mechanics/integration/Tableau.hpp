#pragma once

#include <array>


struct RK4Tableau {
    static constexpr unsigned int stages = 4;
    static constexpr std::array<std::array<double, stages>, stages>
        aTable = {{ {0, 0, 0, 0}, {0.5, 0, 0, 0}, {0, 0.5, 0, 0}, {0, 0, 1, 0} }};
    static constexpr std::array<double, stages> cColumn = {0, 0.5, 0.5, 1};
    static constexpr std::array<double, stages> bString = {(double)1/6, (double)1/3, (double)1/3, (double)1/6};
};