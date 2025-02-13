#pragma once

#include <vector>


template<typename Table, typename RHS>
std::vector<typename RHS::StateAndArg> integrateRK (
    const typename RHS::State& initialState,
    const typename RHS::ArgType& endTime,
    double step,
    const RHS& rhs) {
        std::vector<typename RHS::StateAndArg> solution;
        typename RHS::ArgType currentTime = 0;
        typename RHS::State currentState = initialState;
        typename RHS::State nextState;
        typename RHS::StateAndArg currentStateAndArg;
        while (currentTime <= endTime) {
            currentStateAndArg.state = currentState;
            currentStateAndArg.arg = currentTime;
            solution.push_back(currentStateAndArg);

            std::array<typename RHS::State, Table::stages> Ks;
            Ks[0] = rhs.eval(currentStateAndArg);
            for (std::size_t i = 1; i < Table::stages; ++i) {
                typename RHS::State K_sum = Table::aTable[i][0] * Ks[0];
                for (std::size_t j = 1; j < i; ++j)
                    K_sum += Table::aTable[i][j] * Ks[j];
                typename RHS::StateAndArg x;
                x.state = currentState + step * K_sum;
                x.arg = currentTime + step * Table::cColumn[i];
                Ks[i] = rhs.eval(x);
            }

            nextState = currentState;
            for (std::size_t i = 0; i < Table::stages; ++i)
                nextState += step * Table::bString[i] * Ks[i];
            
            currentTime += step;
            currentState = nextState;
        }
        return solution;
    }