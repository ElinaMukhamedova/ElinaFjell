#include <gtest/gtest.h>
#include "numerical_analysis/transport_equation/UpwindScheme.hpp"


TEST(GridTest, GridStaticMembersWork) {
    ASSERT_EQ(Grid::L, 20);
    ASSERT_EQ(Grid::T, 18);
    ASSERT_EQ(Grid::h_x, 0.5);
}

TEST(GridTest, GridConstructorWorks) {
    
}