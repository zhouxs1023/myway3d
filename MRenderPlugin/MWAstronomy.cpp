#include "MWAstronomy.h"
#include <math.h>

namespace Myway {

    const double Astronomy::PI = 3.1415926535897932384626433832795029L;

    const double Astronomy::J2000 = 2451545.0;

    double Astronomy::radToDeg (double value)
    {
        return value * 180 / PI;
    }

    double Astronomy::degToRad (double value)
    {
        return value * PI / 180;
    }

    double Astronomy::sinDeg (double x) {
        return sin (degToRad (x));
    }

    double Astronomy::cosDeg (double x) {
        return cos (degToRad (x));
    }

    double Astronomy::atan2Deg (double y, double x) {
        return radToDeg(atan2 (y, x));
    }

    double Astronomy::normalizeDegrees (double value)
    {
        value = fmod (value, 360);
        if (value < double (0)) {
            value += double (360);
        }
        return value;
    }

    void Astronomy::convertEclipticToEquatorialRad (
        double lon, double lat,
        double &rasc, double &decl)
    {
        double ecl = Astronomy::degToRad(23.439281);

        double x = cos(lon) * cos(lat);
        double y = cos(ecl) * sin(lon) * cos(lat) - sin(ecl) * sin(lat);
        double z = sin(ecl) * sin(lon) * cos(lat) + cos(ecl) * sin(lat);

        double r = sqrt(x * x + y * y);
        rasc = atan2(y, x);
        decl = atan2(z, r);
    } 

    void Astronomy::convertRectangularToSpherical (
        double x, double y, double z,
        double &rasc, double &decl, double &dist)
    {
        dist = sqrt (x * x + y * y + z * z);
        rasc = atan2Deg (y, x);
        decl = atan2Deg (z, sqrt (x * x + y * y));
    }

    void Astronomy::convertSphericalToRectangular (
        double rasc, double decl, double dist,
        double &x, double &y, double &z)
    {
        x = dist * cosDeg (rasc) * cosDeg (decl);
        y = dist * sinDeg (rasc) * cosDeg (decl);
        z = dist * sinDeg (decl);
    }

    void Astronomy::convertEquatorialToHorizontal (
        double jday,
        double longitude,   double latitude,
        double rasc,        double decl,
        double &azimuth,    double &altitude)
    {
        double d = jday - 2451543.5;
        double w = double (282.9404 + 4.70935E-5 * d);
        double M = double (356.0470 + 0.9856002585 * d);
        // Sun's mean longitude
        double L = w + M;
        // Universal time of day in degrees.
        double UT = double(fmod(d, 1) * 360);
        double hourAngle = longitude + L + double (180) + UT - rasc;

        double x = cosDeg (hourAngle) * cosDeg (decl);
        double y = sinDeg (hourAngle) * cosDeg (decl);
        double z = sinDeg (decl);

        double xhor = x * sinDeg (latitude) - z * cosDeg (latitude);
        double yhor = y;
        double zhor = x * cosDeg (latitude) + z * sinDeg (latitude);

        azimuth = atan2Deg (yhor, xhor) + double (180);
        altitude = atan2Deg (zhor, sqrt (xhor * xhor + yhor * yhor));
    }

    void Astronomy::getHorizontalSunPosition (
        double jday,
        double longitude, double latitude,
        double &azimuth, double &altitude)
    {
        // 2451544.5 == Astronomy::getJulianDayFromGregorianDateTime(2000, 1, 1, 0, 0, 0));
        // 2451543.5 == Astronomy::getJulianDayFromGregorianDateTime(1999, 12, 31, 0, 0, 0));
        double d = jday - 2451543.5;

        // Sun's Orbital elements:
        // argument of perihelion
        double w = double (282.9404 + 4.70935E-5 * d);
        // eccentricity (0=circle, 0-1=ellipse, 1=parabola)
        double e = 0.016709 - 1.151E-9 * d;
        // mean anomaly (0 at perihelion; increases uniformly with time)
        double M = double(356.0470 + 0.9856002585 * d);
        // Obliquity of the ecliptic.
        //double oblecl = double (23.4393 - 3.563E-7 * d);

        // Eccentric anomaly
        double E = M + radToDeg(e * sinDeg (M) * (1 + e * cosDeg (M)));

        // Sun's Distance(R) and true longitude(L)
        double xv = cosDeg (E) - e;
        double yv = sinDeg (E) * sqrt (1 - e * e);
        //double r = sqrt (xv * xv + yv * yv);
        double lon = atan2Deg (yv, xv) + w;
        double lat = 0;

        double lambda = degToRad(lon);
        double beta = degToRad(lat);
        double rasc, decl;
        convertEclipticToEquatorialRad (lambda, beta, rasc, decl);
        rasc = radToDeg(rasc);
        decl = radToDeg(decl);

        // Horizontal spherical.
        Astronomy::convertEquatorialToHorizontal (
            jday, longitude, latitude, rasc, decl, azimuth, altitude);
    }


    void Astronomy::getEclipticMoonPositionRad (
        double jday,
        double &lon, double &lat)
    {
        // Julian centuries since January 1, 2000
        double T = (jday - 2451545.0L) / 36525.0L; 
        double lprim = 3.8104L + 8399.7091L * T;
        double mprim = 2.3554L + 8328.6911L * T;
        double m = 6.2300L + 648.3019L * T;
        double d = 5.1985L + 7771.3772L * T;
        double f = 1.6280L + 8433.4663L * T;
        lon = lprim
            + 0.1098L * sin(mprim)
            + 0.0222L * sin(2.0L * d - mprim)
            + 0.0115L * sin(2.0L * d)
            + 0.0037L * sin(2.0L * mprim)
            - 0.0032L * sin(m)
            - 0.0020L * sin(2.0L * f)
            + 0.0010L * sin(2.0L * d - 2.0L * mprim)
            + 0.0010L * sin(2.0L * d - m - mprim)
            + 0.0009L * sin(2.0L * d + mprim)
            + 0.0008L * sin(2.0L * d - m)
            + 0.0007L * sin(mprim - m)
            - 0.0006L * sin(d)
            - 0.0005L * sin(m + mprim);
        lat =
            + 0.0895L * sin(f)
            + 0.0049L * sin(mprim + f)
            + 0.0048L * sin(mprim - f)
            + 0.0030L * sin(2.0L * d - f)
            + 0.0010L * sin(2.0L * d + f - mprim)
            + 0.0008  * sin(2.0L * d - f - mprim)
            + 0.0006L * sin(2.0L * d + f);
    }

    void Astronomy::getHorizontalMoonPosition (
        double jday,
        double longitude, double latitude,
        double &azimuth, double &altitude)
    {
        // Ecliptic spherical
        double lonecl, latecl;
        Astronomy::getEclipticMoonPositionRad (jday, lonecl, latecl);

        // Equatorial spherical
        double rasc, decl; 
        Astronomy::convertEclipticToEquatorialRad (lonecl, latecl, rasc, decl);

        // Radians to degrees (all angles are in radians up to this point)
        rasc = radToDeg(rasc);
        decl = radToDeg(decl);

        // Equatorial to horizontal
        Astronomy::convertEquatorialToHorizontal (
            jday, longitude, latitude, rasc, decl, azimuth, altitude);
    }

    int Astronomy::getJulianDayFromGregorianDate(
        int year, int month, int day)
    {
        // Formulas from http://en.wikipedia.org/wiki/Julian_day
        // These are all integer divisions, but I'm not sure it works
        // correctly for negative values.
        int a = (14 - month) / 12;
        int y = year + 4800 - a;
        int m = month + 12 * a - 3;
        return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    }

    double Astronomy::getJulianDayFromGregorianDateTime(
        int year, int month, int day,
        int hour, int minute, double second)
    {
        int jdn = getJulianDayFromGregorianDate (year, month, day);
        // These are NOT integer divisions.
        double jd = jdn + (hour - 12) / 24.0 + minute / 1440.0 + second / 86400.0;

        return jd;
    }

    double Astronomy::getJulianDayFromGregorianDateTime(
        int year, int month, int day,
        double secondsFromMidnight)
    {
        int jdn = getJulianDayFromGregorianDate(year, month, day);
        double jd = jdn + secondsFromMidnight / 86400.0 - 0.5;
        return jd;
    }

    void Astronomy::getGregorianDateFromJulianDay(
        int julianDay, int &year, int &month, int &day)
    {
        // From http://en.wikipedia.org/wiki/Julian_day
        int J = julianDay;
        int j = J + 32044;
        int g = j / 146097;
        int dg = j % 146097;
        int c = (dg / 36524 + 1) * 3 / 4;
        int dc = dg - c * 36524;
        int b = dc / 1461;
        int db = dc % 1461;
        int a = (db / 365 + 1) * 3 / 4;
        int da = db - a * 365;
        int y = g * 400 + c * 100 + b * 4 + a;
        int m = (da * 5 + 308) / 153 - 2;
        int d = da - (m + 4) * 153 / 5 + 122;
        year = y - 4800 + (m + 2) / 12;
        month = (m + 2) % 12 + 1;
        day = d + 1;
    }

    void Astronomy::getGregorianDateTimeFromJulianDay(
        double julianDay, int &year, int &month, int &day,
        int &hour, int &minute, double &second)
    {
        // Integer julian days are at noon.
        // static_cast<int)(floor( is more precise than Ogre::Math::IFloor.
        // Yes, it does matter.
        int ijd = static_cast<int>(floor(julianDay + 0.5));
        getGregorianDateFromJulianDay(ijd, year, month, day);

        double s = (julianDay + 0.5 - ijd) * 86400.0;
        hour = static_cast<int>(floor(s / 3600));
        s -= hour * 3600;
        minute = static_cast<int>(floor(s / 60));
        s -= minute * 60;
        second = s;
    }

    void Astronomy::getGregorianDateFromJulianDay(
        double julianDay, int &year, int &month, int &day)
    {
        int hour;
        int minute;
        double second;
        getGregorianDateTimeFromJulianDay(julianDay, year, month, day, hour, minute, second);
    }

}
