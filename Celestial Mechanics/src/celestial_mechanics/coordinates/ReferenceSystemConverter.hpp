#pragma once

#include <Core>
#include <Geometry>
#include <sofa.h>
#include "celestial_mechanics/time/Time.hpp"
#include "celestial_mechanics/time/TimeConverter.hpp"
#include <array>
#include "EOPContainer.hpp"
#include <iomanip>
#include <iostream>

class ReferenceSystemConverter {
    EOPContainer EOPcontainer_;
    TimeConverter<EOPContainer> timeConverter_;
    public:
        ReferenceSystemConverter (const EOPContainer& EOPcontainer) : EOPcontainer_(EOPcontainer), timeConverter_(EOPcontainer_) {}
        
        Eigen::Quaternion<double> GCRS2ITRS(const Time<Scale::TT>& tt) const {
            const auto utc = timeConverter_.convert<Scale::UTC>(tt);
            double utcMJD = utc.mjd();

            const auto ut1 = timeConverter_.convert<Scale::UT1>(utc);
            double era = iauEra00(ut1.jdInt(), ut1.jdFrac());

            double tt1 = tt.jdInt(); double tt2 = tt.jdFrac();
            double dX = EOPcontainer_.dX(utcMJD); double dY = EOPcontainer_.dY(utcMJD);
            double X, Y;
            iauXy06(tt1, tt2, &X, &Y);
            double s = iauS06(tt1, tt2, X, Y);
            double rc2i[3][3];
            iauC2ixys(X+dX, Y+dY, s, rc2i);

            double rpom[3][3];
            double xTerr = EOPcontainer_.xTerr(utcMJD); double yTerr = EOPcontainer_.yTerr(utcMJD);
            double sTerr = iauSp00(tt1, tt2);
            iauPom00(xTerr, yTerr, sTerr, rpom);

            double rc2t[3][3];
            iauC2tcio(rc2i, era, rpom, rc2t);

            Eigen::Matrix<double, 3, 3> rc2tMatrix{{rc2t[0][0], rc2t[0][1], rc2t[0][2]},
                                                    {rc2t[1][0], rc2t[1][1], rc2t[1][2]},
                                                    {rc2t[2][0], rc2t[2][1], rc2t[2][2]}};

            return Eigen::Quaternion<double>{rc2tMatrix};
        }
};