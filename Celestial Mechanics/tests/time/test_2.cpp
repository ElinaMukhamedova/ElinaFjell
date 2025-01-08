#include <gtest/gtest.h>
#include "celestial_mechanics/time/DutContainer.hpp"
#include <vector>
#include <string>
#include <cmath>
#include "celestial_mechanics/rapidcsv.h"
#include "tests/Paths.hpp"
#include <filesystem>

TEST(DutContainerTest, RapidcsvWorks) {
    std::cout << std::filesystem::current_path() << std::endl;
    std::cout << projectPath() << std::endl;
    std::cout << resourcesPath() << std::endl;

    rapidcsv::Document doc = rapidcsv::Document(resourcesPath() / "dut.csv", rapidcsv::LabelParams(0, -1),
                        rapidcsv::SeparatorParams(';'), rapidcsv::ConverterParams(true));
    std::cout << resourcesPath() << std::endl;
    std::vector<double> MJD_column = doc.GetColumn<double>("MJD");
    std::vector<double> dut_column = doc.GetColumn<double>("UT1-UTC");
    std::vector<double> MJD_nodes;
    std::vector<double> dut_values;
    std::size_t N = dut_column.size();
    for (std::size_t i = 0; i < N; ++i)
        if (std::isnan(dut_column[i]) == false) {
            MJD_nodes.push_back(MJD_column[i]);
            dut_values.push_back(dut_column[i]);
        }
    
    std::cout << dut_column.size() << std::endl;
    std::cout << MJD_column.size() << std::endl;
    std::cout << dut_values.size() << std::endl;
    std::cout << MJD_nodes.size() << std::endl;

    EXPECT_TRUE(true);
}

TEST(DutContainerTest, SpaceSeparatorWorks) {
    rapidcsv::Document doc = rapidcsv::Document(resourcesPath() / "trimmed.csv", rapidcsv::LabelParams(0, -1),
                        rapidcsv::SeparatorParams(' '), rapidcsv::ConverterParams(true));

    std::vector<double> MJD_column = doc.GetColumn<double>("MJD");
    std::vector<double> dut_column = doc.GetColumn<double>("UT1-UTC");
    std::vector<double> MJD_nodes;
    std::vector<double> dut_values;
    std::size_t N = dut_column.size();
    for (std::size_t i = 0; i < N; ++i)
        if (std::isnan(dut_column[i]) == false) {
            MJD_nodes.push_back(MJD_column[i]);
            dut_values.push_back(dut_column[i]);
        }
    
    std::cout << dut_column.size() << std::endl;
    std::cout << MJD_column.size() << std::endl;
    std::cout << dut_values.size() << std::endl;
    std::cout << MJD_nodes.size() << std::endl;

    EXPECT_TRUE(true);
}

TEST(DutContainerTest, ConstructorWorks) {
    DutContainer dut = DutContainer::buildFromFile(resourcesPath() / "trimmed.csv", ' ', "MJD", "UT1-UTC");
    EXPECT_TRUE(dut.MJD_nodes().size() == dut.dut_values().size());
}

TEST(DutContainerTest, CorrectValues) {
    DutContainer dut = DutContainer::buildFromFile(resourcesPath() / "earth_rotation.csv");
    std::cout << dut.MJD_nodes()[13] << std::endl;
    EXPECT_TRUE(dut.MJD_nodes()[13] == 37678);
    std::cout << dut.dut_values()[13] << std::endl;
    EXPECT_NEAR(dut.dut_values()[13], 0.0265403, 1e-12);
    std::cout << dut.MJD_nodes()[dut.dut_values().size() - 1] << std::endl;
    EXPECT_TRUE(dut.MJD_nodes()[dut.dut_values().size() - 1] == 59758);
    std::cout << dut.dut_values()[dut.dut_values().size() - 1] << std::endl;
    EXPECT_NEAR(dut.dut_values()[dut.dut_values().size() - 1], -0.0733645, 1e-12);
}