#pragma once

#include <calceph.h>
#include <Core>
#include "celestial_mechanics/time/Time.hpp"
#include "celestial_mechanics/Exception.hpp"

enum class CelestialBody{Mercury = 1, Venus = 2, Earth = 399,
                        Mars = 4, Jupiter = 5, Saturn = 6,
                        Uranus = 7, Neptune = 8, Pluto = 9,
                        Moon = 301, Sun = 10};

class Eternal {
    t_calcephbin *peph;
    
    public:
        Eternal(const std::string& pathEphemerides) : peph{nullptr} {
            peph = calceph_open(pathEphemerides.c_str());
            if (!peph){
                throw Exception("Can't open the ephemerides file!");
            }
        }

        ~Eternal(){
            calceph_close(peph);
        }

        double AU();

        Eigen::Vector3d vector(Time<Scale::TDB> tdb, CelestialBody Centre, CelestialBody Target) const;
};