#include "celestial_mechanics/integration/Integrator.hpp"
#include "celestial_mechanics/integration/RHS.hpp"
#include "celestial_mechanics/integration/Tableau.hpp"
#include "celestial_mechanics/time/Time.hpp"
#include "celestial_mechanics/time/TimeConverter.hpp"
#include "celestial_mechanics/time/DutContainer.hpp"
#include "celestial_mechanics/coordinates/EOPContainer.hpp"
#include <filesystem>


std::filesystem::path projectPath() {
    std::string const filePath = std::string(__FILE__);
    auto const pos = filePath.find("orbit_EarthGravity.cpp");
    return std::filesystem::path(filePath.substr(0, pos)).make_preferred();
}
std::filesystem::path resourcesPath() { return projectPath() / "resources";}

int main() {

    std::string const FILE_PATH = __FILE__;
    std::string const DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - 22);

    double satelliteMass = 10;

    Satellite<EarthGravity>::State const initialState{6800e3, 0, 0, 0, 0, 7656.2};
    Time<Scale::TT> const beginTime = Time<Scale::TT>::fromMJD(58777);
    Time<Scale::TT> const endTime = Time<Scale::TT>::fromMJD(58778);

    DutContainer const dutContainer = DutContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");
    EOPContainer const EOPcontainer = EOPContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");
    EarthGravity const gravity{resourcesPath() / "gravity", 2, 2};
    typename EarthGravity::SatelliteParameters gravityParameters{};

    Satellite<EarthGravity> satelliteRHS{dutContainer, EOPcontainer, gravity, satelliteMass, gravityParameters};
    
    auto solution = integrate<RK4Tableau, Satellite<EarthGravity>>(initialState, beginTime, endTime, 50, satelliteRHS);

    std::ofstream fileRK4(DIR_PATH + "results/" + "gravity" + ".csv");
    fileRK4 << "t,x,y,z,r\n";
    for (const auto stateAndTime: solution) {
        Eigen::Vector3d position = {stateAndTime.state(0), stateAndTime.state(1), stateAndTime.state(2)};
        double r = position.norm();
        fileRK4 << std::setprecision(15) << stateAndTime.arg.mjd() << ',' 
                << position(0) << ',' << position(1) << ',' << position(2) << ','
                << r << '\n';
    }
    fileRK4.close();

    return 0;
}