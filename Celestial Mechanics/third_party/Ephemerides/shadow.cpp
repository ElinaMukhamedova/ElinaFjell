#include <calceph/calceph.h>
#include <Core>
#include <string>
#include <iostream>

static void printcoord(double PV[6], const char *name) {
    printf("%s :\n", name);
    for (int j = 0; j < 6; j++)
        printf("\t%23.16E\n", PV[j]);
    printf("\n");
}

const std::string FILE_PATH = __FILE__;
const std::string DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - 10);

int main() {
    const std::string path = DIR_PATH + "data/de405.bin";
    int res = calceph_sopen(path.c_str());

    if (res) {
        std::cout << "Ephemerides file is opened" << std::endl;
        int timescale = calceph_sgettimescale();
        if (timescale)
            printf("timescale : %s\n", timescale == 1 ? "TDB" : "TCB");
        double jdfirst, jdlast; int cont;
        if (calceph_sgettimespan(&jdfirst, &jdlast, &cont)) {
            printf("data available between [ %f, %f ], continuous=%d\n\n", jdfirst, jdlast, cont);
        }

        double helioEarth[6];
        double jdInt = jdfirst; double jdFrac = 0.5;
        calceph_scompute(jdInt, jdFrac, 3, 11, helioEarth);
        printcoord(helioEarth, "heliocentric coordinates of Earth");

        Eigen::Vector3d SunEarth{helioEarth[0], helioEarth[1], helioEarth[2]};
        std::cout << SunEarth << std::endl;
        
        calceph_sclose();
        std::cout << "Ephemerides file is closed" << std::endl;
    }
    else {
        std::cout << "Can't open the ephemerides file" << std::endl;
    }
}