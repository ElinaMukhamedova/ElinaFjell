#include <string>
#include <vector>
#include "celestial_mechanics/Interpolation.hpp"
#include "DutContainer.hpp"
#include "celestial_mechanics/rapidcsv.h"

DutContainer DutContainer::buildFromFile(std::filesystem::path path_to_file,
                                            char separator,
                                            std::string MJD_column_name, 
                                            std::string dut_column_name)
{
    rapidcsv::Document doc = rapidcsv::Document(path_to_file,
                                                rapidcsv::LabelParams(0, -1),
                                                rapidcsv::SeparatorParams(separator),
                                                rapidcsv::ConverterParams(true));

    std::vector<double> MJD_column = doc.GetColumn<double>(MJD_column_name);
    std::vector<double> dut_column = doc.GetColumn<double>(dut_column_name);
    std::size_t dut_size = dut_column.size();

    std::vector<double> nodes;
    std::vector<double> values;

    for (std::size_t i = 0; i < dut_size; ++i)
        if (std::isnan(dut_column[i]) == false) {
            nodes.push_back(MJD_column[i]);
            values.push_back(dut_column[i]);
        }


    return DutContainer{nodes, values};
}

const std::vector<double>& DutContainer::MJD_nodes() const {return MJD_nodes_;}
const std::vector<double>& DutContainer::dut_values() const {return dut_values_;}

double DutContainer::dut(double mjd) const {
    return interpolant_.evaluate(mjd);
}