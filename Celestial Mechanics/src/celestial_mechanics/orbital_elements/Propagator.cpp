#include "Propagator.hpp"
#include "Elements.hpp"
#include "AnomalyConverter.hpp"
#include <cmath>


KeplerianElements propagate(KeplerianElements const& el, double const& mu, double const& deltaT) {
    double const& inclination = el.inclination;
    double const& ascendingNode = el.ascendingNode;
    double const& semimajor = el.semimajor;
    double const& eccentricity = el.eccentricity;
    double const& argumentPeriapsis = el.argumentPeriapsis;

    double const& trueAnomaly0 = el.trueAnomaly;
    double const& meanMotion = std::sqrt(mu) / std::pow(semimajor, 1.5);

    double const& eccentricAnomaly0 = TrueToEccentric(eccentricity, trueAnomaly0);
    double const& meanAnomaly0 = EccentricToMean(eccentricity, eccentricAnomaly0);
    double const& meanAnomaly = meanAnomaly0 + meanMotion * deltaT;
    double const& eccentricAnomaly = MeanToEccentric(eccentricity, meanAnomaly);
    double const& trueAnomaly = EccentricToTrue(eccentricity, eccentricAnomaly);

    return KeplerianElements{inclination, ascendingNode, semimajor, eccentricity,
                                argumentPeriapsis, trueAnomaly};
}