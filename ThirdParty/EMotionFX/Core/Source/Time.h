/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __TIME_H
#define __TIME_H

#include "String.h"


namespace MCore
{

/**
 * The time class represents a concrete date and time.
 */
class Time
{
	public:
		/**
		 * Default constructor.
		 */
		Time();

		/**
		 * Copy constructor.
		 * @param time The time to copy.
		 */
		Time(const Time& time);

		/**
		 * Constructor.
		 * Initializes the time with the passed parameters.
		 * @param day The day in the month ranging from 1 to 31.
		 * @param month The month in the year ranging from 1 to 12.
		 * @param year The year.
		 * @param hours Specifies how many hours of the day have already been passed. Ranging from 0 to 24.
		 * @param minutes Specifies how many minutes in the current hour have already been passed. Ranging from 0 to 59.
		 * @param seconds Specifies how many seconds in the current minute have already been passed. Ranging from 0 to 59.
		 */
		Time(const int day, const int month, const int year, const int hours, const int minutes, const int seconds);

		/**
		 * Destructor.
		 */
		virtual ~Time();

		/**
		 * Set the year of the date/time.
		 * @param year The year component of the date.
		 */
		void SetYear(const int year);

		/**
		 * Set the month in the year.
		 * @param month The month in the year ranging from 1 to 12.
		 */
		void SetMonth(const int month);

		/**
		 * Set the day in the month.
		 * @param day The day in the month ranging from 1 to 31.
		 */
		void SetDay(const int day);

		/**
		 * Set how many hours have been passed of the current day.
		 * @param hours Specifies how many hours of the day have already been passed. Ranging from 0 to 24.
		 */
		void SetHours(const int hours);

		/**
		 * Set how many minutes have been passed in the current hour.
		 * @param minutes Specifies how many minutes in the current hour have already been passed. Ranging from 0 to 59.
		 */
		void SetMinutes(const int minutes);

		/**
		 * Set how many seconds have been passed in the current minute.
		 * @param seconds Specifies how many seconds in the current minute have already been passed. Ranging from 0 to 59.
		 */
		void SetSeconds(const int seconds);

		/**
		 * Set all components of the time.
		 * @param day The day in the month ranging from 1 to 31.
		 * @param month The month in the year ranging from 1 to 12.
		 * @param year The year.
		 * @param hours Specifies how many hours of the day have already been passed. Ranging from 0 to 24.
		 * @param minutes Specifies how many minutes in the current hour have already been passed. Ranging from 0 to 59.
		 * @param seconds Specifies how many seconds in the current minute have already been passed. Ranging from 0 to 59.
		 */
		void Set(const int day, const int month, const int year, const int hours, const int minutes, const int seconds);

		/**
		 * Return the year of the date/time.
		 * @return The year component of the date.
		 */
		int GetYear() const;

		/**
		 * Return the month in the year.
		 * @return The month in the year ranging from 1 to 12.
		 */
		int GetMonth() const;

		/**
		 * Return the day in the month.
		 * @return The day in the month ranging from 1 to 31.
		 */
		int GetDay() const;

		/**
		 * Return how many hours have been passed of the current day.
		 * @return Specifies how many hours of the day have already been passed. Ranging from 0 to 24.
		 */
		int GetHours() const;

		/**
		 * Return how many minutes have been passed in the current hour.
		 * @return Specifies how many minutes in the current hour have already been passed. Ranging from 0 to 59.
		 */
		int GetMinutes() const;

		/**
		 * Return how many seconds have been passed in the current minute.
		 * @return Specifies how many seconds in the current minute have already been passed. Ranging from 0 to 59.
		 */
		int GetSeconds() const;

		/**
		 * Return a string which contains the date and time in the normal form
		 * DAY.MONTH.YEAR HOURS:MINUTES:SECONDS.
		 * @return The time as string.
		 */
		String GetAsString() const;

		/**
		 * Return only the time as string which contains the time in the normal form HOURS:MINUTES:SECONDS.
		 * @return The time as string.
		 */
		String GetTimeAsString() const;

		/**
		 * Return only the date as string which contains the date in the normal form DAY.MONTH.YEAR.
		 * @return The date as string.
		 */
		String GetDateAsString() const;

		/**
		 * Assignment operator.
		 * @see Time()
		 * @param time The time to assign.
		 * @result A reference to *this which allows concatenation.
		 */
		Time& operator=(const Time& time);

		/*
		 * Equality operator.
		 * Compares this time and a passed time for equality.
		 * @param time The time to compare with.
		 * @result True if the two times are equal, false if not.
		 */
		bool operator==(const Time& time) const;

		/**
		 * Inequality operator.
		 * Times are inequal if one of its components are inequal.
		 * @param time The time to compare with.
		 * @result True if the two times are inequal, false if not.
		 */
		bool operator!=(const Time& time) const;

		/**
		 * Comparison operator.
		 * @param time The time to compare with.
		 * @result True if the time is less or equal than the passed time, false if not.
		 */
		bool operator<=(const Time& time) const;

		/**
		 * Comparison operator.
		 * @param time The time to compare with.
		 * @result True if the time is greater or equal than the passed time, false if not.
		 */
		bool operator>=(const Time& time) const;

		/**
		 * Comparison operator.
		 * @param time The time to compare with.
		 * @result True if the time is less than the passed time, false if not.
		 */
		bool operator<(const Time& time) const;

		/**
		 * Comparison operator.
		 * @param time The time to compare with.
		 * @result True if the time is greater than the passed time, false if not.
		 */
		bool operator>(const Time& time) const;

	private:
		int	mYear;
		int	mMonth;
		int	mDay;
		int	mHours;
		int	mMinutes;
		int	mSeconds;
};

} // namespace MCore


#endif