#pragma once

#include <array>
#include <vector>
#include <Core>
#include "explicitRK.hpp"
#include <celestial_mechanics/Exception.hpp>
#include <iostream>

struct BDF4 {
    static constexpr unsigned int size = 4;
    static constexpr std::array<double, size> alpha = {(double)11/6, -3, 1.5, (double)-1/3};
    static constexpr std::array<double, size> beta = {(double)1/3, 0.5, -1, (double)1/6};
};

struct BDF5 {
    static constexpr unsigned int size = 5;
    static constexpr std::array<double, size> alpha = {(double)25/12, -4, 3, (double)-4/3, 0.25};
    static constexpr std::array<double, size> beta = {0.25, (double)5/6, -1.5, 0.5, (double)1/12};
};

struct IntegrationParameters {
    double step;
    double epsilon;
    double maxIter;

    IntegrationParameters(double h, double delta, double N) : step(h), epsilon(delta), maxIter(N) {}
};

template<typename BDF, typename RHS, typename RKTable>
std::vector<typename RHS::StateAndArg> integrateBDF(
    const typename RHS::State& initialState,
    const typename RHS::Argument& endTime,
    const IntegrationParameters& parameters,
    const RHS& rhs){

        auto step = parameters.step;

        std::vector<typename RHS::StateAndArg> solution;

        typename RHS::Argument currentTime = -step;
        typename RHS::StateAndArg currentStateAndArg;
        
        auto SpeedUpTime = step * (BDF::size - 2);
        auto base = integrateRK<RKTable, RHS>(initialState, SpeedUpTime, step, rhs);

        for (unsigned int i = 0; i < BDF::size - 1; ++i) {
            solution.push_back(base[i]);
            currentTime += step;
        }
        currentStateAndArg.state = base[BDF::size - 2].state;
        currentStateAndArg.arg = currentTime;

        typename RHS::State pastSum;
        typename RHS::State previousApproximation;
        typename RHS::State currentApproximation;
        typename RHS::StateAndArg StateAndArgApproximation;

        unsigned int index = BDF::size - 2;

        while (currentTime <= endTime) {

            for (unsigned int i = 0; i < BDF::size - 1; ++i)
                if (i > 0)
                    pastSum += BDF::beta[1 + i] * solution[index - i].state;
                else
                    pastSum = BDF::beta[1] * solution[index].state;

            //std::cout << rhs.calc(currentStateAndArg) << std::endl;
            //std::cout << pastSum << std::endl;

            currentTime += step;
            unsigned int nIter = 0;
            previousApproximation = (step * rhs.calc(currentStateAndArg) - pastSum) / BDF::beta[0];
            StateAndArgApproximation.state = previousApproximation;
            StateAndArgApproximation.arg = currentTime;
            currentApproximation = (step/BDF::alpha[0]) * rhs.calc(StateAndArgApproximation) - pastSum/BDF::alpha[0];
            while (rhs.calcDif(currentApproximation, previousApproximation) >= parameters.epsilon) {
                nIter += 1;
                if (nIter > parameters.maxIter)
                    throw Exception("Iteration method failed to converge!");
                previousApproximation = currentApproximation;
                StateAndArgApproximation.state = previousApproximation;
                StateAndArgApproximation.arg = currentTime;
                for (unsigned int i = 0; i < BDF::size - 1; ++i)
                    if (i > 0)
                        pastSum += BDF::alpha[1 + i] * solution[index - i].state;
                    else
                        pastSum = BDF::alpha[1] * solution[index].state;
                currentApproximation = (step/BDF::alpha[0]) * rhs.calc(StateAndArgApproximation) - pastSum/BDF::alpha[0];
            }

            currentStateAndArg.state = currentApproximation;
            currentStateAndArg.arg = currentTime;
            solution.push_back(currentStateAndArg);

            index += 1;
        }

        return solution;
}