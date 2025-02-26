#include "Grid.hpp"

#include <array>
#include <vector>

///////////////////////////////////////////////////////////////
///    First order upwind scheme for advection equation:    ///
///        u.partial_diff(t) + u.partial_diff(x) = 0        ///
///        u(x, 0) = u0(x),     0 <= x <= L                 ///
///        u(0, t) = u(L, t),   0 <= t <= T                 ///
///////////////////////////////////////////////////////////////

std::vector<std::array<double, Grid::N_x>> Grid::solveUpwind() const {
    double currentTime = 0;
    std::vector<std::array<double, N_x>> solution{initialCondition};
    std::array<double, N_x> state = initialCondition;
    while (currentTime <= T) {
        currentTime += h_t;
        for (int i = N_x - 1; i > 0; --i) {
            state[i] = (1-Courant)*state[i] + Courant*state[i-1];
        }
        state[0] = state[N_x-1];
        solution.push_back(state);
    }
    return solution;
}