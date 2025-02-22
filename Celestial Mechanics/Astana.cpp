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
#include <GeographicLib/Geocentric.hpp>

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

double inclination_bracket(double const u, KeplerianElements const& initialKeplerian) {
    return 0.5*std::cos(2*u)+0.5*initialKeplerian.eccentricity*(std::cos(u+initialKeplerian.argumentPeriapsis)+std::cos(3*u-initialKeplerian.argumentPeriapsis)/3);
}

double inclination(double const argumentLatitude, KeplerianElements const& initialKeplerian, double const epsilon, double const mu) {
    double const factor = epsilon/(2*mu*initialKeplerian.focalParameter()*initialKeplerian.focalParameter())*std::sin(2*initialKeplerian.inclination);
    double const bracket = inclination_bracket(argumentLatitude, initialKeplerian) - inclination_bracket(initialKeplerian.argumentLatitude(), initialKeplerian);
    return initialKeplerian.inclination + factor*bracket;
}

double ascendingNode_bracket(double const u, KeplerianElements const& initialKeplerian) {
    return u-0.5*std::sin(2*u)+initialKeplerian.eccentricity*(std::sin(u-initialKeplerian.argumentPeriapsis)-0.5*std::sin(u+initialKeplerian.argumentPeriapsis)-std::sin(3*u-initialKeplerian.argumentPeriapsis)/6);
}

double ascendingNode(double const argumentLatitude, KeplerianElements const& initialKeplerian, double const epsilon, double const mu) {
    double const factor = epsilon/(mu*initialKeplerian.focalParameter()*initialKeplerian.focalParameter())*std::cos(initialKeplerian.inclination);
    double const bracket = ascendingNode_bracket(argumentLatitude, initialKeplerian)-ascendingNode_bracket(initialKeplerian.argumentLatitude(), initialKeplerian);
    return initialKeplerian.ascendingNode-factor*bracket;
}

double D_1(KeplerianElements const& Keplerian) {return -(1+0.25*Keplerian.eccentricity*Keplerian.eccentricity)*(1.5*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination)-1);}
double D_2(KeplerianElements const& Keplerian) {return 0.25*(1+2.75*Keplerian.eccentricity*Keplerian.eccentricity)*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination);}
double D_3(KeplerianElements const& Keplerian) {return (7+4.25*Keplerian.eccentricity*Keplerian.eccentricity)*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination)/12;}
double D_4(KeplerianElements const& Keplerian) {return 1.25*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination);}
double D_5(KeplerianElements const& Keplerian) {return -0.5*(1.5*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination)-1);}
double D_6(KeplerianElements const& Keplerian) {return 0.375*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination);}
double D_7(KeplerianElements const& Keplerian) {return -(1.5*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination)-1)/12;}
double D_8(KeplerianElements const& Keplerian) {return 0.0625*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination);}

double S_0(KeplerianElements const& Keplerian) {return 2-2.5*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination);}
double S_1(KeplerianElements const& Keplerian) {return 1+1.75*Keplerian.eccentricity*Keplerian.eccentricity-std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination)*(1.5+2.125*Keplerian.eccentricity*Keplerian.eccentricity);}
double S_2(KeplerianElements const& Keplerian) {return -0.25*(1-4.25*Keplerian.eccentricity*Keplerian.eccentricity)*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination)-0.5*Keplerian.eccentricity*Keplerian.eccentricity;}
double S_3(KeplerianElements const& Keplerian) {return (7+4.25*Keplerian.eccentricity*Keplerian.eccentricity)*std::sin(Keplerian.inclination)*std::sin(Keplerian.inclination)/12-Keplerian.eccentricity*Keplerian.eccentricity/6;}

double argumentPeriapsis_bracket(double const u, KeplerianElements const& initialKeplerian) {
    double const S0 = S_0(initialKeplerian);
    double const S1 = S_1(initialKeplerian);
    double const S2 = S_2(initialKeplerian);
    double const S3 = S_3(initialKeplerian);
    double const D4 = D_4(initialKeplerian);
    double const D5 = D_5(initialKeplerian);
    double const D6 = D_6(initialKeplerian);
    double const D7 = D_7(initialKeplerian);
    double const D8 = D_8(initialKeplerian);
    return S0*u+(S1*std::sin(u-initialKeplerian.argumentPeriapsis)+S2*std::sin(u+initialKeplerian.argumentPeriapsis)+S3*std::sin(3*u-initialKeplerian.argumentPeriapsis))/initialKeplerian.eccentricity+(D4-0.5)*std::sin(2*u)+D5*std::sin(2*(u-initialKeplerian.argumentPeriapsis))+D6*std::sin(4*u-2*initialKeplerian.argumentPeriapsis)+initialKeplerian.eccentricity*(D7*std::sin(3*(u-initialKeplerian.argumentPeriapsis))+D8*std::sin(u-3*initialKeplerian.argumentPeriapsis)+D8*std::sin(5*u-3*initialKeplerian.argumentPeriapsis));
}

double argumentPeriapsis(double const argumentLatitude, KeplerianElements const& initialKeplerian, double const epsilon, double const mu) {
    double const factor = epsilon/(mu*initialKeplerian.focalParameter()*initialKeplerian.focalParameter());
    double const bracket = argumentPeriapsis_bracket(argumentLatitude, initialKeplerian) - argumentPeriapsis_bracket(initialKeplerian.argumentLatitude(), initialKeplerian);
    return initialKeplerian.argumentPeriapsis+factor*bracket;
}

double eccentricity_bracket(double const u, KeplerianElements const& initialKeplerian) {
    double const D1 = D_1(initialKeplerian);
    double const D2 = D_2(initialKeplerian);
    double const D3 = D_3(initialKeplerian);
    double const D4 = D_4(initialKeplerian);
    double const D5 = D_5(initialKeplerian);
    double const D6 = D_6(initialKeplerian);
    double const D7 = D_7(initialKeplerian);
    double const D8 = D_8(initialKeplerian);
    return D1*std::cos(u-initialKeplerian.argumentPeriapsis)+D2*std::cos(u+initialKeplerian.argumentPeriapsis)+D3*std::cos(3*u-initialKeplerian.argumentPeriapsis)+initialKeplerian.eccentricity*(D4*std::cos(2*u)+D5*std::cos(2*(u-initialKeplerian.argumentPeriapsis))+D6*std::cos(4*u-2*initialKeplerian.argumentPeriapsis))+initialKeplerian.eccentricity*initialKeplerian.eccentricity*(D7*std::cos(3*(u-initialKeplerian.argumentPeriapsis))+D8*std::cos(u-3*initialKeplerian.argumentPeriapsis)+D8*std::cos(5*u-3*initialKeplerian.argumentPeriapsis));
}

double eccentricity(double const argumentLatitude, KeplerianElements const& initialKeplerian, double const epsilon, double const mu) {
    double const factor = epsilon/(mu*initialKeplerian.focalParameter()*initialKeplerian.focalParameter());
    double const bracket = eccentricity_bracket(argumentLatitude, initialKeplerian) - eccentricity_bracket(initialKeplerian.argumentLatitude(), initialKeplerian);
    return initialKeplerian.eccentricity+factor*bracket;
}

double focalParameter_bracket(double const u, KeplerianElements const& initialKeplerian) {
    return std::cos(2*u)+initialKeplerian.eccentricity*(std::cos(u+initialKeplerian.argumentPeriapsis)+std::cos(3*u-initialKeplerian.argumentPeriapsis)/3);
}

double focalParameter(double const argumentLatitude, KeplerianElements const& initialKeplerian, double const epsilon, double const mu) {
    double const factor = epsilon*std::sin(initialKeplerian.inclination)*std::sin(initialKeplerian.inclination)/(mu*initialKeplerian.focalParameter());
    double const bracket = focalParameter_bracket(argumentLatitude, initialKeplerian) - focalParameter_bracket(initialKeplerian.argumentLatitude(), initialKeplerian);
    return initialKeplerian.focalParameter()+factor*bracket;
}

double semimajor(double const argumentLatitude, KeplerianElements const& initialKeplerian, double const epsilon, double const mu) {
    return focalParameter(argumentLatitude, initialKeplerian, epsilon, mu)/(1-initialKeplerian.eccentricity*initialKeplerian.eccentricity)+2*initialKeplerian.eccentricity/((1-initialKeplerian.eccentricity*initialKeplerian.eccentricity)*(1-initialKeplerian.eccentricity*initialKeplerian.eccentricity))*eccentricity(argumentLatitude, initialKeplerian, epsilon, mu);
}

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
    double const J2 = 1.0826e-3;
    GeographicLib::Geocentric const& Earth = GeographicLib::Geocentric::WGS84();
    double const epsilon = 1.5*mu*J2*Earth.EquatorialRadius()*Earth.EquatorialRadius();

    double mass = 100;

    KeplerianElements const initialKeplerian{51 * M_PI/180, M_PI/3, 6800e3, 0.5, M_PI/6, M_PI/4};
    CartesianElements const initialCartesian = Orbit::convertKeplerianToCartesian(initialKeplerian, mu);

    Eigen::Vector3d const initialPosition = initialCartesian.position;
    Eigen::Vector3d const initialVelocity = initialCartesian.velocity;

    Satellite<EarthGravity>::State initialState{initialPosition(0), initialPosition(1), initialPosition(2),
                                                initialVelocity(0), initialVelocity(1), initialVelocity(2)};
    Satellite<EarthGravity> rhs{dutContainer, EOPcontainer, gravity, mass, gravityParameters};
    std::vector<typename Satellite<EarthGravity>::StateAndArg> solutionCartesian = integrate<RK4Tableau, Satellite<EarthGravity>>(initialState, beginTime, endTime, 10, rhs);
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
    file << "Astana_argumentLatitude,Astana_focalParameter,Astana_semimajor,Astana_eccentricity,Astana_argumentPeriapsis,Astana_inclination,Astana_ascendingNode,Astana_analytical_inclination,Astana_analytical_ascendingNode,Astana_analytical_argumentPeriapsis,Astana_analytical_eccentricity,Astana_analytical_focalParameter,Astana_analytical_semimajor\n";
    double previous = 0;
    double current;
    unsigned int k = 0;
    for (const auto elements : solutionKeplerian) {
        current = elements.argumentLatitude() + M_PI*k;
        while (current < previous) {
            current += M_PI;
            ++k;
        }
        previous = current;
        file << std::setprecision(15) << current << ','
                << elements.focalParameter() << ',' 
                << elements.semimajor << ','
                << elements.eccentricity << ','
                << elements.argumentPeriapsis << ','
                << elements.inclination << ','
                << elements.ascendingNode << ','
                << inclination(current, initialKeplerian, epsilon, mu) << ','
                << ascendingNode(current, initialKeplerian, epsilon, mu) << ','
                << argumentPeriapsis(current, initialKeplerian, epsilon, mu) << ','
                << eccentricity(current, initialKeplerian, epsilon, mu) << ','
                << focalParameter(current, initialKeplerian, epsilon, mu) << ','
                << semimajor(current, initialKeplerian, epsilon, mu) << '\n';
    }
    file.close();

    return 0;
}