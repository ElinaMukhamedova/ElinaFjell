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

#include <filesystem>

#include <vector>
#include <string>
#include <iostream>


std::filesystem::path projectPath() {
    std::string const filePath = std::string(__FILE__);
    auto const pos = filePath.find("C20_firstOrder.cpp");
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

struct KeplerianElementsAndTime {
    KeplerianElements elements;
    Time<Scale::TT> time;
};

int main() {

    std::string const FILE_PATH = __FILE__;
    std::string const DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - 18);

    DutContainer const dutContainer = DutContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");
    EOPContainer const EOPcontainer = EOPContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");

    Time<Scale::TT> const beginTime = Time<Scale::TT>::fromMJD(58777);
    Time<Scale::TT> const endTime = Time<Scale::TT>::fromMJD(58778);

    EarthGravity const gravity{resourcesPath() / "gravity", 2, 0};
    typename EarthGravity::SatelliteParameters gravityParameters{};    
    double const mu = gravity.gravitationalParameter();

    double mass = 100;

    OrbitType const equatorial_circular{"equatorial", {0, 13, 6800e3, 0, 17, 21}};
    OrbitType const Astana_circular{"Astana", {51, 13, 6800e3, 0, 17, 21}};
    OrbitType const Flåm_circular{"Flåm", {61, 13, 6800e3, 0, 17, 21}};
    OrbitType const polar_circular{"polar", {90, 13, 6800e3, 0, 17, 21}};

    std::vector<OrbitType> circular{equatorial_circular, Astana_circular, Flåm_circular, polar_circular};
    for (OrbitType orbit : circular) {

        CartesianElements const initCartesian = orbit.initialCartesian(mu);
        Eigen::Vector3d const initPosition = initCartesian.position;
        Eigen::Vector3d const initVelocity = initCartesian.velocity;
        
        Satellite<EarthGravity>::State const initialState{initPosition(0), initPosition(1), initPosition(2),
                                                            initVelocity(0), initVelocity(1), initVelocity(2)};
    
        Satellite<EarthGravity> satelliteRHS{dutContainer, EOPcontainer, gravity, mass, gravityParameters};
    
        auto solution = integrate<RK4Tableau, Satellite<EarthGravity>>(initialState, beginTime, endTime, 50, satelliteRHS);
        unsigned int const N = solution.size();
        std::vector<KeplerianElementsAndTime> KeplerianSolution;
        for (int i = 0; i < N; ++i) {
            Eigen::Vector3d const position{solution[i].state(0), solution[i].state(1), solution[i].state(2)};
            Eigen::Vector3d const velocity{solution[i].state(3), solution[i].state(4), solution[i].state(5)};
            Time<Scale::TT> const time = solution[i].arg;
            CartesianElements const Cartesian{position, velocity};
            KeplerianElements const Keplerian = Orbit::convertCartesianToKeplerian(Cartesian, mu);
            KeplerianSolution.push_back(KeplerianElementsAndTime{Keplerian, time});
        }
    
    
    
        std::ofstream file(DIR_PATH + "results/C20_firstOrder/circular/" + orbit.name + ".csv");
        file << "t,semimajor,eccentricity,argumentPeriapsis,inclination,ascendingNode\n";
        for (const auto stateAndTime: KeplerianSolution) {
            file << std::setprecision(15) << stateAndTime.time.mjd() << ',' 
                    << stateAndTime.elements.semimajor << ','
                    << stateAndTime.elements.eccentricity << ','
                    << stateAndTime.elements.argumentPeriapsis << ','
                    << stateAndTime.elements.inclination << ','
                    << stateAndTime.elements.ascendingNode << '\n';
        }
        file.close();
    
    }

    return 0;
}