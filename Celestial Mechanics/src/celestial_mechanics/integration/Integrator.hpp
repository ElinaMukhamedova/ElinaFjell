#pragma once

#include <vector>


template<typename Tableau, typename RHS>
std::vector<typename RHS::StateAndArg> integrate (
    typename RHS::State const& initialState,
    typename RHS::ArgType const& endTime,
    double step,
    RHS const& rhs) {

        std::vector<typename RHS::StateAndArg> solution;

        typename RHS::ArgType currentTime = 0;
        typename RHS::State currentState = initialState;
        typename RHS::State nextState;
        typename RHS::StateAndArg currentStateAndArg;

        while (currentTime <= endTime) {

            currentStateAndArg.state = currentState;
            currentStateAndArg.arg = currentTime;
            solution.push_back(currentStateAndArg);

            std::array<typename RHS::State, Tableau::stages> Ks;
            Ks[0] = rhs.evaluate(currentStateAndArg);
            for (std::size_t i = 1; i < Tableau::stages; ++i) {
                typename RHS::State K_sum = Tableau::aTable[i][0] * Ks[0];
                for (std::size_t j = 1; j < i; ++j)
                    K_sum += Tableau::aTable[i][j] * Ks[j];
                typename RHS::StateAndArg x;
                x.state = currentState + step * K_sum;
                x.arg = currentTime + step * Tableau::cColumn[i];
                Ks[i] = rhs.evaluate(x);
            }

            nextState = currentState;
            for (std::size_t i = 0; i < Tableau::stages; ++i)
                nextState += step * Tableau::bString[i] * Ks[i];
            
            currentTime += step;
            currentState = nextState;
        }
        return solution;
    }