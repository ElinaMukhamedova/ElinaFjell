#pragma once

#include <sofa.h>
#include "Time.hpp"
#include "celestial_mechanics/Exception.hpp"
#include <type_traits>

template <typename SomeContainer>
class TimeConverter {
    SomeContainer dutContainer_;
    public:
        TimeConverter (const SomeContainer& dutContainer) : dutContainer_(dutContainer) {}
        template<Scale To, Scale From> Time<To> convert(const Time<From>& from) const;

        template<> Time<Scale::UT1> convert<Scale::UT1, Scale::UTC>(const Time<Scale::UTC>& from) const {
            double ut11, ut12;
            double dut = dutContainer_.dut(from.mjd());
            int j = iauUtcut1(from.jdInt(), from.jdFrac(), dut, &ut11, &ut12);
            switch (j) {
                case -1:
                    throw Exception("unacceptable date");
                default:
                    return Time<Scale::UT1>(ut11, ut12);
            }
        }

        template<> Time<Scale::UTC> convert<Scale::UTC, Scale::UT1>(const Time<Scale::UT1>& from) const {
            const auto ut1_jdInt = from.jdInt();
            const auto ut1_jdFrac = from.jdFrac();
            
            const auto utc_0 = Time<Scale::UTC>(ut1_jdInt, ut1_jdFrac);
            const auto dut_0 = dutContainer_.dut(utc_0.mjd());

            double quasi1_1, quasi2_1;
            int j = iauUt1utc(ut1_jdInt, ut1_jdFrac, dut_0, &quasi1_1, &quasi2_1);
            if (j == -1)
                throw Exception("unacceptable date");
            const auto utc_1 = Time<Scale::UTC>(quasi1_1, quasi2_1);

            double quasi1_2, quasi2_2;
            const auto dut_1 = dutContainer_.dut(utc_1.mjd());
            j = iauUt1utc(ut1_jdInt, ut1_jdFrac, dut_1, &quasi1_2, &quasi2_2);
            if (j == -1)
                throw Exception("unacceptable date");
            const auto utc_2 = Time<Scale::UTC>(quasi1_2, quasi2_2);

            double quasi1_3, quasi2_3;
            const auto dut_2 = dutContainer_.dut(utc_2.mjd());
            j = iauUt1utc(ut1_jdInt, ut1_jdFrac, dut_2, &quasi1_3, &quasi2_3);
            if (j == -1)
                throw Exception("unacceptable date");
            const auto utc_3 = Time<Scale::UTC>(quasi1_3, quasi2_3);

            return utc_3;
        }

        template<> Time<Scale::TAI> convert<Scale::TAI, Scale::UTC>(const Time<Scale::UTC>& from) const {
            double tai1, tai2;
            int j = iauUtctai(from.jdInt(), from.jdFrac(), &tai1, &tai2);
            if (j == -1)
                throw Exception("unacceptable date");
            return Time<Scale::TAI>(tai1, tai2);
        }

        template<> Time<Scale::UTC> convert<Scale::UTC, Scale::TAI>(const Time<Scale::TAI>& from) const {
            double utc1, utc2;
            int j = iauTaiutc(from.jdInt(), from.jdFrac(), &utc1, &utc2);
            if (j == -1)
                throw Exception("unacceptable date");
            return Time<Scale::UTC>(utc1, utc2);
        }

        template<> Time<Scale::UT1> convert<Scale::UT1, Scale::TAI>(const Time<Scale::TAI>& from) const {
            const auto utc = convert<Scale::UTC>(from);
            return convert<Scale::UT1>(utc);
        }

        template<> Time<Scale::TAI> convert<Scale::TAI, Scale::UT1>(const Time<Scale::UT1>& from) const {
            const auto utc = convert<Scale::UTC>(from);
            return convert<Scale::TAI>(utc);
        }

        template<> Time<Scale::TT> convert<Scale::TT, Scale::TAI>(const Time<Scale::TAI>& from) const{
            double tt1, tt2;
            int j = iauTaitt(from.jdInt(), from.jdFrac(), &tt1, &tt2);
            return Time<Scale::TT>(tt1, tt2);
        }

        template<> Time<Scale::TAI> convert<Scale::TAI, Scale::TT>(const Time<Scale::TT>& from) const {
            double tai1, tai2;
            int j = iauTttai(from.jdInt(), from.jdFrac(), &tai1, &tai2);
            return Time<Scale::TAI>(tai1, tai2);
        }

        template<> Time<Scale::TT> convert<Scale::TT, Scale::UT1>(const Time<Scale::UT1>& from) const {
            const auto tai = convert<Scale::TAI>(from);
            return convert<Scale::TT>(tai);
        }

        template<> Time<Scale::UT1> convert<Scale::UT1, Scale::TT>(const Time<Scale::TT>& from) const {
            const auto tai = convert<Scale::TAI>(from);
            return convert<Scale::UT1>(tai);
        }

        template<> Time<Scale::TT> convert<Scale::TT, Scale::UTC>(const Time<Scale::UTC>& from) const {
            const auto tai = convert<Scale::TAI>(from);
            return convert<Scale::TT>(tai);
        }

        template<> Time<Scale::UTC> convert<Scale::UTC, Scale::TT>(const Time<Scale::TT>& from) const {
            const auto tai = convert<Scale::TAI>(from);
            return convert<Scale::UTC>(tai);
        }

        template<> Time<Scale::TCG> convert<Scale::TCG, Scale::TT>(const Time<Scale::TT>& from) const {
            double tcg1, tcg2;
            int j = iauTttcg(from.jdInt(), from.jdFrac(), &tcg1, &tcg2);
            return Time<Scale::TCG>(tcg1, tcg2);
        }

        template<> Time<Scale::TT> convert<Scale::TT, Scale::TCG>(const Time<Scale::TCG>& from) const {
            double tt1, tt2;
            int j = iauTcgtt(from.jdInt(), from.jdFrac(), &tt1, &tt2);
            return Time<Scale::TT>(tt1, tt2);
        }

        template<> Time<Scale::TCG> convert<Scale::TCG, Scale::UT1>(const Time<Scale::UT1>& from) const {
            const auto tt = convert<Scale::TT>(from);
            return convert<Scale::TCG>(tt);
        }

        template<> Time<Scale::UT1> convert<Scale::UT1, Scale::TCG>(const Time<Scale::TCG>& from) const {
            const auto tt = convert<Scale::TT>(from);
            return convert<Scale::UT1>(tt);
        }

        template<> Time<Scale::TCG> convert<Scale::TCG, Scale::UTC>(const Time<Scale::UTC>& from) const {
            const auto tt = convert<Scale::TT>(from);
            return convert<Scale::TCG>(tt);
        }

        template<> Time<Scale::UTC> convert<Scale::UTC, Scale::TCG>(const Time<Scale::TCG>& from) const {
            const auto tt = convert<Scale::TT>(from);
            return convert<Scale::UTC>(tt);
        }

        template<> Time<Scale::TCG> convert<Scale::TCG, Scale::TAI>(const Time<Scale::TAI>& from) const {
            const auto tt = convert<Scale::TT>(from);
            return convert<Scale::TCG>(tt);
        }

        template<> Time<Scale::TAI> convert<Scale::TAI, Scale::TCG>(const Time<Scale::TCG>& from) const {
            const auto tt = convert<Scale::TT>(from);
            return convert<Scale::TAI>(tt);
        }

        template<> Time<Scale::TDB> convert<Scale::TDB, Scale::TT>(const Time<Scale::TT>& from) const {
            double g = 6.24 + 0.017202 * (from.jd() - 2451545);
            double dtr = 0.001657 * std::sin(g);
            double tdb1, tdb2;
            int j = iauTttdb(from.jdInt(), from.jdFrac(), dtr, &tdb1, &tdb2);
            return Time<Scale::TDB>(tdb1, tdb2);
        }

        template<> Time<Scale::TT> convert<Scale::TT, Scale::TDB>(const Time<Scale::TDB>& from) const {
            double tdb_int = from.jdInt();
            double tdb_frac = from.jdFrac();
            
            const auto tt0 = Time<Scale::TT>(tdb_int, tdb_frac);
            
            double g0 = 6.24 + 0.017202 * (tt0.jd() - 2451545);
            double dtr0 = 0.001657 * std::sin(g0);

            double tt1_1, tt1_2;
            int j1 = iauTdbtt(tdb_int, tdb_frac, dtr0, &tt1_1, &tt1_2);
            const auto tt1 = Time<Scale::TT>(tt1_1, tt1_2);

            double g1 = 6.24 + 0.017202 * (tt1.jd() - 2451545);
            double dtr1 = 0.001657 * std::sin(g1);

            double tt2_1, tt2_2;
            int j2 = iauTdbtt(tdb_int, tdb_frac, dtr1, &tt2_1, &tt2_2);
            const auto tt2 = Time<Scale::TT>(tt2_1, tt2_2);

            double g2 = 6.24 + 0.017202 * (tt2.jd() - 2451545);
            double dtr2 = 0.001657 * std::sin(g2);

            double tt3_1, tt3_2;
            int j3 = iauTdbtt(tdb_int, tdb_frac, dtr2, &tt3_1, &tt3_2);
            const auto tt3 = Time<Scale::TT>(tt3_1, tt3_2);

            return tt3;
        }

        template<> Time<Scale::TDB> convert<Scale::TDB, Scale::UTC>(const Time<Scale::UTC>& from) const {
            const auto tt = convert<Scale::TT>(from);
            return convert<Scale::TDB>(tt);
        }
};