#pragma once

#include <Core>
#include <celestial_mechanics/time/Time.hpp>
#include <string>
#include <cmath>
#include "Eternal.hpp"

class SolarRadiationPressure {

    Eternal eternal_;
    double TSI_;

    public:
        SolarRadiationPressure(Eternal& eternal, double TSI = 1361)
            : eternal_(eternal), TSI_(TSI) {}

        struct SatelliteParameters {
            double S_srp;
        };

        int scalarCylindricalShadow(CelestialBody Caster, double rCaster, Eigen::Vector3d SunSatellite, Time<Scale::TDB> tdb);

        template<typename Params>
        Eigen::Vector3d calcAcceleration(const Eigen::Vector3d& positionECI, const Eigen::Vector3d& velocityECI,
                                            const double mass, const SatelliteParameters& satParams,
                                                const Params& params){

            Time<Scale::TDB> tdb = params.tdb;
            
            const double AU = 149597870700;
            const double rEarth = 6378140;
            const double rMoon = 1737400;
            const double lightSpeed = 299792458;
            
            Eigen::Vector3d SunEarth = eternal_.vector(tdb, CelestialBody::Sun, CelestialBody::Earth);
            Eigen::Vector3d EarthSatellite = positionECI;
            Eigen::Vector3d SunSatellite = SunEarth + EarthSatellite;

            int shadow = scalarCylindricalShadow(CelestialBody::Earth, rEarth, SunSatellite, tdb);
            shadow *= scalarCylindricalShadow(CelestialBody::Moon, rMoon, SunSatellite, tdb);

            Eigen::Vector3d n = SunSatellite.normalized();
            double SunSatelliteDistance = SunSatellite.norm();
            Eigen::Vector3d j0 = TSI_ * std::pow(AU/SunSatelliteDistance, 2) * n;

            Eigen::Vector3d F_srp = (shadow * satParams.S_srp / lightSpeed) * j0;

            return F_srp / mass;
        }

};