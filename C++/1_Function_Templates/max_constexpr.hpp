#pragma once


// to be able to use the maximum function at compile time
// (in places with compile-time context),
// declare it as follows:
template<typename T1, typename T2>
constexpr auto max(T1 a, T2 b) {
    return b < a ? a : b;
}