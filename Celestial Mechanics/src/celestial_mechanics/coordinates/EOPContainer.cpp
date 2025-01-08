#include <filesystem>
#include "celestial_mechanics/Interpolation.hpp"
#include "celestial_mechanics/rapidcsv.h"
#include <vector>
#include <string>
#include "EOPContainer.hpp"

EOPContainer EOPContainer::buildFromFile(std::filesystem::path abs_path,
                    char separator,
                    std::string MJD_column_name,
                    std::string dut_column_name ,
                    std::string dX_column_name ,
                    std::string dY_column_name ,
                    std::string xTerr_column_name,
                    std::string yTerr_column_name)
{
    rapidcsv::Document doc = rapidcsv::Document(abs_path,
                                                rapidcsv::LabelParams(0, -1),
                                                rapidcsv::SeparatorParams(separator),
                                                rapidcsv::ConverterParams(true));
    std::vector<double> MJD_column = doc.GetColumn<double>(MJD_column_name);
    std::vector<double> dut_column = doc.GetColumn<double>(dut_column_name);
    std::vector<double> dX_column = doc.GetColumn<double>(dX_column_name);
    std::vector<double> dY_column = doc.GetColumn<double>(dY_column_name);
    std::vector<double> xTerr_column = doc.GetColumn<double>(xTerr_column_name);
    std::vector<double> yTerr_column = doc.GetColumn<double>(yTerr_column_name);
    std::size_t N = MJD_column.size();

    std::vector<double> MJD_nodes, dut_values, dX_values, dY_values, xTerr_values, yTerr_values;

    for (std::size_t i = 0; i < N; ++i){
        MJD_nodes.push_back(MJD_column[i]);
        dut_values.push_back(dut_column[i]);
        dX_values.push_back(dX_column[i] * 4.848136811095359935899141e-6);
        dY_values.push_back(dY_column[i] * 4.848136811095359935899141e-6);
        xTerr_values.push_back(xTerr_column[i] * 4.848136811095359935899141e-6);
        yTerr_values.push_back(yTerr_column[i] * 4.848136811095359935899141e-6);
    }
    
    return EOPContainer(MJD_nodes, dut_values, dX_values, dY_values, xTerr_values, yTerr_values);
}

const std::vector<double>& EOPContainer::MJD_nodes() const {return MJD_nodes_;}
const std::vector<double>& EOPContainer::dut_values() const {return dut_values_;}
const std::vector<double>& EOPContainer::dX_values() const {return dX_values_;}
const std::vector<double>& EOPContainer::dY_values() const {return dY_values_;}
const std::vector<double>& EOPContainer::xTerr_values() const {return xTerr_values_;}
const std::vector<double>& EOPContainer::yTerr_values() const {return yTerr_values_;}

double EOPContainer::dut(double mjd) const {
    return dut_interpolant_.evaluate(mjd);
}
double EOPContainer::dX(double mjd) const {
    return dX_interpolant_.evaluate(mjd);
}
double EOPContainer::dY(double mjd) const {
    return dY_interpolant_.evaluate(mjd);
}
double EOPContainer::xTerr(double mjd) const {
    return xTerr_interpolant_.evaluate(mjd);
}
double EOPContainer::yTerr(double mjd) const {
    return yTerr_interpolant_.evaluate(mjd);
}