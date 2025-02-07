#include "AnomalyConverter.hpp"
#include <numbers>
#include <cmath>
#include <limits>
#include "celestial_mechanics/Exception.hpp"


double N(double const& x) {return x>=0 ? x : x + 2*std::numbers::pi;}

double TrueToEccentric(double const& eccentricity, double const& trueAnomaly) {
    return N(std::atan2(std::sin(trueAnomaly)*std::sqrt(1-eccentricity*eccentricity),
                        eccentricity + std::cos(trueAnomaly)));
}

double EccentricToTrue(double const& eccentricity, double const& eccentricAnomaly) {
    return N(std::atan2(std::sin(eccentricAnomaly)*std::sqrt(1-eccentricity*eccentricity),
                        std::cos(eccentricAnomaly) - eccentricity));
}

double EccentricToMean(double const& eccentricity, double const& eccentricAnomaly) {
    return eccentricAnomaly - eccentricity * std::sin(eccentricAnomaly);
}

double MeanToEccentric(double const& eccentricity, double const& meanAnomaly) {
    double const accuracy = 1000 * std::numeric_limits<double>::epsilon();
    int const maxIter = 50;
    bool solutionFound = false;
    int iter = 0;
    double E_prev = meanAnomaly>std::numbers::pi ?
                            meanAnomaly-eccentricity : meanAnomaly+eccentricity;
    double E;
    while (solutionFound == false && iter < maxIter) {
        E = E_prev - (E_prev-eccentricity*std::sin(E_prev)-meanAnomaly)/(1-eccentricity*std::cos(E_prev));
        if (std::abs(E-E_prev) <= accuracy) {solutionFound = true;}
        E_prev = E;
        ++iter;
    }
    if (solutionFound == false) {throw Exception("Kepler solver failed to converge!");}
    return E;
}