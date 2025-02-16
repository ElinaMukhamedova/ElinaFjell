#include "celestial_mechanics/integration/Integrator.hpp"
#include "celestial_mechanics/integration/RHS.hpp"
#include "celestial_mechanics/integration/Tableau.hpp"
#include "celestial_mechanics/time/Time.hpp"
#include "celestial_mechanics/time/TimeConverter.hpp"
#include "celestial_mechanics/time/DutContainer.hpp"
#include "celestial_mechanics/coordinates/EOPContainer.hpp"
#include "celestial_mechanics/forces/EarthGravity.hpp"
#include "celestial_mechanics/forces/AtmosphericDrag.hpp"
#include "celestial_mechanics/forces/SolarRadiationPressure.hpp"
#include "celestial_mechanics/forces/Eternal.hpp"
#include "celestial_mechanics/forces/GOST4401_81.hpp"
#include <filesystem>


std::filesystem::path projectPath() {
    std::string const filePath = std::string(__FILE__);
    auto const pos = filePath.find("orbit_Satellite.cpp");
    return std::filesystem::path(filePath.substr(0, pos)).make_preferred();
}
std::filesystem::path resourcesPath() { return projectPath() / "resources";}

int main() {

    std::string const FILE_PATH = __FILE__;
    std::string const DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - 19);

    double satelliteMass = 1;
    double S_srp = 1;
    double S_drag = 1;
    double C_drag = 1;

    Satellite<EarthGravity, SolarRadiationPressure, AtmosphericDrag>::State const initialState{6800e3, 0, 0, 0, 0, 7656.2};
    Time<Scale::TT> const beginTime = Time<Scale::TT>::fromMJD(58739);
    Time<Scale::TT> const endTime = Time<Scale::TT>::fromMJD(58847);

    DutContainer const dutContainer = DutContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");
    EOPContainer const EOPcontainer = EOPContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");
    EarthGravity const gravity{resourcesPath() / "gravity", 13, 17};
    std::filesystem::path const pathEphemerides = resourcesPath() / "de405.bin";
    Eternal const eternal = Eternal(pathEphemerides.string());
    SolarRadiationPressure const solarRadiationPressure{eternal};
    GOST4401_81 gost;
    AtmosphericDrag atmosphericDrag{gost};

    Satellite<EarthGravity, SolarRadiationPressure, AtmosphericDrag> satelliteRHS{satelliteMass, S_srp, S_drag, C_drag,
                                                                            dutContainer, EOPcontainer,
                                                                            gravity, solarRadiationPressure, atmosphericDrag};
    auto solution = integrate<RK4Tableau, Satellite<EarthGravity, SolarRadiationPressure, AtmosphericDrag>>(initialState, beginTime, endTime, 0.1, satelliteRHS);
}