#include <gtest/gtest.h>
#include "celestial_mechanics/orbital_elements/Propagator.hpp"
#include "celestial_mechanics/orbital_elements/Elements.hpp"
#include "celestial_mechanics/orbital_elements/AnomalyConverter.hpp"
#include <numbers>
#include <iostream>


TEST(PropagatorTest, PropagatorWorks) {
    KeplerianElements const start{std::numbers::pi/3, std::numbers::pi/6, 6378137, 0.9, std::numbers::pi/5, std::numbers::pi/4};
    KeplerianElements const finish = Orbit::propagate(start, 986e14, 5);
    std::cout << start.trueAnomaly << ' ' << finish.trueAnomaly << '\n';
    double const& meanMotion = std::sqrt(986e14) / std::pow(6378137, 1.5);
    double const& M1 = Orbit::EccentricToMean(start.eccentricity, Orbit::TrueToEccentric(start.eccentricity, start.trueAnomaly));
    double const& M2 = Orbit::EccentricToMean(finish.eccentricity, Orbit::TrueToEccentric(finish.eccentricity, finish.trueAnomaly));
    ASSERT_NEAR(std::abs(start.inclination-finish.inclination)/start.inclination, 0, 1e-14);
    ASSERT_NEAR(std::abs(start.ascendingNode-finish.ascendingNode)/start.ascendingNode, 0, 1e-14);
    ASSERT_NEAR(std::abs(start.semimajor-finish.semimajor)/start.semimajor, 0, 1e-14);
    ASSERT_NEAR(std::abs(start.eccentricity-finish.eccentricity)/start.eccentricity, 0, 1e-14);
    ASSERT_NEAR(std::abs(start.argumentPeriapsis-finish.argumentPeriapsis)/start.argumentPeriapsis, 0, 1e-14);
    ASSERT_NEAR(std::abs(M2-M1)/5 - meanMotion, 0, 1e-14);
}