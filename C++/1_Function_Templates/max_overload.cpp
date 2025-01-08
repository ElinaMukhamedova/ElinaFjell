#include <iostream>
#include <string>

int max(int a, int b) {
    std::cout << "nontemplate function ";
    return b < a ? a : b;
}

template<typename T>
T max(T a, T b) {
    std::cout << "template function ";
    return b < a ? a : b;
}

int main() {
    std::cout << ::max(7, 42) << std::endl;         //calls the nontemplate for two ints
    std::cout << ::max(7.0, 42.0) << std::endl;     //calls max<double> (by argument deduction)
    std::cout << ::max('a', 'b') << std::endl;      //calls max<char> (by argument deduction)
    std::cout << ::max<>(7, 42) << std::endl;       //calls max<int> (by argument deduction)
    std::cout << ::max<double>(7, 42) << std::endl; //calls max<double> (no argument deduction)
    std::cout << ::max('a', 42.7) << std::endl;     //calls the nontemplate for two ints
    std::cout << ::max<int>('a', 42.7) << std::endl;
    std::cout << ::max<double>('a', 1317.21) << std::endl;
}