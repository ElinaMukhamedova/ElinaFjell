#include "ElementsConverter.hpp"
#include <cmath>
#include <numbers>
#include <Core>


Eigen::Matrix<double, 3, 3> R1(double const& phi) {
    double const cos_phi = std::cos(phi);
    double const sin_phi = std::sin(phi);
    return Eigen::Matrix<double, 3, 3>{{1, 0, 0},
                                        {0, cos_phi, sin_phi},
                                        {0, -sin_phi, cos_phi}};
}

Eigen::Matrix<double, 3, 3> R3(double const& phi) {
    double const cos_phi = std::cos(phi);
    double const sin_phi = std::sin(phi);
    return Eigen::Matrix<double, 3, 3>{{cos_phi, sin_phi, 0},
                                        {-sin_phi, cos_phi, 0},
                                        {0, 0, 1}};
}

double N(double const& x) {return x>=0 ? x : x + 2*std::numbers::pi;}

namespace Orbit {

CartesianElements convertKeplerianToCartesian(KeplerianElements const& el, double const mu) {
    double const inclination = el.inclination;
    double const ascendingNode = el.ascendingNode;
    double const semimajor = el.semimajor;
    double const eccentricity = el.eccentricity;
    double const argumentPeriapsis = el.argumentPeriapsis;
    double const trueAnomaly = el.trueAnomaly;

    double const cos_trueAnomaly = std::cos(trueAnomaly);
    double const sin_trueAnomaly = std::sin(trueAnomaly);

    double const p = semimajor * (1 - eccentricity*eccentricity);
    double const r = p / (1 + eccentricity * cos_trueAnomaly);
    double const coef = std::sqrt(mu / p);

    Eigen::Vector3d r_elli{r * cos_trueAnomaly, r * sin_trueAnomaly, 0};
    Eigen::Vector3d v_elli{-coef * sin_trueAnomaly, coef * (eccentricity + cos_trueAnomaly), 0};
    Eigen::Matrix<double, 3, 3> Q = R3(-ascendingNode) * R1(-inclination) * R3(-argumentPeriapsis);
    
    Eigen::Vector3d position = Q * r_elli;
    Eigen::Vector3d velocity = Q * v_elli;

    return CartesianElements{position, velocity};
}

KeplerianElements convertCartesianToKeplerian(CartesianElements const& el, double const mu) {
    const Eigen::Vector3d& position = el.position;
    const Eigen::Vector3d& velocity = el.velocity;
    Eigen::Vector3d const x{1, 0, 0};
    Eigen::Vector3d const y{0, 1, 0};
    Eigen::Vector3d const z{0, 0, 1};
    double const r = position.norm();
    double const v = velocity.norm();

    Eigen::Vector3d const L = position.cross(velocity);
    Eigen::Vector3d const l1 = z.cross(L);
    Eigen::Vector3d const l2 = l1.norm()==0 ? x : l1.normalized();
    Eigen::Vector3d const l = l2.cross(z);
    double const inclination = std::atan2(L.dot(l), L.dot(z));
    double const ascendingNode = N(std::atan2(l2.dot(y), l2.dot(x)));
    
    double const semimajor = mu / (2*mu/r - v*v);
    Eigen::Vector3d const e = (1/mu) * ((v*v - mu/r) * position - position.dot(velocity) * velocity);
    double const eccentricity = e.norm();

    Eigen::Vector3d const e1 = e.norm()==0 ? l2 : e.normalized();
    Eigen::Vector3d const e2 = L.cross(e1).normalized();
    Eigen::Vector3d const g1 = l2;
    Eigen::Vector3d const g2 = L.cross(g1).normalized();
    double const argumentPeriapsis = N(std::atan2(e1.dot(g2), e1.dot(g1)));
    double const trueAnomaly = N(atan2(position.dot(e2), position.dot(e1)));

    return KeplerianElements{inclination, ascendingNode, semimajor,
                                eccentricity, argumentPeriapsis, trueAnomaly};
}

};