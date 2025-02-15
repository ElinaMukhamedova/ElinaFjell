#include <numerical_analysis/transport_equation/Grid.hpp>
#include <array>
#include <cmath>
#include <numbers>
#include <vector>
#include <iostream>


int main() {
    std::array<double, Grid::N_x> u0{};
    for (int i = 0; i < Grid::N_x; ++i) {
        double const x = Grid::h_x * i;
        u0[i] = std::sin(4*M_PI*x/Grid::L);
    }

    std::vector<double> Courant_choice{0.6, 1, 1.01};
    std::vector<double> t_choice{0, 5, 10, 15, 18};
    for (auto C : Courant_choice) {
        std::cout << "Courant number = " << C << '\n';
        Grid grid{C, u0};
        for (auto t : t_choice) {
            std::array<double, 41> solution = grid.solveUpwind(t);
            std::cout << "t = " << t << '\n';
            for (int i = 0; i < 41; ++i) {
                std::cout << solution[i] << ", ";
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
}