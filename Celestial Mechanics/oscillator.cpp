#include <celestial_mechanics/oscillator/RKTable.hpp>
#include <celestial_mechanics/oscillator/RHS.hpp>
#include <celestial_mechanics/oscillator/Integrator.hpp>
#include <iostream>


typename Oscillator::State true_oscillator(typename Oscillator::State initialState, typename Oscillator::ArgType t) {
    typename Oscillator::State state;
    state[0] = initialState[0] * std::cos(t) + initialState[1] * std::sin(t);
    state[1] = -initialState[0] * std::sin(t) + initialState[1] * std::cos(t);
    return state;
}

int main() {
    Oscillator oscillator;
    auto initialState =typename Oscillator::State {0, 1};
    auto endTime = typename Oscillator::ArgType {5};
    std::vector<std::array<double, 2>> logSteplogError;

    std::array<double, 10> step_choice = {1};
    unsigned int n = step_choice.size();
    for (unsigned int i = 1; i < n; ++i)
        step_choice[i] = step_choice[i - 1] / 2;

    std::cout << "logStep" << std::endl;
    for (double step : step_choice) {
        auto integrated = integrateRK<RK4Table, Oscillator>(initialState, endTime, step, oscillator);
        
        double max_error = 0;
        for (auto StateAndArg : integrated) {
            auto delta_state = StateAndArg.state - true_oscillator(initialState, StateAndArg.arg);
            double error = delta_state.norm();
            if (error > max_error)
                max_error = error;
        }
        
        double logStep = std::log2(step);
        std::cout << logStep << ", ";
        logSteplogError.push_back(std::array<double, 2>{logStep, std::log2(max_error)});
    }
    
    std::cout << std::endl;
    std::cout << "logError" << std::endl;
    for (unsigned int i = 0; i < n; ++i)
        std::cout << logSteplogError[i][1] << ", ";

    return 0;
}