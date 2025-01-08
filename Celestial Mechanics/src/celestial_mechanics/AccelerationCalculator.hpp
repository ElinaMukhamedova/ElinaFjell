#pragma once

#include <tuple>
#include <Core>

template<typename EarthGrav, typename ... OtherForces>
class ForceCalculator{
    EarthGrav EarthGravity_;
    std::tuple<OtherForces...> forces_;

    public:
        ForceCalculator(const EarthGrav& EarthGravity, const OtherForces& ... forces)
            : EarthGravity_(EarthGravity), forces_(forces ...) {}

        struct SatelliteParameters : EarthGrav::SatelliteParameters,
                                        OtherForces::SatelliteParameters ... {
            double mass;
        };

        Eigen::Vector3d calcAcceleration(const Eigen::Vector3d& positionECI, const Eigen::Vector3d& velocityECI,
                                            const double mass, const SatelliteParameters& satParams,
                                                const Params& params){

            const auto sum = [&positionECI, &velocityECI, &mass, &satParams, &params]
                (const auto& ... forces) {
                    if constexpr (std::tuple_size_v<forces> != 0) {
                        return (forces.calcAcceleration(positionECI, velocityECI, mass, satParams, params) + ...);
                    } else {return Eigen::Vector3d::Zero();}
                };
            
            return EarthGravity_.calcAcceleration(positionECI, velocityECI, mass, satParams, params) + std::apply(sum, forces_);
        }
};