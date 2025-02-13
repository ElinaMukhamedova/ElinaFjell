#include <gtest/gtest.h>
#include "celestial_mechanics/orbital_elements/Elements.hpp"
#include "celestial_mechanics/orbital_elements/ElementsConverter.hpp"
#include <Core>
#include <numbers>


TEST(ElementsConverterTest, KeplerianToCartesianToKeplerianWorks) {
    KeplerianElements const origin{std::numbers::pi/3, std::numbers::pi/6, 6378137, 0.9, std::numbers::pi/5, std::numbers::pi/4};
    CartesianElements const transfer = Orbit::convertKeplerianToCartesian(origin, 3.986e14);
    KeplerianElements const result = Orbit::convertCartesianToKeplerian(transfer, 3.986e14);
    ASSERT_NEAR(std::abs(origin.inclination-result.inclination)/origin.inclination, 0, 1e-14);
    ASSERT_NEAR(std::abs(origin.inclination-result.inclination)/origin.inclination, 0, 1e-14);
    ASSERT_NEAR(std::abs(origin.ascendingNode-result.ascendingNode)/origin.ascendingNode, 0, 1e-14);
    ASSERT_NEAR(std::abs(origin.semimajor-result.semimajor)/origin.semimajor, 0, 1e-14);
    ASSERT_NEAR(std::abs(origin.eccentricity-result.eccentricity)/origin.eccentricity, 0, 1e-14);
    ASSERT_NEAR(std::abs(origin.argumentPeriapsis-result.argumentPeriapsis)/origin.argumentPeriapsis, 0, 1e-14);
    ASSERT_NEAR(std::abs(origin.trueAnomaly-result.trueAnomaly)/origin.trueAnomaly, 0, 1e-14);
}

TEST(ElementsConverterTest, CartesianToKeplerianToCartesianWorks) {
    Eigen::Vector3d position{6400000, 6500000, 6600000};
    Eigen::Vector3d velocity{100, -5000, -1000};
    CartesianElements const origin{position, velocity};
    KeplerianElements const transfer = Orbit::convertCartesianToKeplerian(origin, 3.986e14);
    CartesianElements const result = Orbit::convertKeplerianToCartesian(transfer, 3.986e14);
    ASSERT_NEAR(std::abs(origin.position(0)-result.position(0))/origin.position(0), 0, 1e-13);  
    ASSERT_NEAR(std::abs(origin.position(1)-result.position(1))/origin.position(1), 0, 1e-13);
    ASSERT_NEAR(std::abs(origin.position(2)-result.position(2))/origin.position(2), 0, 1e-13);
    ASSERT_NEAR(std::abs(origin.velocity(0)-result.velocity(0))/origin.velocity(0), 0, 1e-13);
    ASSERT_NEAR(std::abs(origin.velocity(1)-result.velocity(1))/origin.velocity(1), 0, 1e-13);
    ASSERT_NEAR(std::abs(origin.velocity(2)-result.velocity(2))/origin.velocity(2), 0, 1e-13);
}