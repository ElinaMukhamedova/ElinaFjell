#include <gtest/gtest.h>
#include "celestial_mechanics/orbital_elements/AnomalyConverter.hpp"
#include <numbers>


TEST(AnomalyConverterTest, TrueToEccentricToTrueWorks) {
    double const origin = std::numbers::pi * 1.5;
    double const transfer = Orbit::TrueToEccentric(0.5, origin);
    double const result = Orbit::EccentricToTrue(0.5, transfer);
    double const origin1 = std::numbers::pi / 6;
    double const transfer1 = Orbit::TrueToEccentric(0, origin1);
    double const result1 = Orbit::EccentricToTrue(0, transfer1);
    ASSERT_NEAR(std::abs(origin-result)/origin, 0, 1e-14);
    ASSERT_NEAR(std::abs(origin1-result1)/origin1, 0, 1e-14);
}

TEST(AnomalyConverterTest, EccentricToTrueToEccentricWorks) {
    double const origin = std::numbers::pi / 3;
    double const transfer = Orbit::EccentricToTrue(0.9, origin);
    double const result = Orbit::TrueToEccentric(0.9, transfer);
    ASSERT_NEAR(std::abs(origin-result)/origin, 0, 1e-14);
}

TEST(AnomalyConverterTest, EccentricToMeanToEccentricWorks) {
    double const origin = std::numbers::pi / 7;
    double const transfer = Orbit::EccentricToMean(0.3, origin);
    double const result = Orbit::MeanToEccentric(0.3, transfer);
    ASSERT_NEAR(std::abs(origin-result)/origin, 0, 1e-14);
}