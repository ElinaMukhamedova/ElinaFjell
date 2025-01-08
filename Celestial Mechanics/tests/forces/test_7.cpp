#include <gtest/gtest.h>
#include "celestial_mechanics/forces/Eternal.hpp"
#include "tests/Paths.hpp"
#include <filesystem>
#include <string>
#include "celestial_mechanics/time/Time.hpp"

class SolarRadiationPressureTest : public testing::Test {
    protected:
        std::filesystem::path pathEphemerides = resourcesPath() / "de405.bin";
        std::string path = pathEphemerides.string();
        Eternal eternal = Eternal(path);
        double AU = eternal.AU();
};

TEST_F(SolarRadiationPressureTest, AUvalueKM) {
    ASSERT_NEAR(AU, 149597870.7, 1e-2);
}

TEST_F(SolarRadiationPressureTest, EvigWorks) {

    Time<Scale::TDB> tdb{2442457, 0.5};

    Eigen::Vector3d EarthMoon = eternal.vector(tdb, CelestialBody::Earth, CelestialBody::Moon);
    ASSERT_EQ(EarthMoon(0), 2.6915796317492878E-03 * AU * 1000);
    ASSERT_EQ(EarthMoon(1), -2.9700174658236497E-04 * AU * 1000);
    ASSERT_EQ(EarthMoon(2), 1.1595435241138705E-04 * AU * 1000);

    Eigen::Vector3d SunMars = eternal.vector(tdb, CelestialBody::Sun, CelestialBody::Mars);
    ASSERT_EQ(SunMars(0), -1.7171930091732712E-01 * AU * 1000);
    ASSERT_EQ(SunMars(1), -1.3283912525999000E+00 * AU * 1000);
    ASSERT_EQ(SunMars(2), -6.0462731092619948E-01 * AU * 1000);
}