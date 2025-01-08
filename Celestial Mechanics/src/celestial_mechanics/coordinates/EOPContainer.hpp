#pragma once

#include <filesystem>
#include "celestial_mechanics/Interpolation.hpp"
#include <vector>
#include <string>

class EOPContainer {

    std::vector<double> MJD_nodes_;

    std::vector<double> dut_values_;
    std::vector<double> dX_values_;
    std::vector<double> dY_values_;
    std::vector<double> xTerr_values_;
    std::vector<double> yTerr_values_;

    Interpolant<double, double> dut_interpolant_;
    Interpolant<double, double> dX_interpolant_;
    Interpolant<double, double> dY_interpolant_;
    Interpolant<double, double> xTerr_interpolant_;
    Interpolant<double, double> yTerr_interpolant_;

    public:

        EOPContainer(const std::vector<double>& MJD_nodes,
                    const std::vector<double>& dut_values,
                    const std::vector<double>& dX_values, const std::vector<double>& dY_values,
                    const std::vector<double>& xTerr_values, const std::vector<double>& yTerr_values)
                        : MJD_nodes_(MJD_nodes), dut_values_(dut_values),
                        dX_values_(dX_values), dY_values_(dY_values), xTerr_values_(xTerr_values), yTerr_values_(yTerr_values),
                        dut_interpolant_(MJD_nodes, dut_values),
                        dX_interpolant_(MJD_nodes, dX_values), dY_interpolant_(MJD_nodes, dY_values),
                        xTerr_interpolant_(MJD_nodes, xTerr_values), yTerr_interpolant_(MJD_nodes, yTerr_values){}
        
        static EOPContainer buildFromFile(std::filesystem::path abs_path,
                    char separator = ',',
                    std::string MJD_column_name = "mjd",
                    std::string dut_column_name = "UT1-UTC s",
                    std::string dX_column_name = "dX arcsec",
                    std::string dY_column_name = "dY arcsec",
                    std::string xTerr_column_name = "x arcsec",
                    std::string yTerr_column_name = "y arcsec");
        

        const std::vector<double>& MJD_nodes() const;
        const std::vector<double>& dut_values() const;
        const std::vector<double>& dX_values() const;
        const std::vector<double>& dY_values() const;
        const std::vector<double>& xTerr_values() const;
        const std::vector<double>& yTerr_values() const;
        
        double dut(double mjd) const;
        double dX(double mjd) const;
        double dY(double mjd) const;
        double xTerr(double mjd) const;
        double yTerr(double mjd) const;
};