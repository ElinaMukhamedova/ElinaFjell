#include "SolarRadiationPressure.hpp"
#include <Geometry>

int SolarRadiationPressure::scalarCylindricalShadow(CelestialBody Caster, double rCaster,
                                                        Eigen::Vector3d SunSatellite,
                                                            Time<Scale::TDB> tdb) const {
            
    Eigen::Vector3d SunCaster = eternal_.vector(tdb, CelestialBody::Sun, Caster);
    Eigen::Vector3d CasterSun = -SunCaster;
            
    Eigen::Vector3d CasterSatellite = CasterSun + SunSatellite;

    double shadow = 1;
    if (CasterSun.dot(CasterSatellite) <= 0) {
        double numerator = (CasterSun.cross(CasterSatellite)).norm();
        double denominator = CasterSun.norm();
        double rho = numerator / denominator;
        if (rho < rCaster)
            shadow = 0;
    }

    return shadow;
}