#pragma once

#include <string>
#include <Core>
#include <Geometry>
#include <GravityModel.hpp>

class EarthGravity{

    GeographicLib::GravityModel gravityModel_;

    public:
        EarthGravity(const std::string& path, unsigned int n, unsigned int m)
                    : gravityModel_(GeographicLib::GravityModel("egm2008", path, n, m)){}

        struct SatelliteParameters{};

        template<typename Params>
        Eigen::Vector3d calcAcceleration(const Eigen::Vector3d& positionECI, const Eigen::Vector3d& velocityECI,
                                            const double mass, const SatelliteParameters& satParams,
                                                const Params& params){
            const Eigen::Quaternion eci2ecef = params.eci2ecef;
            const Eigen::Vector3d posECEF = eci2ecef._transformVector(positionECI);
            
            double gx, gy, gz;
            gravityModel_.V(posECEF(0), posECEF(1), posECEF(2), &gx, &gy, &gz);

            const Eigen::Vector3d accECEF = Eigen::Vector3d{gx, gy, gz};

            return eci2ecef.conjugate()._transformVector(accECEF);
        }
};