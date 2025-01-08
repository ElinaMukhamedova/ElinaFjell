#pragma once

#include <vector>
#include <cmath>
#include <math.h>
#include <algorithm>
#include "celestial_mechanics/Exception.hpp"
#include <functional>
#include <iostream>

template<typename xType, typename yType>
class Interpolant {

    std::vector<xType> nodes_;
    std::vector<yType> values_;

    public:
    Interpolant(const std::vector<xType>& nodes, const std::vector<yType>& values) : nodes_(nodes), values_(values) {
        std::size_t nodesSize = nodes.size();
        std::size_t valuesSize = values.size();
        std::size_t N = valuesSize >= nodesSize ? nodesSize : valuesSize;

        if (N < 2)
            throw Exception("Interpolant does not make sense!");

        nodes_.resize(N); values_.resize(N);
        
        auto begin = nodes_.begin(); auto end = nodes_.end();
        auto first_violator = std::adjacent_find(begin, end, std::greater_equal<xType>());

        if (first_violator != end)
            throw Exception("Nodes are not in ascending order!");
    }
    yType evaluate(xType x) const {
        std::size_t N = nodes_.size();
        
        if (x < nodes_[0] || x > nodes_[N - 1])
            throw Exception("Requested point is out of bounds to Interpolant");

        auto iter = std::lower_bound(nodes_.begin(), nodes_.end(), x);
        int index = iter - nodes_.begin();

        const bool condition = index != 0;
        xType a = condition ? nodes_[index - 1] : nodes_[0];
        xType b = condition ? *iter : nodes_[1];
        yType value_a = condition ? values_[index - 1] : values_[0];
        yType value_b = condition ? values_[index] : values_[1];

        double slope = (value_b - value_a) / (b - a);
        return slope * (x - a) + value_a;
    }
};