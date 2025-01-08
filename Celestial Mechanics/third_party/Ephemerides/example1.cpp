//
// Created by fukin on 28.09.22.
//

#include <calceph/calceph.h>
#include <string>
#include <iostream>

static void printcoord(double PV[6], const char *name);

int main(void);

/*-----------------------------------------------------------------*/
/* print coordinates */
/*-----------------------------------------------------------------*/
static void printcoord(double PV[6], const char *name) {
    int j;

    printf("%s :\n", name);
    for (j = 0; j < 6; j++)
        printf("\t%23.16E\n", PV[j]);
    printf("\n");
}

const std::string FILE_PATH = __FILE__;
const std::string DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - 12);

/*-----------------------------------------------------------------*/
/* main program */
/*-----------------------------------------------------------------*/
int main(void) {
    const std::string path = DIR_PATH + "data/de405.bin";

    int res;

    double AU, EMRAT, GM_Mer;

    double jd0 = 2442457;

    double dt = 0.5E0;

    double PV[6];

    int timescale;

    int j;

    double valueconstant;

    char nameconstant[CALCEPH_MAX_CONSTANTNAME];

    double jdfirst, jdlast;

    int cont;

    /* open the ephemeris file */
    res = calceph_sopen(path.c_str());
    if (res) {
        printf("The ephemeris is already opened\n");
        /* get the bound time span */
        timescale = calceph_sgettimescale();
        if (timescale)
            printf("timescale : %s\n", timescale == 1 ? "TDB" : "TCB");
        if (calceph_sgettimespan(&jdfirst, &jdlast, &cont)) {
            printf("data available between [ %f, %f ]. continuous=%d\n\n", jdfirst, jdlast, cont);
        }
        /* print the values of AU, EMRAT and GM_Mer */
        if (calceph_sgetconstant("AU", &AU))
            printf("AU=%23.16E\n", AU);
        if (calceph_sgetconstant("EMRAT", &EMRAT))
            printf("EMRAT=%23.16E\n", EMRAT);
        if (calceph_sgetconstant("GM_Mer", &GM_Mer))
            printf("GM_Mer=%23.16E\n", GM_Mer);

        /* compute and print the coordinates */
        /* the geocentric moon coordinates */
        calceph_scompute(jd0, dt, 10, 3, PV);
        printcoord(PV, "geocentric coordinates of the Moon");

        /* the value TT-TDB */
        if (calceph_scompute(jd0, dt, 16, 0, PV)) {
            printf("TT-TDB = %23.16E\n", PV[0]);
        }
        printf("mars\n");
        /* the heliocentric coordinates of Mars */
        calceph_scompute(jd0, dt, 4, 11, PV);
        printcoord(PV, "heliocentric coordinates of Mars");

        /* print the list of the constants */
        printf("list of constants\n");
        for (j = 1; j <= calceph_sgetconstantcount(); j++) {
            calceph_sgetconstantindex(j, nameconstant, &valueconstant);
            printf("'%s'\t= %23.16E\n", nameconstant, valueconstant);
        }

        /* close the ephemeris file */
        calceph_sclose();
        printf("The ephemeris is already closed\n");
    } else {
        printf("The ephemeris can't be opened\n");
    }
    return res;
}

