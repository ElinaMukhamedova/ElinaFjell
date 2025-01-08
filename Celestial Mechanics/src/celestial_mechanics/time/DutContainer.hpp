#pragma once

#include <vector>
#include <array>
#include <string>
#include <filesystem>
#include "celestial_mechanics/Interpolation.hpp"
#include "celestial_mechanics/rapidcsv.h"

class DutContainer {
    std::vector<double> MJD_nodes_;
    std::vector<double> dut_values_;
    Interpolant<double, double> interpolant_;

    public:
        DutContainer(const std::vector<double>& nodes, const std::vector<double>& values) : MJD_nodes_(nodes), dut_values_(values), interpolant_(nodes, values){}
        
        static DutContainer buildFromFile(std::filesystem::path path_to_file,
                                            char separator = ',',
                                            std::string MJD_column_name = "mjd", 
                                            std::string dut_column_name = "UT1-UTC s");

        const std::vector<double>& MJD_nodes() const;
        const std::vector<double>& dut_values() const;
        
        double dut(double mjd) const;
};