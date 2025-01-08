#include <gtest/gtest.h>
#include "celestial_mechanics/time/Time.hpp"

class TimeTest : public testing::Test {
    protected:
        int iy, im, id, ihour, imin;
        double sec, jd, mjd;

        int iy_2, im_2, id_2, ihour_2, imin_2;
        double sec_2, jd_2, mjd_2;

        void SetUp() {
            iy = 2008; im = 2; id = 29;
            ihour = 23; imin = 59; sec = 59.9;
            jd = 2454526.499999; mjd = 54525.999999;

            iy_2 = 2008; im_2 = 3; id_2 = 1;
            ihour_2 = 5; imin_2 = 59; sec_2 = 59.9;
            jd_2 = 2454526.749999; mjd_2 = 54526.249999;
        }
};

TEST_F(TimeTest, fromJDWorks) {
    const auto JD = Time<Scale::UT1>::fromJD(jd);
    EXPECT_EQ(JD.jdInt(), 2454526);
    EXPECT_NEAR(JD.jdFrac(), 0.499999, 1e-6);
    EXPECT_NEAR(JD.jd(), 2454526.499999, 1e-6);
    EXPECT_NEAR(JD.mjd(), 54525.999999, 1e-6);

    const auto JD_2 = Time<Scale::UT1>::fromJD(jd_2);
    EXPECT_EQ(JD_2.jdInt(), 2454526 + 1);
    EXPECT_NEAR(JD_2.jdFrac(), 0.749999 - 1, 1e-6);
    EXPECT_NEAR(JD_2.jd(), 2454526.749999, 1e-6);
    EXPECT_NEAR(JD_2.mjd(), 54526.249999, 1e-6);
}

TEST_F(TimeTest, fromMJDWorks) {
    const auto JD = Time<Scale::UT1>::fromMJD(mjd);
    EXPECT_EQ(JD.jdInt(), 2454526);
    EXPECT_NEAR(JD.jdFrac(), 0.499999, 1e-6);
    EXPECT_NEAR(JD.jd(), 2454526.499999, 1e-6);
    EXPECT_NEAR(JD.mjd(), 54525.999999, 1e-6);

    const auto JD_2 = Time<Scale::UT1>::fromMJD(mjd_2);
    EXPECT_EQ(JD_2.jdInt(), 2454526 + 1);
    EXPECT_NEAR(JD_2.jdFrac(), 0.749999 - 1, 1e-6);
    EXPECT_NEAR(JD_2.jd(), 2454526.749999, 1e-6);
    EXPECT_NEAR(JD_2.mjd(), 54526.249999, 1e-6);
}

TEST_F(TimeTest, fromCalendarWorks) {
    const auto JD = Time<Scale::UT1>::fromCalendar(iy, im, id, ihour, imin, sec);
    EXPECT_EQ(JD.jdInt(), 2454526);
    EXPECT_NEAR(JD.jdFrac(), 0.499999, 1e-6);
    EXPECT_NEAR(JD.jd(), 2454526.499999, 1e-6);
    EXPECT_NEAR(JD.mjd(), 54525.999999, 1e-6);

    const auto JD_2 = Time<Scale::UT1>::fromCalendar(iy_2, im_2, id_2, ihour_2, imin_2, sec_2);
    EXPECT_EQ(JD_2.jdInt(), 2454526 + 1);
    EXPECT_NEAR(JD_2.jdFrac(), 0.749999 - 1, 1e-6);
    EXPECT_NEAR(JD_2.jd(), 2454526.749999, 1e-6);
    EXPECT_NEAR(JD_2.mjd(), 54526.249999, 1e-6);
}

TEST_F(TimeTest, comparisonWorks) {
    const auto JD = Time<Scale::UT1>::fromJD(jd);
    const auto JD_2 = Time<Scale::UT1>::fromJD(jd_2);
    EXPECT_TRUE(JD == JD);
    EXPECT_TRUE(JD <= JD);
    EXPECT_TRUE(JD >= JD);
    EXPECT_FALSE(JD > JD);
    EXPECT_FALSE(JD < JD);

    EXPECT_TRUE(JD <= JD_2);
    EXPECT_TRUE(JD < JD_2);
    EXPECT_FALSE(JD >= JD_2);
    EXPECT_FALSE(JD > JD_2);
    EXPECT_FALSE(JD == JD_2);
}

TEST_F(TimeTest, operatorsWork) {
    const auto JD = Time<Scale::UT1>::fromJD(jd);
    const auto JD_2 = Time<Scale::UT1>::fromJD(jd_2);
    EXPECT_NEAR(JD_2 - JD, 0.25, 1e-12);
    EXPECT_TRUE(JD + 0.25 == JD_2);
    EXPECT_TRUE(std::abs((JD_2 - 0.25) - JD) <= 1e-6);
    EXPECT_TRUE(JD_2 - 0.25 == JD);
}

TEST(jdFracTest, AbsoluteNotGreaterThanOneHalf) {
    const auto utc1 = Time<Scale::UTC>(13, -0.5);
    ASSERT_EQ(utc1.jdInt(), 12);
    ASSERT_EQ(utc1.jdFrac(), 0.5);

    const auto utc2 = Time<Scale::UTC>(17, 0.5);
    ASSERT_EQ(utc2.jdInt(), 17);
    ASSERT_EQ(utc2.jdFrac(), 0.5);

    const auto utc3 = Time<Scale::UTC>(13, -0.4);
    ASSERT_EQ(utc3.jdInt(), 13);
    ASSERT_EQ(utc3.jdFrac(), -0.4);

    const auto utc4 = Time<Scale::UTC>(17, 0.4);
    ASSERT_EQ(utc4.jdInt(), 17);
    ASSERT_EQ(utc4.jdFrac(), 0.4);

    const auto utc5 = Time<Scale::UTC>(13, -0.6);
    ASSERT_EQ(utc5.jdInt(), 12);
    ASSERT_EQ(utc5.jdFrac(), 0.4);

    const auto utc6 = Time<Scale::UTC>(17, 0.6);
    ASSERT_EQ(utc6.jdInt(), 18);
    ASSERT_EQ(utc6.jdFrac(), -0.4);
}