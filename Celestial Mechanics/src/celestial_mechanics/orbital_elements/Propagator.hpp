#pragma once

#include "Elements.hpp"


KeplerianElements propagate(KeplerianElements const& el, double const& mu, double const& deltaT);