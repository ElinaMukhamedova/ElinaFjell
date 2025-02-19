#pragma once

#include "celestial_mechanics/time/Time.hpp"
#include <Core>
#include <Geometry>
#include "celestial_mechanics/forces/EarthGravity.hpp"
#include "celestial_mechanics/AccelerationCalculator.hpp"
#include "celestial_mechanics/coordinates/ReferenceSystemConverter.hpp"
#include "celestial_mechanics/coordinates/EOPContainer.hpp"
#include "celestial_mechanics/time/TimeConverter.hpp"
#include "celestial_mechanics/time/DutContainer.hpp"


template<typename EarthGrav, typename ... OtherForces>
class Satellite {
    TimeConverter<DutContainer> timeConverter_;
    ReferenceSystemConverter referenceSystemConverter_;
    ForceCalculator<EarthGrav, OtherForces...> forceCalculator_;
    typename ForceCalculator<EarthGrav, OtherForces...>::SatelliteParameters satelliteParameters_;

    public:
        Satellite(DutContainer const& dutContainer, EOPContainer const& EOPcontainer,
                    EarthGrav const& gravity, OtherForces const& ... forces,
                        double satelliteMass,
                            typename EarthGrav::SatelliteParameters const& EarthGravParameters,
                                typename OtherForces::SatelliteParameters const&... OtherForcesParameters)
            :   timeConverter_(dutContainer), referenceSystemConverter_(EOPcontainer),
                    forceCalculator_(gravity, forces...){
                        satelliteParameters_ = typename ForceCalculator<EarthGrav, OtherForces...>::SatelliteParameters{EarthGravParameters, OtherForcesParameters...};
                        satelliteParameters_.mass = satelliteMass;
                    }

        static constexpr unsigned int dim = 6;
        using ArgType = Time<Scale::TT>;
        using State = Eigen::Vector<double, dim>;
        struct StateAndArg {
            State state;
            ArgType arg;
        };

        struct Parameters {
            Eigen::Quaternion<double> eci2ecef;
            Time<Scale::TDB> tdb;
        };

        State evaluate(StateAndArg const& stateAndArg) const {

            Eigen::Quaternion<double> const eci2ecef = referenceSystemConverter_.GCRS2ITRS(stateAndArg.arg);
            Time<Scale::TDB> const tdb = timeConverter_.convert<Scale::TDB>(stateAndArg.arg);
            Parameters params{eci2ecef, tdb};
            

            Eigen::Vector<double, 3> const positionECI{stateAndArg.state(0), stateAndArg.state(1), stateAndArg.state(2)};
            Eigen::Vector<double, 3> const velocityECI{stateAndArg.state(3), stateAndArg.state(4), stateAndArg.state(5)};
            Eigen::Vector<double, 3> const acceleration = forceCalculator_.template calcAcceleration<Parameters>(positionECI, velocityECI,
                                                                                            satelliteParameters_.mass,
                                                                                            satelliteParameters_,
                                                                                            params);
            return Eigen::Vector<double, 6>{velocityECI(0), velocityECI(1), velocityECI(2),
                                            acceleration(0), acceleration(1), acceleration(2)};
        }
};