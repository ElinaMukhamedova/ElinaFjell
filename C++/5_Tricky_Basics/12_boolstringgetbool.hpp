#pragma once

#include "11_boolstring.hpp"


// full specialisation for BoolString::get<>() for bool
template<>
inline bool BoolString::get<bool>() const {
    return value == "true" || value == "1" || value == "on";
}