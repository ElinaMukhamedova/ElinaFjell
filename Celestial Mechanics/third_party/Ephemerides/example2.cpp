//
// Created by fukin on 28.09.22.
//

#include <calceph/calceph.h>
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <iomanip>

const std::string FILE_PATH = __FILE__;
const std::string DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - 12);

/*-----------------------------------------------------------------*/
/* main program */
/*-----------------------------------------------------------------*/
int main(void) {
    const std::string path = DIR_PATH + "data/de405.bin";

    double PV[6];

    /* open the ephemeris file */
    int res = calceph_sopen(path.c_str());

    std::array<int, 10> targetsArray = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11};

    const size_t numOfPlanets = 11;

//    const double jd_init = 2458849;  // 2020-01-01
    const double jd_init = 2415020;  // 1900-01-01
//    const double jd_end = 2459215;  // 2021-01-01
    const double jd_end = 2466154;  // 2040-01-01
    const int step = 10;
    const int center = 12;
    const double dt = 0.5;

    double AU;

    calceph_sgetconstant("AU", &AU);

//    std::ofstream file(DIR_PATH + "results/planets_trajectories.csv");

//    file << "t,Mercury,Venusx,Venusy,Venusz,Earthx,Earthy,Earthz,Marsx,Marsy,Marsz,Moonx,Moony,Moonz\n";

    if (res) {
        printf("The ephemeris is already opened\n");
        for (const auto target: targetsArray) {
            std::ofstream file(DIR_PATH + "results/" + std::to_string(target) + "_trajectories" + ".csv");
            file << "t,x,y,z\n";
            for (int currentTime = static_cast<int>(jd_init); currentTime < jd_end; currentTime += step) {
                calceph_scompute(currentTime, dt, target, center, PV);
                file << std::setprecision(15) << currentTime - jd_init << ',' << PV[0] << ',' << PV[1]
                     << ',' << PV[2] << '\n';
            }
            file.close();
        }

    } else {
        printf("The ephemeris can't be opened\n");
    }
    return res;
}