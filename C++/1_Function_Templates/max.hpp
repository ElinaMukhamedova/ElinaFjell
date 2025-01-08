#pragma once

#include <type_traits>

template<typename T>
T max(T a, T b) {
    return b < a ? a : b;
}

template<typename T1, typename T2>
auto max_auto(T1 a, T2 b) {
    return b < a ? a : b;
}

template<typename T1, typename T2>
auto max_decltype(T1 a, T2 b) -> decltype(b<a?a:b) {
    return b < a ? a : b;
}

template<typename T1, typename T2>
auto max_decltype_decay(T1 a, T2 b) -> typename std::decay<decltype(true?a:b)>::type {
    return b < a ? a : b;
}

template<typename T1, typename T2>
std::common_type_t<T1, T2> max_common(T1 a, T2 b) {
    return b < a ? a : b;
}

template<typename T1, typename T2,
            typename RT = std::decay_t<decltype(true ? T1() : T2())>>
RT max_default_decltype(T1 a, T2 b) {
    return b < a ? a : b;
}

template<typename T1, typename T2,
            typename RT = std::common_type_t<T1, T2>>
RT max_default_common(T1 a, T2 b) {
    return b < a ? a : b;
}