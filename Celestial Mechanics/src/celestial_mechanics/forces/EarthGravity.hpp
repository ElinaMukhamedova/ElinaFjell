#pragma once

#include <string>
#include <Core>
#include <Geometry>
#include <GeographicLib/GravityModel.hpp>

class EarthGravity{

    std::string path_;
    unsigned int n_;
    unsigned int m_;
    GeographicLib::GravityModel gravityModel_;

    public:
        EarthGravity(const std::string& path, unsigned int n, unsigned int m)
                    : path_(path), n_(n), m_(m), gravityModel_(GeographicLib::GravityModel("egm2008", path, n, m)){}
        
        std::string path() const;
        unsigned int n() const;
        unsigned int m() const;
        
        EarthGravity(EarthGravity const& gravity);

        double gravitationalParameter() const;

        struct SatelliteParameters{};

        template<typename Params>
        Eigen::Vector3d calcAcceleration(const Eigen::Vector3d& positionECI, const Eigen::Vector3d& velocityECI,
                                            const double mass, const SatelliteParameters& satParams,
                                                const Params& params) const {
            const Eigen::Quaternion eci2ecef = params.eci2ecef;
            const Eigen::Vector3d posECEF = eci2ecef._transformVector(positionECI);
            
            double gx, gy, gz;
            gravityModel_.V(posECEF(0), posECEF(1), posECEF(2), gx, gy, gz);

            const Eigen::Vector3d accECEF = Eigen::Vector3d{gx, gy, gz};

            return eci2ecef.conjugate()._transformVector(accECEF);
        }
};