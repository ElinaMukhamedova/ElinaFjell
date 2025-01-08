#include <gtest/gtest.h>
#include "celestial_mechanics/Interpolation.hpp"
#include "celestial_mechanics/time/DutContainer.hpp"
#include "tests/Paths.hpp"
#include <vector>
#include "celestial_mechanics/Exception.hpp"

class InterpolationTest : public testing::Test {
    protected:
    std::vector<double> nodes = {1, 3, 5, 7, 9, 11, 13};
    std::vector<double> values = {3, 7, 11, 15, 19, 23, 27};
    std::size_t N = nodes.size();
    Interpolant<double, double> interpolant{nodes, values};
    std::vector<double> midpoints = {2, 4, 6, 8, 10, 12};
    std::vector<double> values_midpoints = {5, 9, 13, 17, 21, 25};
};

TEST_F(InterpolationTest, InterpolationAtNodesWorks) {
    for (std::size_t i = 0; i < N; ++i)
        EXPECT_NEAR(interpolant.evaluate(nodes[i]), values[i], 1e-12);
}

TEST_F(InterpolationTest, InterpolationAtMidpointsWorks) {
    for (std::size_t i = 0; i < N - 1; ++i)
        EXPECT_NEAR(interpolant.evaluate(midpoints[i]), values_midpoints[i], 1e-12);
}

TEST(InterpolantTest, OutOfBoundsExceptionWorks) {
    Interpolant<double, double> interpolant{{13, 15, 17}, {1, 2, 3, 5}};
    ASSERT_THROW(interpolant.evaluate(21), Exception);
    ASSERT_THROW(interpolant.evaluate(11), Exception);
}