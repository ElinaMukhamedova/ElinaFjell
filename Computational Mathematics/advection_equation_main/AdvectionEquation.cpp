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

    std::vector<double> Courant_choice{0.6, 1, 1.1};
    unsigned int const N_Co = Courant_choice.size();

    for (int i = 0; i < N_Co; ++i) {
        double const Co = Courant_choice[i];
        Grid const grid{Co, u0};

        auto const solutionUpwind = grid.solveUpwind();
        unsigned int const n_upwind = solutionUpwind.size();

        std::ofstream upwind_file(DIR_PATH+"resources_for_animation/upwind_scheme/"+std::to_string(i+1)+".csv");
        //file << header << '\n';
        for (int i = 0; i < n_upwind; ++i) {
            auto const state = solutionUpwind[i];
            upwind_file << std::setprecision(15) << i*grid.h_t;
            for (int j = 0; j < Grid::N_x; ++j) {
                upwind_file << std::setprecision(15) << ',' << state[j];
            }
            upwind_file << '\n';
        }
        upwind_file.close();

        std::ofstream upwind_file1(DIR_PATH+"resources_for_animation/upwind_scheme/"+std::to_string(i+1)+"_withoutTime.csv");
        for (int i = 0; i < n_upwind; ++i) {
            auto const state = solutionUpwind[i];
            upwind_file1 << std::setprecision(15) << state[0];
            for (int j = 1; j < Grid::N_x; ++j) {
                upwind_file1 << std::setprecision(15) << ',' << state[j];
            }
            upwind_file1 << '\n';
        }
        upwind_file1.close();

        auto const solutionLaxWendroff = grid.solveLaxWendroff();
        unsigned int const n_LaxWendroff = solutionLaxWendroff.size();

        std::ofstream LaxWendroff_file(DIR_PATH+"resources_for_animation/LaxWendroff_scheme/"+std::to_string(i+1)+".csv");
        for (int i = 0; i < n_LaxWendroff; ++i) {
            auto const state = solutionLaxWendroff[i];
            LaxWendroff_file << std::setprecision(15) << i*grid.h_t;
            for (int j = 0; j < Grid::N_x; ++j) {
                LaxWendroff_file << std::setprecision(15) << ',' << state[j];
            }
            LaxWendroff_file << '\n';
        }
        LaxWendroff_file.close();

        std::ofstream LaxWendroff_file1(DIR_PATH+"resources_for_animation/LaxWendroff_scheme/"+std::to_string(i+1)+"_withoutTime.csv");
        for (int i = 0; i < n_LaxWendroff; ++i) {
            auto const state = solutionLaxWendroff[i];
            LaxWendroff_file1 << std::setprecision(15) << state[0];
            for (int j = 1; j < Grid::N_x; ++j) {
                LaxWendroff_file1 << std::setprecision(15) << ',' << state[j];
            }
            LaxWendroff_file1 << '\n';
        }
        LaxWendroff_file1.close();
    }
}