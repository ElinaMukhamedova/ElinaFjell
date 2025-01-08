#include <iostream>
#include <typeinfo>

#define PRINT_NAME(x) std::cout << #x << " - " << typeid(x).name() << '\n'

template<typename T1, typename T2>
auto max(T1 a, T2 b) {
    return b < a ? a : b;
}

template<typename RT, typename T1, typename T2>
RT max(T1 a, T2 b) {
    return b < a ? a : b;
}

int main() {
    PRINT_NAME(char);
    PRINT_NAME(signed char);
    PRINT_NAME(unsigned char);
    PRINT_NAME(short);
    PRINT_NAME(unsigned short);
    PRINT_NAME(int);
    PRINT_NAME(unsigned int);
    PRINT_NAME(long);
    PRINT_NAME(unsigned long);
    PRINT_NAME(float);
    PRINT_NAME(double);
    PRINT_NAME(long double);
    PRINT_NAME(char*);
    PRINT_NAME(const char*);

    auto a = ::max(4, 7.2);              //uses first template
    std::cout << typeid(a).name() << " " << a << std::endl;
    auto b = ::max<long double>(7.2, 4); //uses second template
    std::cout << typeid(b).name() << " " << b << std::endl;
    //auto c = ::max<int>(4, 7.2);       //ERROR: both function templates match
}