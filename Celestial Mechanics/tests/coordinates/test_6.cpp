#include <gtest/gtest.h>
#include "celestial_mechanics/coordinates/ReferenceSystemConverter.hpp"
#include "celestial_mechanics/coordinates/EOPContainer.hpp"
#include "celestial_mechanics/time/Time.hpp"
#include "celestial_mechanics/time/TimeConverter.hpp"
#include <Core>
#include <Geometry>
#include "tests/Paths.hpp"
#include "resources/coordinates_result.hpp"

class ReferenceSystemConverterTest : public testing::Test {
    protected:
        EOPContainer EOPcontainer = EOPContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");
        ReferenceSystemConverter coordinatesConverter = ReferenceSystemConverter(EOPcontainer);
        TimeConverter<EOPContainer> timeConverter = TimeConverter<EOPContainer>(EOPcontainer);
};

TEST_F(ReferenceSystemConverterTest, 6700e3x_GCRS2ITRSworks) {

    Eigen::Vector<double, 3> r_GCRS{6700e3, 0, 0};

    for (auto el : earthRotationResult) {

        double ttJD = el[0];
        const auto tt = Time<Scale::TT>::fromJD(ttJD);

        Eigen::Quaternion<double> gcrs2itrs = coordinatesConverter.GCRS2ITRS(tt);
        Eigen::Vector<double, 3> r_ITRS = gcrs2itrs._transformVector(r_GCRS);

        ASSERT_NEAR(r_ITRS(0), el[1], 0.1);
        ASSERT_NEAR(r_ITRS(1), el[2], 0.1);
        ASSERT_NEAR(r_ITRS(2), el[3], 0.1);

        ASSERT_NEAR((r_ITRS(0) - el[1]) / 6700e3, 0, 1e-8);
        ASSERT_NEAR((r_ITRS(1) - el[2]) / 6700e3, 0, 1e-8);
        ASSERT_NEAR((r_ITRS(2) - el[3]) / 6700e3, 0, 1e-8);
    }
}

TEST_F(ReferenceSystemConverterTest, 6700e3y_GCRS2ITRSworks) {

    Eigen::Vector<double, 3> r_GCRS{0, 6700e3, 0};

    for (auto el : earthRotationResult) {

        double ttJD = el[0];
        const auto tt = Time<Scale::TT>::fromJD(ttJD);

        Eigen::Quaternion<double> gcrs2itrs = coordinatesConverter.GCRS2ITRS(tt);
        Eigen::Vector<double, 3> r_ITRS = gcrs2itrs._transformVector(r_GCRS);

        ASSERT_NEAR(r_ITRS(0), el[4], 0.1);
        ASSERT_NEAR(r_ITRS(1), el[5], 0.1);
        ASSERT_NEAR(r_ITRS(2), el[6], 0.1);

        ASSERT_NEAR((r_ITRS(0) - el[4]) / 6700e3, 0, 1e-8);
        ASSERT_NEAR((r_ITRS(1) - el[5]) / 6700e3, 0, 1e-8);
        ASSERT_NEAR((r_ITRS(2) - el[6]) / 6700e3, 0, 1e-8);
    }
}

TEST_F(ReferenceSystemConverterTest, 6700e3z_GCRS2ITRSworks) {

    Eigen::Vector<double, 3> r_GCRS{0, 0, 6700e3};

    for (auto el : earthRotationResult) {

        double ttJD = el[0];
        const auto tt = Time<Scale::TT>::fromJD(ttJD);

        Eigen::Quaternion<double> gcrs2itrs = coordinatesConverter.GCRS2ITRS(tt);
        Eigen::Vector<double, 3> r_ITRS = gcrs2itrs._transformVector(r_GCRS);

        ASSERT_NEAR(r_ITRS(0), el[7], 0.1);
        ASSERT_NEAR(r_ITRS(1), el[8], 0.1);
        ASSERT_NEAR(r_ITRS(2), el[9], 0.1);

        ASSERT_NEAR((r_ITRS(0) - el[7]) / 6700e3, 0, 1e-8);
        ASSERT_NEAR((r_ITRS(1) - el[8]) / 6700e3, 0, 1e-8);
        ASSERT_NEAR((r_ITRS(2) - el[9]) / 6700e3, 0, 1e-8);
    }
}