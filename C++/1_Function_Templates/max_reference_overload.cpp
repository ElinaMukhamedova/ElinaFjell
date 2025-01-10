#include <cstring>
#include <iostream>

//maximum of two values of any type (call-by-reference)
template<typename T>
T const& max(T const& a, T const& b) {
    return b < a ? a : b;
}

//maximum of two C-strings (call-by-value)
char const* max(char const* a, char const* b) {
    return std::strcmp(b,a) < 0 ? a : b;
}

//maximum of three values of any type (call-by-reference)
template<typename T>
T const& max(T const& a, T const& b, T const& c) {
    return max(max(a,b), c);        //error if max(a,b) uses call-by-value:
}                                   //max(a,b) creates a new, temporary local value
//that is returned by reference, but that temporary value expires as soon as the
//return statement is complete, leaving main() with a dangling reference 

int main() {
    auto m1 = ::max(7, 42, 68);     //temporaries are created for the arguments
    std::cout << m1 << '\n';        //(7, 42, and 68), but those temporaries are created
    //in main() where they persist until the statement is done

    char const* s1 = "frederic";
    char const* s2 = "anica";
    char const* s3 = "lucas";
    auto m2 = ::max(s1, s2, s3);    //run-time ERROR (undefined behaviour)
    std::cout << m2 << '\n';
}