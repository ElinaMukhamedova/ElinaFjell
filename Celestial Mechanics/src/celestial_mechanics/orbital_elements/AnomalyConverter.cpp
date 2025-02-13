#include "AnomalyConverter.hpp"
#include <numbers>
#include <cmath>
#include <limits>
#include "celestial_mechanics/Exception.hpp"


double N(double const x) {return x>=0 ? x : x + 2*std::numbers::pi;}

double normAngle(double const x) {return fmod(x, 2*M_PI);}

namespace Orbit{

double TrueToEccentric(double const eccentricity, double const trueAnomaly) {
    return N(std::atan2(std::sin(trueAnomaly)*std::sqrt(1-eccentricity*eccentricity),
                        eccentricity + std::cos(trueAnomaly)));
}

double EccentricToTrue(double const eccentricity, double const eccentricAnomaly) {
    return N(std::atan2(std::sin(eccentricAnomaly)*std::sqrt(1-eccentricity*eccentricity),
                        std::cos(eccentricAnomaly) - eccentricity));
}

double EccentricToMean(double const eccentricity, double const eccentricAnomaly) {
    return normAngle(eccentricAnomaly - eccentricity * std::sin(eccentricAnomaly));
}

double MeanToEccentric(double const eccentricity, double meanAnomaly) {
    meanAnomaly = normAngle(meanAnomaly);
    double const accuracy = 1000 * std::numeric_limits<double>::epsilon();
    unsigned int const maxIter = 50;
    double E = meanAnomaly>std::numbers::pi ?
                            meanAnomaly-eccentricity : meanAnomaly+eccentricity;
    for (unsigned int iter = 0; iter < maxIter; ++iter) {
        const double dE = - (E-eccentricity*std::sin(E)-meanAnomaly)/(1-eccentricity*std::cos(E));
        E = E + dE;
        if (std::abs(dE) <= accuracy) {return E;}
    }
    throw Exception("Kepler solver failed to converge!");
}

};