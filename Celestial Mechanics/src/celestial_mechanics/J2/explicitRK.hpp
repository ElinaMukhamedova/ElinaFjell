#pragma once

#include <array>
#include <vector>
#include <cmath>
#include <type_traits>
#include <Core>

struct RK4Table {
    static constexpr unsigned int stages = 4;
    static constexpr std::array<std::array<double, stages>, stages>
        aTable = {{ {0, 0, 0, 0}, {0.5, 0, 0, 0}, {0, 0.5, 0, 0}, {0, 0, 1, 0} }};
    static constexpr std::array<double, stages> cColumn = {0, 0.5, 0.5, 1};
    static constexpr std::array<double, stages> bString = {(double)1/6, (double)1/3, (double)1/3, (double)1/6};
};

template<typename Table, typename RHS>
std::vector<typename RHS::StateAndArg> integrateRK (
    const typename RHS::State& initialState,
    const typename RHS::Argument& endTime,
    double step,
    const RHS& rhs) {
        std::vector<typename RHS::StateAndArg> solution;
        typename RHS::Argument currentTime = 0;
        typename RHS::State currentState = initialState;
        typename RHS::State nextState;
        typename RHS::StateAndArg currentStateAndArg;
        while (currentTime <= endTime) {
            currentStateAndArg.state = currentState;
            currentStateAndArg.arg = currentTime;
            solution.push_back(currentStateAndArg);

            std::array<typename RHS::State, Table::stages> Ks;
            Ks[0] = rhs.calc(currentStateAndArg);
            for (std::size_t i = 1; i < Table::stages; ++i) {
                typename RHS::State K_sum = Table::aTable[i][0] * Ks[0];
                for (std::size_t j = 1; j < i; ++j)
                    K_sum += Table::aTable[i][j] * Ks[j];
                typename RHS::StateAndArg x;
                x.state = currentState + step * K_sum;
                x.arg = currentTime + step * Table::cColumn[i];
                Ks[i] = rhs.calc(x);
            }

            nextState = currentState;
            for (std::size_t i = 0; i < Table::stages; ++i)
                nextState += step * Table::bString[i] * Ks[i];
            
            currentTime += step;
            currentState = nextState;
        }
        return solution;
    }