#include <string>
#include <vector>
#include "4_addspace.hpp"


template<typename C, typename... Idx>
void printElems(C const& coll, Idx... idx) {
    print(coll[idx]...);
}

// one can also declare nontype template parameters
// to be parameter packs:
template<std::size_t... Idx, typename C>
void printIdx(C const& coll) {
    print(coll[Idx]...);
}

int main() {
    std::vector<std::string> coll = {"Kazakhstan", "Astana", "Norge", "Oslo"};
    printElems(coll, 1, 0, 3, 2);
    std::vector<std::string> where = {"Astana", "Garden", "View"};
    printIdx<1,2,0>(where);
}