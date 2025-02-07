#pragma once

#include <Core>


struct KeplerianElements {

    double inclination;
    double ascendingNode;

    double semimajor;
    double eccentricity;

    double argumentPeriapsis;
    double trueAnomaly;
};

struct CartesianElements {

    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
};