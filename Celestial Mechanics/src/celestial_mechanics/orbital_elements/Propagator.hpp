#pragma once

#include "Elements.hpp"


namespace Orbit {

KeplerianElements propagate(KeplerianElements const& el, double const mu, double const deltaT);

};