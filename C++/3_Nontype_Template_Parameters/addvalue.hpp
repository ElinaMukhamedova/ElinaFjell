#pragma once

// these kinds of functions can be useful if
// functions or operations are used as parameters
template<int Val, typename T>
T addvalue(T x) {
    return x + Val;
}

/*
template<auto Val, typename T = decltype(Val)>
T foo();

template<typename T, T Val = T{}>
T bar();
*/