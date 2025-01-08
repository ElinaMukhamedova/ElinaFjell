#include "J2.hpp"
#include <array>
#include <Core>

std::array<double, 2> CentralAndJ2gravity::U(Position const& position) const {
    double const r = position.norm();
    double const r2 = position.squaredNorm();
    double const r3 = r * r2;
    double const& z = position(2);
    double main = CentralAndJ2gravity::mu / r;
    double coeff = CentralAndJ2gravity::alpha / (3 * r3);
    double corr = -coeff * (3 * std::pow(z/r, 2) - 1);
    std::array<double, 2> potential;
    potential[0] = main;
    potential[1] = main + corr;
    return potential;
}

std::array<Eigen::Vector<double, 3>, 2> CentralAndJ2gravity::acceleration(Position const& position) const {
    double const r = position.norm();
    double const r2 = position.squaredNorm();
    double const r3 = r * r2;
    double const r5 = r3 * r2;
    double const r7 = r5 * r2;
    double const& x = position(0);
    double const& y = position(1);
    double const& z = position(2);
    Eigen::Vector3d main = (-CentralAndJ2gravity::mu / r3) * position;
    double const corrX = -CentralAndJ2gravity::alpha * x * (x * x + y * y - 4 * z * z) / r7;
    double const corrY = -CentralAndJ2gravity::alpha * y * (x * x + y * y - 4 * z * z) / r7;
    double const corrZ = CentralAndJ2gravity::alpha * z * (-3 * x * x - 3 * y * y + 2 * z * z) / r7;
    Eigen::Vector3d corr = {corrX, corrY, corrZ};

    std::array<Eigen::Vector3d, 2> acceleration;
    acceleration[0] = main;
    acceleration[1] = main + corr;

    return acceleration;
}

double CentralAndJ2gravity::MajorSemiaxis(Position const& position, Velocity const& velocity) const {
    double r = position.norm();
    double v2 = velocity.squaredNorm();
    double numerator = CentralAndJ2gravity::mu * r;
    double denominator = 2 * CentralAndJ2gravity::mu - r * v2;
    return numerator / denominator;
}