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
    double satelliteMass_;
    double S_srp_;
    double S_drag_;
    double C_drag_;
    TimeConverter<DutContainer> timeConverter_;
    ReferenceSystemConverter referenceSystemConverter_;
    ForceCalculator<EarthGrav, OtherForces...> forceCalculator_;

    public:
        Satellite(double const satelliteMass, double const S_srp, double const S_drag, double const C_drag,
                    DutContainer const& dutContainer, EOPContainer const& EOPcontainer,
                        EarthGrav const& gravity, OtherForces const& ... forces)
            : satelliteMass_(satelliteMass), S_srp_(S_srp), S_drag_(S_drag), C_drag_(C_drag),
                timeConverter_(dutContainer),referenceSystemConverter_(EOPcontainer),
                    forceCalculator_(gravity, forces...) {}

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

            typename ForceCalculator<EarthGrav, OtherForces...>::SatelliteParameters satelliteParameters;
            satelliteParameters.mass = satelliteMass_;
            satelliteParameters.S_srp = S_srp_;
            satelliteParameters.S_drag = S_drag_;
            satelliteParameters.C_drag = C_drag_;
            

            Eigen::Vector<double, 3> const positionECI{stateAndArg.state(0), stateAndArg.state(1), stateAndArg.state(2)};
            Eigen::Vector<double, 3> const velocityECI{stateAndArg.state(3), stateAndArg.state(4), stateAndArg.state(5)};
            Eigen::Vector<double, 3> const acceleration = forceCalculator_.template calcAcceleration<Parameters>(positionECI, velocityECI,
                                                                                            satelliteMass_,
                                                                                            satelliteParameters,
                                                                                            params);
            return Eigen::Vector<double, 6>{velocityECI(0), velocityECI(1), velocityECI(2),
                                            acceleration(0), acceleration(1), acceleration(2)};
        }
};