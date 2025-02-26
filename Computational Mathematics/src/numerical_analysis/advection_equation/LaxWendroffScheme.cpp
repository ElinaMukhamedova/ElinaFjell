#include "Grid.hpp"

#include <array>
#include <vector>

///////////////////////////////////////////////////////////////
///       Lax-Wendroff scheme for advection equation:       ///
///        u.partial_diff(t) + u.partial_diff(x) = 0        ///
///        u(x, 0) = u0(x),     0 <= x <= L                 ///
///        u(0, t) = u(L, t),   0 <= t <= T                 ///
///////////////////////////////////////////////////////////////

std::vector<std::array<double, Grid::N_x>> Grid::solveLaxWendroff() const {
    std::vector<std::array<double, N_x>> solution{initialCondition};
    std::array<double, N_x> previous = initialCondition;
    double currentTime = 0;
    std::array<double, N_x> current{};

    while (currentTime <= T) {
        currentTime += h_t;

        //current[0] = Courant/2*(1+Courant)*previous[N_x-1] + (1-Courant*Courant)*previous[0] - Courant/2*(1-Courant)*previous[1];
        
        for (int i = 1; i < N_x-1; ++i) {
            current[i] = Courant/2*(1+Courant)*previous[i-1] + (1-Courant*Courant)*previous[i] - Courant/2*(1-Courant)*previous[i+1];
        }
        
        current[N_x-1] = Courant/2*(1+Courant)*previous[N_x-2] + (1-Courant*Courant)*previous[N_x-1] - Courant/2*(1-Courant)*previous[0];
        current[0] = current[N_x-1];

        solution.push_back(current);
        previous = current;
    }

    return solution;
}