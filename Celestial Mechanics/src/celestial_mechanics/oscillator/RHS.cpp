#include "RHS.hpp"
#include <Core>

Eigen::Vector<double, Oscillator::size> Oscillator::eval(Oscillator::StateAndArg const& stateAndArg) const {
    return Eigen::Vector<double, size> {Oscillator::f * stateAndArg.state * std::pow(omega, 2)};
}