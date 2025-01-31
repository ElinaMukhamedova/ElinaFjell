#include<string>
#include<complex>
#include<iostream>
#include"4_addspace.hpp"


template<typename... T>
void printDoubled(T const&... args) {
    print(args + args...);
}

template<typename... T>
void addOne(T const&... args) {
    print(args + 1 ...);
    print((args + 1)...);
}

template<typename T1, typename... TN>
constexpr bool isHomogeneous(T1, TN...) {
    return (std::is_same<T1,TN>::value && ...);     // since C++17
}

int main() {
    print(7.5, std::string("hello"), std::complex<float>(13.21, 17.21));
    printDoubled(7.5, std::string("hello"), std::complex<float>(13.21, 17.21));
    addOne(7.5, 21.13, 21.17);
    std::cout << isHomogeneous(43, -1, "hello") << '\n';
    std::cout << isHomogeneous("hello", " ", "world", "!") << '\n';
}