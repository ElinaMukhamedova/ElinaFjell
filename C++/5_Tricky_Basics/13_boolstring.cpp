#include <ios>
#include <iostream>
#include "11_boolstring.hpp"
#include "12_boolstringgetbool.hpp"


int main() {
    std::cout << std::boolalpha;
    BoolString s1("hello");
    std::cout << s1.get() << '\n';          // prints hello
    std::cout << s1.get<bool>() << '\n';    // prints false
    BoolString s2("on");
    std::cout << s2.get<bool>() << '\n';    // prints true
}