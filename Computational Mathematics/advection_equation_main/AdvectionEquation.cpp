#include <numerical_analysis/advection_equation/Grid.hpp>
#include <array>
#include <cmath>
#include <numbers>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>


int main() {

    std::string const FILE_PATH = __FILE__;
    std::string const DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - 21);
    
    std::array<double, Grid::N_x> u0{};
    for (int i = 0; i < Grid::N_x; ++i) {
        double const x = Grid::h_x * i;
        u0[i] = std::sin(4*M_PI*x/Grid::L);
    }

    std::string header{"t"};
    for (int i = 0; i < Grid::N_x; ++i) {
        header += ",u("+std::to_string(i*Grid::h_x)+",t)";
    }

    std::vector<double> Courant_choice{0.6, 1, 1.01};
    unsigned int const N_Co = Courant_choice.size();

    for (int i = 0; i < N_Co; ++i) {
        double const Co = Courant_choice[i];
        Grid grid{Co, u0};
        auto solution = grid.solveUpwind();
        unsigned int n = solution.size();

        std::ofstream file(DIR_PATH+"resources_for_animation/upwind_scheme/"+std::to_string(i+1)+".csv");
        //file << header << '\n';
        for (int i = 0; i < n; ++i) {
            auto state = solution[i];
            file << std::setprecision(15) << i*grid.h_t;
            for (int j = 0; j < Grid::N_x; ++j) {
                file << std::setprecision(15) << ',' << state[j];
            }
            file << '\n';
        }
        file.close();

        std::ofstream file1(DIR_PATH+"resources_for_animation/upwind_scheme/"+std::to_string(i+1)+"_withoutTime.csv");
        for (int i = 0; i < n; ++i) {
            auto state = solution[i];
            file1 << std::setprecision(15) << state[0];
            for (int j = 1; j < Grid::N_x; ++j) {
                file1 << std::setprecision(15) << ',' << state[j];
            }
            file1 << '\n';
        }
        file1.close();
    }
}