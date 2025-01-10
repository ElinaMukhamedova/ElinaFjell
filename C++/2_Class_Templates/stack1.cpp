#include "stack1.hpp"
#include <iostream>
#include <string>

/*void foo(Stack<int> const& s) {     //parameter s is int stack
    using IntStack = Stack<int>;    //IntStack is another name for Stack<int>
    Stack<int> istack[10];          //istack is array of 10 int stacks
    IntStack istack2[10];           //istack2 is also an array of 10 int stacks (same type)
    ...
}*/

int main() {
    Stack<int> intStack;             //stack of ints
    Stack<std::string> stringStack;  //stack of strings

    //manipulate int stack
    intStack.push(7);
    std::cout << intStack.top() << '\n';

    //manipulate string stack
    stringStack.push("hello");
    std::cout << stringStack.top() << '\n';
    stringStack.pop();
}