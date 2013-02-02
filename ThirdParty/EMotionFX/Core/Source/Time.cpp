/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Time.h"


namespace MCore
{

// default constructor
Time::Time() : mYear(0), mMonth(0), mDay(0), mHours(0), mMinutes(0), mSeconds(0)
{
}


// copy constructor
Time::Time(const Time& time) : mYear(time.mYear), mMonth(time.mMonth), mDay(time.mDay), mHours(time.mHours), mMinutes(time.mMinutes), mSeconds(time.mSeconds)
{
}


// initializes the time with the passed parameters
Time::Time(const int day, const int month, const int year, const int hours, const int minutes, const int seconds) : mYear(year), mMonth(month), mDay(day), mHours(hours), mMinutes(minutes), mSeconds(seconds)
{
}


// destructor
Time::~Time()
{
}


// set the year of the date/time
void Time::SetYear(const int year)
{
	mYear = year;
}


// set the month in the year
void Time::SetMonth(const int month)
{
	mMonth = month;
}


// set the day in the month
void Time::SetDay(const int day)
{
	mDay = day;
}


// set how many hours have been passed of the current day
void Time::SetHours(const int hours)
{
	mHours = hours;
}


// set how many minutes have been passed in the current hour
void Time::SetMinutes(const int minutes)
{
	mMinutes = minutes;
}


// set how many seconds have been passed in the current minute
void Time::SetSeconds(const int seconds)
{
	mSeconds = seconds;
}


// return the year of the date/time
int Time::GetYear() const
{
	return mYear;
}


// return the month in the year
int Time::GetMonth() const
{
	return mMonth;
}


// return the day in the month
int Time::GetDay() const
{
	return mDay;
}


// return how many hours have been passed of the current day
int Time::GetHours() const
{
	return mHours;
}


// return how many minutes have been passed in the current hour
int Time::GetMinutes() const
{
	return mMinutes;
}


// return how many seconds have been passed in the current minute
int Time::GetSeconds() const
{
	return mSeconds;
}


// set all components of the time
void Time::Set(const int day, const int month, const int year, const int hours, const int minutes, const int seconds)
{
	mYear		= year;
	mMonth		= month;
	mDay		= day;
	mHours		= hours;
	mMinutes	= minutes;
	mSeconds	= seconds;
}


// return a string which contains the date and time
String Time::GetAsString() const
{
	String time;
	time = String(mDay) + "." + String(mMonth) + "." + String(mYear) + " ";
	time += String(mHours) + ":" + String(mMinutes) + ":" + String(mSeconds);

	return time;
}


// return a string which contains the time
String Time::GetTimeAsString() const
{
	String time;
	time = String(mHours) + ":" + String(mMinutes) + ":" + String(mSeconds);

	return time;
}


// return a string which contains the date
String Time::GetDateAsString() const
{
	String time;
	time = String(mDay) + "." + String(mMonth) + "." + String(mYear);

	return time;
}


// assignment operator
Time& Time::operator=(const Time& time)
{
	mYear		= time.mYear;
	mMonth		= time.mMonth;
	mDay		= time.mDay;
	mHours		= time.mHours;
	mMinutes	= time.mMinutes;
	mSeconds	= time.mSeconds;

	return *this;
}


// equality operator
bool Time::operator==(const Time& time) const
{
	return ( (mYear == time.mYear) && (mMonth == time.mMonth) && (mDay == time.mDay) &&	(mHours == time.mHours) && (mMinutes == time.mMinutes) && (mSeconds == time.mSeconds) );
}


// inequality operator
bool Time::operator!=(const Time& time) const
{
	return ( (mYear != time.mYear) || (mMonth != time.mMonth) || (mDay != time.mDay) ||	(mHours != time.mHours) || (mMinutes != time.mMinutes) || (mSeconds != time.mSeconds) );
}


// comparison operator
bool Time::operator<=(const Time& time) const
{
	return ( (*this == time) || (*this < time) );
}


// comparison operator
bool Time::operator>=(const Time& time) const
{
	return ( (*this == time) || (*this > time) );
}


// comparison operator
bool Time::operator<(const Time& time) const
{
	// check if the year already is less and return true if it is
	if (mYear < time.mYear)
	{
		return true;
	}

	// the year is equal to the passed time
	if (mYear == time.mYear)
	{
		// since the year is the same check the month
		if (mMonth < time.mMonth)
		{
			return true;
		}

		if (mMonth == time.mMonth)
		{
			// year and month are equal
			if (mDay < time.mDay)
			{
				return true;
			}

			if (mDay == time.mDay)
			{
				// the date is equal, now the time
				if (mHours < time.mHours)
				{
					return true;
				}

				if (mHours == time.mHours)
				{
					if (mMinutes < time.mMinutes)
					{
						return true;
					}

					if (mMinutes == time.mMinutes)
					{
						if (mSeconds < time.mSeconds)
						{
							return true;
						}

						if (mSeconds == time.mSeconds)
						{
							// the time is exact the same
							return false;
						}
					}
				}
			}
		}
	}

	// the passed date is greater
	return false;
}


// comparison operator
bool Time::operator>(const Time& time) const
{
	// check if the year already is greater and return true if it is
	if (mYear > time.mYear)
	{
		return true;
	}

	// the year is equal to the passed time
	if (mYear == time.mYear)
	{
		// since the year is the same check the month
		if (mMonth > time.mMonth)
		{
			return true;
		}

		if (mMonth == time.mMonth)
		{
			// year and month are equal
			if (mDay > time.mDay)
			{
				return true;
			}

			if (mDay == time.mDay)
			{
				// the date is equal, now the time
				if (mHours > time.mHours)
				{
					return true;
				}

				if (mHours == time.mHours)
				{
					if (mMinutes > time.mMinutes)
					{
						return true;
					}

					if (mMinutes == time.mMinutes)
					{
						if (mSeconds > time.mSeconds)
						{
							return true;
						}

						if (mSeconds == time.mSeconds)
						{
							// the time is exact the same
							return false;
						}
					}
				}
			}
		}
	}

	// the passed date is less
	return false;
}

}