#include "celestial_mechanics/integration/Integrator.hpp"
#include "celestial_mechanics/integration/RHS.hpp"
#include "celestial_mechanics/integration/Tableau.hpp"

#include "celestial_mechanics/time/Time.hpp"
#include "celestial_mechanics/time/TimeConverter.hpp"
#include "celestial_mechanics/time/DutContainer.hpp"
#include "celestial_mechanics/coordinates/EOPContainer.hpp"

#include "celestial_mechanics/forces/EarthGravity.hpp"

#include "celestial_mechanics/orbital_elements/Elements.hpp"
#include "celestial_mechanics/orbital_elements/ElementsConverter.hpp"

#include <Core>

#include <filesystem>

#include <vector>
#include <string>
#include <iostream>


std::filesystem::path projectPath() {
    std::string const filePath = std::string(__FILE__);
    auto const pos = filePath.find("Astana.cpp");
    return std::filesystem::path(filePath.substr(0, pos)).make_preferred();
}
std::filesystem::path resourcesPath() { return projectPath() / "resources";}

struct OrbitType {
    std::string name;
    KeplerianElements initialKeplerian;
    CartesianElements const initialCartesian(double const mu) const {
        return Orbit::convertKeplerianToCartesian(initialKeplerian, mu);
    }
};

int main() {

    std::string const FILE_PATH = __FILE__;
    std::string const DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - 10);

    DutContainer const dutContainer = DutContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");
    EOPContainer const EOPcontainer = EOPContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");

    Time<Scale::TT> const beginTime = Time<Scale::TT>::fromMJD(58777);
    Time<Scale::TT> const endTime = Time<Scale::TT>::fromMJD(58777.2);

    EarthGravity const gravity{resourcesPath() / "gravity", 2, 0};
    typename EarthGravity::SatelliteParameters gravityParameters{};    
    double const mu = gravity.gravitationalParameter();

    double mass = 100;

    KeplerianElements const initialKeplerian{51 * M_PI/180, M_PI/3, 6800e3, 0.5, M_PI/6, M_PI/4};
    CartesianElements const initialCartesian = Orbit::convertKeplerianToCartesian(initialKeplerian, mu);

    Eigen::Vector3d const initialPosition = initialCartesian.position;
    Eigen::Vector3d const initialVelocity = initialCartesian.velocity;

    Satellite<EarthGravity>::State initialState{initialPosition(0), initialPosition(1), initialPosition(2),
                                                initialVelocity(0), initialVelocity(1), initialVelocity(2)};
    Satellite<EarthGravity> rhs{dutContainer, EOPcontainer, gravity, mass, gravityParameters};
    std::vector<typename Satellite<EarthGravity>::StateAndArg> solutionCartesian = integrate<RK4Tableau, Satellite<EarthGravity>>(initialState, beginTime, endTime, 50, rhs);
    std::vector<KeplerianElements> solutionKeplerian;
    unsigned int const n = solutionCartesian.size();
    for (int i = 0; i < n; ++i) {
        Eigen::Vector3d const position{solutionCartesian[i].state(0), solutionCartesian[i].state(1), solutionCartesian[i].state(2)};
        Eigen::Vector3d const velocity{solutionCartesian[i].state(3), solutionCartesian[i].state(4), solutionCartesian[i].state(5)};
        CartesianElements const elementsCartesian{position, velocity};
        KeplerianElements const elementsKeplerian = Orbit::convertCartesianToKeplerian(elementsCartesian, mu);
        solutionKeplerian.push_back(elementsKeplerian);
    }

    std::ofstream file(DIR_PATH + "results/C20_firstOrder/" + "Astana" + ".csv");
    file << "Astana_argumentLatitude,Astana_semimajor,Astana_eccentricity,Astana_argumentPeriapsis,Astana_inclination,Astana_ascendingNode\n";
    for (const auto elements : solutionKeplerian) {
        file << std::setprecision(15) << elements.argumentPeriapsis + elements.trueAnomaly << ',' 
                << elements.semimajor << ','
                << elements.eccentricity << ','
                << elements.argumentPeriapsis << ','
                << elements.inclination << ','
                << elements.ascendingNode << '\n';
    }
    file.close();

    return 0;
}