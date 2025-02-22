#include "Elements.hpp"


double KeplerianElements::focalParameter() const {return  semimajor*(1-eccentricity*eccentricity);}

double KeplerianElements::argumentLatitude() const {return argumentPeriapsis+trueAnomaly;}
