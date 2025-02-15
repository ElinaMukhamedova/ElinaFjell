#include "Grid.hpp"

#include <array>


std::array<double, 41> Grid::solveUpwind(double const t) {
    double currentTime = 0;
    std::array<double, 41> state = initialCondition;
    while (currentTime < t) {
        for (int i = N_x - 1; i > 0; --i) {
            state[i] = (1-Courant)*state[i] + Courant*state[i-1];
        }
        state[0] = state[N_x-1];
        currentTime += h_t;
    }
    return state;
}