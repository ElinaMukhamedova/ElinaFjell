#pragma once

#include <exception>

class Exception : public std::exception {
    const char * error_;

    public:
        Exception(const char * error) : error_(error){}
        const char * what();
};