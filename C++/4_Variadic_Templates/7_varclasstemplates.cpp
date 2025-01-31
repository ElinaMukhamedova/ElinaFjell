#include "4_addspace.hpp"
#include <array>
#include <tuple>


// type for arbitrary number of indices:
template<std::size_t...>
struct Indices{};

template<typename T, std::size_t... Idx>
void printByIdx(T t, Indices<Idx...>) {
    print(std::get<Idx>(t)...);
}

int main() {
    std::array<std::string, 3> arr = {"programming", "creativity", "freedom"};
    printByIdx(arr, Indices<1,0,2>());

    auto t = std::make_tuple("Astana", 13, 17, 21, "Garden View");
    printByIdx(t, Indices<3,2,1,4,0>());
}