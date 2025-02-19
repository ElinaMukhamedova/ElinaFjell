#pragma once

#include <tuple>
#include <Core>
#include <iostream>

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

        template<typename Params>
        Eigen::Vector3d calcAcceleration(const Eigen::Vector3d& positionECI, const Eigen::Vector3d& velocityECI,
                                            const double mass, const SatelliteParameters& satParams,
                                                const Params& params) const {

            const auto sum = [this, &positionECI, &velocityECI, &mass, &satParams, &params]
                (const auto& ... forces) {
                    if constexpr (std::tuple_size_v<std::tuple<OtherForces...>> != 0) {
                        Eigen::Vector3d const forcesAcc = (... + forces.template calcAcceleration<Params>(positionECI, velocityECI, mass, satParams, params));
                        return forcesAcc;
                    } else {return Eigen::Vector3d::Zero();}
                };
            Eigen::Vector3d const gravityAcc = EarthGravity_.calcAcceleration(positionECI, velocityECI, mass, satParams, params);
            Eigen::Vector3d const otherForcesAcc = std::apply(sum, forces_);
            Eigen::Vector3d const acceleration = gravityAcc + otherForcesAcc;
            std::cout << gravityAcc << '\n';
            std::cout << '\n';
            std::cout << otherForcesAcc << '\n';
            std::cout << '\n';
            std::cout << acceleration << '\n';
            std::cout << '\n';
            std::cout << '\n';
            std::cout << '\n';
            return acceleration;
        }
};