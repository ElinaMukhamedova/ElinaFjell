#pragma once

#include "time/Time.hpp"
#include <Core>
#include "forces/EarthGravity.hpp"
#include "AccelerationCalculator.hpp"
#include "Paths.hpp"
#include "coordinates/ReferenceSystemConverter.hpp"
#include "coordinates/EOPContainer.hpp"
#include "time/TimeConverter.hpp"
#include "time/DutContainer.hpp"


template<Scale scale, typename EarthGrav, typename ... OtherForces>
class Satellite {
    TimeConverter timeConverter_;
    ReferenceSystemConverter referenceSystemConverter_;
    ForceCalculator forceCalculator_;

    public:
        Satellite(DutContainer const& dutContainer, EOPContainer const& EOPcontainer,
                    EarthGrav const& gravity, OtherForces const& ... forces)
            : timeConverter_(dutContainer),referenceSystemConverter_(EOPcontainer),
                forceCalculator_(gravity, forces...) {}

        static constexpr unsigned int dim = 6;
        using ArgType = Time<scale>;
        using State = Eigen::Vector<double, dim>;
        struct StateAndArg {
            State state;
            ArgType arg;
        };

        struct Parameters {
            Eigen::Quaternion eci2ecef;
            Time<Scale::TDB> tdb;
        };

        State evaluate(StateAndArg const& stateAndArg) const {

            Parameters params{referenceSystemConverter_.GCRS2ITRS(), timeConverter_.convert<Scale::TDB>(stateAndArg.arg)};

            Eigen::Vector<double, 3> const positionECI{stateAndArg.state(0), stateAndArg.state(1), stateAndArg.state(2)};
            Eigen::Vector<double, 3> const velocityECI{stateAndArg.state(3), stateAndArg.state(4), stateAndArg.state(5)};
            Eigen::Vector<double, 3> const acceleration = forceCalculator_.calcAcceleration<Parameters>(positionECI, velocityECI,
                                                                                            forceCalculator_.SatelliteParameters.mass,
                                                                                            forceCalculator_.SatelliteParameters,
                                                                                            params);
            return Eigen::Vector<double, 6>{velocityECI(0), velocityECI(1), velocityECI(2),
                                            acceleration(0), acceleration(1), acceleration(2)};
        }
};