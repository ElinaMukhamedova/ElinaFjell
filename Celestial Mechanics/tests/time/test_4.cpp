#include <gtest/gtest.h>
#include "celestial_mechanics/Interpolation.hpp"
#include "celestial_mechanics/time/DutContainer.hpp"
#include "celestial_mechanics/rapidcsv.h"
#include "tests/Paths.hpp"
#include <vector>

class DutInterpolationTest : public testing::Test {
    protected:
    DutContainer dutContainer = DutContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");
    std::vector<double> MJD_nodes;
    std::vector<double> dut_values;
    std::size_t N;
    double first_inner_point, first_inner_value;
    double last_inner_point, last_inner_value;
    void SetUp() {
        MJD_nodes = dutContainer.MJD_nodes();
        dut_values = dutContainer.dut_values();
        N = dut_values.size();
        first_inner_point = MJD_nodes[0] + (MJD_nodes[1] - MJD_nodes[0]) / 3;
        first_inner_value = dut_values[0] + (dut_values[1] - dut_values[0]) / 3;
        last_inner_point = MJD_nodes[N-2] + 2 * (MJD_nodes[N-1] - MJD_nodes[N-2]) / 3;
        last_inner_value = dut_values[N-2] + 2 * (dut_values[N-1] - dut_values[N-2]) / 3;
    }
};

TEST_F(DutInterpolationTest, InterpolationAtNodesWorks) {
    for (std::size_t i = 0; i < N; ++i)
        EXPECT_NEAR(dutContainer.dut(MJD_nodes[i]), dut_values[i], 1e-12);
}

TEST_F(DutInterpolationTest, InterpolationAtInnerPointsWorks) {
    EXPECT_NEAR(dutContainer.dut(first_inner_point), first_inner_value, 1e-12);
    EXPECT_NEAR(dutContainer.dut(last_inner_point), last_inner_value, 1e-12);
}