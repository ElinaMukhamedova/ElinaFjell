#pragma once

#include <Core>
#include <array>

class CentralAndJ2gravity {
    public:
        static constexpr double mu = 3986e+11;
        static constexpr double J2 = 1.082e-3;
        static constexpr double R = 6378137;
        static constexpr double alpha = 1.5 * mu * R * R * J2;

        using Position = Eigen::Vector3d;
        using Velocity = Eigen::Vector3d;

        std::array<double, 2> U(Position const& position) const;
        std::array<Eigen::Vector3d, 2> acceleration(Position const& position) const;
        double MajorSemiaxis(Position const& position, Velocity const& velocity) const;
};

template<typename Gravity, int Precision>
class GravityRHS {

    Gravity gravity;

    public:
        static constexpr unsigned int dim = 6;
        using Argument = double;
        using Position = Eigen::Vector3d;
        using Velocity = Eigen::Vector3d;
        using Acceleration = Eigen::Vector3d;
        
        using State = Eigen::Vector<double, 6>;
        struct StateAndArg {
            State state;
            Argument arg;
        };

        double calcDif(const State& first, const State& second) const {
            return (first - second).norm();
        }

        Eigen::Vector<double, 6> calc(StateAndArg const& stateAndTime) const {
            Position position = {stateAndTime.state(0), stateAndTime.state(1), stateAndTime.state(2)};
            Velocity velocity = {stateAndTime.state(3), stateAndTime.state(4), stateAndTime.state(5)};
            Acceleration acceleration = gravity.acceleration(position)[Precision-1];
            Eigen::Vector<double, 6> rhs;
            rhs << velocity, acceleration;
            return rhs;
        }
};