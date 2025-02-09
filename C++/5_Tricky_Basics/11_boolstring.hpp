#pragma once

#include <string>


class BoolString {
    private:
        std::string value;
    public:
        BoolString(std::string const& s) : value(s) {}
        template<typename T = std::string>
        T get() const {return value;}       // get value (converted to T)
};