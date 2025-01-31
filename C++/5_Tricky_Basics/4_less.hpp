#pragma once

// if the template parameters are declared as references, the arguments don't decay
// only when passing the argument by value, the types decay,
// so that string literals are converted to type char const*

// you can provide templates that specifically deal with raw arrays or string literals
template<typename T, int N, int M>
bool less(T(&a)[N], T(&b)[M]) {
    for (int i = 0; i<N && i<M; ++i) {
        if (a[i]<b[i]) return true;
        if (b[i]<a[i]) return false;
    }
    return N < M;
}
/*
if you only want to provide a function template for string literals
(and other char arrays), you can do this as follows:

template<int N, int M>
bool less(char const(&a)[N], char const(&b)[M]) {
    for (int i = 0; i<N && i<M; ++i) {
        if (a[i]<b[i]) return true;
        if (b[i]<a[i]) return false;
    }
    return N<M;
}
*/