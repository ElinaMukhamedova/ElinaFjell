#pragma once

#include <Core>
#include <Geometry>
#include <GeographicLib/Geocentric.hpp>
#include <math.h>
#include "GOST4401_81.hpp"

class AtmosphericDrag {

    GOST4401_81 gost_;

    public:
        AtmosphericDrag(GOST4401_81 gost) : gost_(gost) {}

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

            double rho = gost_.density(height_metres);
            double v2 = (velECEF - rotationVelocity).squaredNorm();
            Eigen::Vector3d n = -velECEF.normalized();
            Eigen::Vector3d accECEF = (0.5 * rho * v2 * satParams.C_drag * satParams.S_drag / mass) * n;

            return eci2ecef.conjugate()._transformVector(accECEF);
        }
};