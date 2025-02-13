#include <gtest/gtest.h>
#include "numerical_analysis/transport_equation/Grid.hpp"
#include <array>
#include <iostream>


TEST(GridTest, GridStaticMembersWork) {
    ASSERT_EQ(Grid::L, 20);
    ASSERT_EQ(Grid::T, 18);
    ASSERT_EQ(Grid::h_x, 0.5);
    ASSERT_EQ(Grid::N_x, 41);
}

TEST(GridTest, GridConstructorWorks) {
    std::array<double, 41> u0{};
    Grid myGrid{0.7, u0};
    ASSERT_EQ(myGrid.L, 20);
    ASSERT_EQ(myGrid.T, 18);
    ASSERT_EQ(myGrid.h_x, 0.5);
    ASSERT_EQ(myGrid.N_x, 41);
    ASSERT_EQ(myGrid.h_t, 0.35);
    for (int i = 0; i < Grid::N_x; ++i) {ASSERT_EQ(myGrid.initialCondition[i], u0[i]);}
}