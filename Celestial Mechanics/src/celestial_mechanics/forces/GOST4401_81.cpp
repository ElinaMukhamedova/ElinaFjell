#include "GOST4401_81.hpp"

double GOST4401_81::density(double height) const{
    return interpolant_.evaluate(height);
}