#pragma once


namespace Myway { namespace Env {
   
    class Astronomy
    {
    private:
        Astronomy() {}

        static const double PI;

        /** Normalize an angle to the 0, 360 range.
         *  @param x The angle to normalize
         */
        static double normalizeDegrees (double x);

        /// Convert radians to degrees.
        static double radToDeg (double x);

        /// Convert degrees to radians.
        static double degToRad (double x);

        static double sinDeg (double x);
        static double cosDeg (double x);
        static double atan2Deg (double y, double x);

    public:
        /// January 1, 2000, noon
        static const double J2000;

        /** Convert from ecliptic to ecuatorial spherical coordinates, in radians.
         *  @param lon Ecliptic longitude
         *  @param lat Ecliptic latitude
         *  @param rasc Right ascension
         *  @param decl Declination
         *  @warning: This function works in radians.
         */
		static void convertEclipticToEquatorialRad (
                double lon, double lat,
                double& rasc, double& decl);

		static void convertRectangularToSpherical (
                double x, double y, double z,
                double &rasc, double &decl, double &dist);

        static void convertSphericalToRectangular (
                double rasc, double decl, double dist,
                double &x, double &y, double &z);

        /** Convert from equatorial to horizontal coordinates.
         *  This function converts from angles relative to the earth's equator
         *  to angle relative to the horizon at a given point.
         *  @param jday Astronomical time as julian day.
         *  @param longitude Observer's longitude in degrees east.
         *  @param latitude Observer's latitude in degrees north.
         *  @param rasc Object's right ascension.
         *  @param decl Object's declination.
         *  @param azimuth Object's azimuth (clockwise degrees from true north).
         *  @param altitude Object's altitude (degrees above the horizon).
         */
        static void convertEquatorialToHorizontal (
                double jday,
                double longitude, double latitude,
                double rasc,      double decl,
                double &azimuth,  double &altitude);

        /** Get the sun's position in the sky in, relative to the horizon.
         *  @param jday Astronomical time as julian day.
         *  @param longitude Observer longitude
         *  @param latitude Observer latitude
         *  @param azimuth Astronomical azimuth, measured clockwise from North = 0.
         *  @param altitude Astronomical altitude, elevation above the horizon.
         */
        static void getHorizontalSunPosition (
                double jday,
                double longitude, double latitude,
                double &azimuth, double &altitude);

        /// Gets the moon position at a specific time in ecliptic coordinates
        /// @param lon: Ecliptic longitude, in radians.
        /// @param lat: Ecliptic latitude, in radians.
		static void getEclipticMoonPositionRad (
                double jday,
                double &lon,
                double &lat);

        static void getHorizontalMoonPosition (
                double jday,
                double longitude, double latitude,
                double &azimuth, double &altitude);

        /** Get astronomical julian day from normal gregorian calendar.
         *  From wikipedia: the integer number of days that have elapsed
         *  since the initial epoch defined as
         *  noon Universal Time (UT) Monday, January 1, 4713 BC
         *  @note this is the time at noon, not midnight.
         */
        static int getJulianDayFromGregorianDate (
                int year, int month, int day); 

        /** Get astronomical julian day from normal gregorian calendar.
         *  Calculate julian day from a day in the normal gregorian calendar.
         *  Time should be given as UTC.
         *  @see http://en.wikipedia.org/wiki/Julian_day
         */
        static double getJulianDayFromGregorianDateTime (
                int year, int month, int day,
                int hour, int minute, double second); 

        /** Get astronomical julian day from normal gregorian calendar.
         *  @see above (I don't know the proper doxygen syntax).
         */
        static double getJulianDayFromGregorianDateTime (
                int year, int month, int day,
                double secondsFromMidnight); 

        /// Get gregorian date from integer julian day.
        static void getGregorianDateFromJulianDay (
                int julianDay, int &year, int &month, int &day);

        /// Get gregorian date time from floating point julian day.
        static void getGregorianDateTimeFromJulianDay (
                double julianDay, int &year, int &month, int &day,
                int &hour, int &minute, double &second);

        /// Get gregorian date from floating point julian day.
        static void getGregorianDateFromJulianDay (
                double julianDay, int &year, int &month, int &day);
    };

}}

