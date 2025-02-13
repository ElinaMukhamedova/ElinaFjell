#include <numerical_analysis/transport_equation/Grid.hpp>
#include <array>
#include <cmath>
#include <numbers>
#include <vector>


int main() {
    std::array<double, Grid::N_x> u0{};
    for (int i = 0; i < Grid::N_x; ++i) {
        double const x = Grid::h_x * i;
        u0[i] = std::sin(4*M_PI*x/Grid::L);
    }

    std::vector<double> Courant_choice{0.6, 1, 1.01};
    for (auto C : Courant_choice) {
        Grid grid{C, u0};
        
    }
}