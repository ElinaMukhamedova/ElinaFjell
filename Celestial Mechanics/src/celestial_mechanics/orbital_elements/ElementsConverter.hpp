#pragma once

#include "Elements.hpp"


CartesianElements convertKeplerianToCartesian(KeplerianElements const& el, double const mu);

KeplerianElements convertCartesianToKeplerian(CartesianElements const& el, double const mu);