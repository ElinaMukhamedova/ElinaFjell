#include "celestial_mechanics/J2/J2.hpp"
#include "celestial_mechanics/J2/explicitRK.hpp"
#include "celestial_mechanics/J2/BDF.hpp"
#include <Core>
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <iomanip>


int main() {
    auto initialState =typename GravityRHS<CentralAndJ2gravity, 2>::State {6800e3, 0, 0, 0, 0, 7656.2};
    auto endTime = typename GravityRHS<CentralAndJ2gravity, 2>::Argument {150000};
    GravityRHS<CentralAndJ2gravity, 2> gravityRHS;
    auto solutionRK4 = integrateRK<RK4Table, GravityRHS<CentralAndJ2gravity, 2>>(initialState, endTime, 0.1, gravityRHS); 

    IntegrationParameters parameters{0.1, 1e-9, 1000}; 
    auto solutionBDF4 = integrateBDF<BDF4, GravityRHS<CentralAndJ2gravity, 2>, RK4Table>(initialState, endTime, parameters, gravityRHS);
    auto solutionBDF5 = integrateBDF<BDF5, GravityRHS<CentralAndJ2gravity, 2>, RK4Table>(initialState, endTime, parameters, gravityRHS);       

    const std::string FILE_PATH = __FILE__;
    const std::string DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - 12);
    
    std::ofstream fileRK4(DIR_PATH + "results/" + "J2_RK4" + ".csv");
    fileRK4 << "t,r\n";
    for (const auto stateAndTime: solutionRK4) {
        Eigen::Vector3d position = {stateAndTime.state(0), stateAndTime.state(1), stateAndTime.state(2)};
        double r = position.norm();
        fileRK4 << std::setprecision(15) << stateAndTime.arg << ',' << r << '\n';
    }
    fileRK4.close();

    std::ofstream fileBDF4(DIR_PATH + "results/" + "J2_BDF4" + ".csv");
    fileBDF4 << "t,r\n";
    for (const auto stateAndTime: solutionBDF4) {
        Eigen::Vector3d position = {stateAndTime.state(0), stateAndTime.state(1), stateAndTime.state(2)};
        double r = position.norm();
        fileBDF4 << std::setprecision(15) << stateAndTime.arg << ',' << r << '\n';
    }
    fileBDF4.close();

    std::ofstream fileBDF5(DIR_PATH + "results/" + "J2_BDF5" + ".csv");
    fileBDF5 << "t,r\n";
    for (const auto stateAndTime: solutionBDF5) {
        Eigen::Vector3d position = {stateAndTime.state(0), stateAndTime.state(1), stateAndTime.state(2)};
        double r = position.norm();
        fileBDF5 << std::setprecision(15) << stateAndTime.arg << ',' << r << '\n';
    }
    fileBDF5.close();

    return 0;
}