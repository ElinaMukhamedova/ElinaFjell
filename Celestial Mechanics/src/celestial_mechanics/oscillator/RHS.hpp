#pragma once

#include <Core>
#include <array>


class Oscillator {
    double omega;

    public:
        Oscillator(double const w = 1) : omega(w) {}

        using ArgType = double;
        static constexpr int size = 2;
        using State = Eigen::Vector<double, size>;

        struct StateAndArg {
            State state;
            ArgType arg;
        };

        Eigen::Matrix<double, size, size> f{{0, 1}, {-1, 0}};

        State eval(const StateAndArg& stateAndArg) const;
};