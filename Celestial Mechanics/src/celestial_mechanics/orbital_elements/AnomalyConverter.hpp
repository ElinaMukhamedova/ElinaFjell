#pragma once


double TrueToEccentric(double const& eccentricity, double const& trueAnomaly);

double EccentricToTrue(double const& eccentricity, double const& eccentricAnomaly);

double EccentricToMean(double const& eccentricity, double const& eccentricAnomaly);

double MeanToEccentric(double const& eccentricity, double const& meanAnomaly);