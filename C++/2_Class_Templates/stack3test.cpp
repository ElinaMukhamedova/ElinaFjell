#include "stack3.hpp"
#include <iostream>
#include <deque>


int main() {
    // stack of ints:
    Stack<int> intStack;

    // stack of doubles using std::deque<> to manage the elements:
    Stack<double, std::deque<double>> dblStack;

    // manipulate int stack
    intStack.push(21);
    std::cout << intStack.top() << '\n';
    intStack.pop();

    // manipulate double stack
    dblStack.push(13.17);
    std::cout << dblStack.top() << '\n';
    dblStack.pop();
}