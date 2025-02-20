#include "EarthGravity.hpp"
#include <string>
#include <GeographicLib/GravityModel.hpp>


std::string EarthGravity::path() const {return path_;}

unsigned int EarthGravity::n() const {return n_;}

unsigned int EarthGravity::m() const {return m_;}

EarthGravity::EarthGravity(EarthGravity const& gravity)
    : path_(gravity.path()), n_(gravity.n()), m_(gravity.m()),
        gravityModel_(GeographicLib::GravityModel("egm2008", path_, n_, m_)) {}


double EarthGravity::gravitationalParameter() const {return gravityModel_.MassConstant();}