#include <iostream>


template<auto T>        // take value of any possible nontype parameter (since C++17)
class Message {
    public:
        void print() {
            std::cout << T << '\n';
        }
};

/*
template<decltype(auto) N>      // allows instantiation of N as a reference
class C {
    ...
};

int i;
C<(i)> x;                       // N is int&
*/

int main() {
    Message<131721> msg1;   // initialise with int 42
    msg1.print();       // and print that value

    static char const s[] = "hello";
    Message<s> msg2;    // initialise with char const[6] "hello"
    msg2.print();       // and print that value
}