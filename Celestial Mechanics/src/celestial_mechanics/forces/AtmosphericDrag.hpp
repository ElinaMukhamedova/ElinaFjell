#pragma once

#include <Core>
#include <Geometry>
#include <GeographicLib/Geocentric.hpp>
#include <math.h>
#include "GOST4401_81.hpp"
#include <iostream>

class AtmosphericDrag {
    public:

        struct SatelliteParameters {
            double S_drag;
            double C_drag;
        };

        template<typename Params>
        Eigen::Vector3d calcAcceleration(const Eigen::Vector3d& positionECI, const Eigen::Vector3d& velocityECI,
                                            const double mass, const SatelliteParameters& satParams,
                                                const Params& params) const {
            const Eigen::Quaternion eci2ecef = params.eci2ecef;
            const Eigen::Vector3d posECEF = eci2ecef._transformVector(positionECI);
            const Eigen::Vector3d velECEF = eci2ecef._transformVector(velocityECI);
            
            const double siderealDay = 23 * 60 * 60 + 56 * 60 + 4;
            Eigen::Vector3d omega{0, 0, 2*M_PI/siderealDay};
            Eigen::Vector3d rotationVelocity = omega.cross(posECEF);
            
            GeographicLib::Geocentric const& Earth = GeographicLib::Geocentric::WGS84();
            double latitude_degrees, longitude_degrees, height_metres;
            Earth.Reverse(posECEF(0), posECEF(1), posECEF(2), latitude_degrees, longitude_degrees, height_metres);
            
            double rho;
            if (height_metres < GOST4401_81::minHeight) {
                std::cout << "Satellite has fallen!\n";
                throw Exception("Requested point is out of bounds to Interpolant");
            }
            if (height_metres > GOST4401_81::maxHeight)
                rho = 0;
            else {
                auto iter = std::lower_bound(GOST4401_81::heights.begin(), GOST4401_81::heights.end(), height_metres);
                int index = iter - GOST4401_81::heights.begin();

                const bool condition = index != 0;
                double a = condition ? GOST4401_81::heights[index - 1] : GOST4401_81::heights[0];
                double b = condition ? *iter : GOST4401_81::heights[1];
                double value_a = condition ? GOST4401_81::densities[index - 1] : GOST4401_81::densities[0];
                double value_b = condition ? GOST4401_81::densities[index] : GOST4401_81::densities[1];

                double slope = (value_b - value_a) / (b - a);
                rho = slope * (height_metres - a) + value_a;
            }

            

            double v2 = (velECEF - rotationVelocity).squaredNorm();
            Eigen::Vector3d n = -velECEF.normalized();
            Eigen::Vector3d accECEF = (0.5 * rho * v2 * satParams.C_drag * satParams.S_drag / mass) * n;

            return eci2ecef.conjugate()._transformVector(accECEF);
        }
};