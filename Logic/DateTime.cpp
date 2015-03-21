#pragma once

#include <ctime>
#include <string>
#include <sstream>

using namespace std;

//constants
const int MIN_YEAR = 2000;
const int MAX_YEAR = 3000;
const string WEEKDAY[]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const string MONTH[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

class DateTime {

private:
	time_t _now;
	tm _today;

public:
	DateTime() {
		_now = time(0);
		localtime_s(&_today, &_now);
	}

	int getCurrentDay() {
		return (_today.tm_mday);
	}

	int getCurrentMonth() {
		return (_today.tm_mon);
	}

	int getCurrentYear() {
		return (1900 + _today.tm_year);
	}

	string getCurrentWeekDay() {
		return (WEEKDAY[_today.tm_wday]);
	}

	int getCurrentMinute() {
		return (_today.tm_min);
	}

	int getCurrentHour() {
		return (1 + _today.tm_hour);
	}

	// returns the weekday for the specified day, month and year
	// Sunday = 0, Saturday = 6;
	int getIntWeekDay (int day, int month, int year) {
		static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
		year -= month < 3;
		int wday = (year + year/4 - year/100 + year/400 + t[month - 1] + day) % 7;
		return wday;
	}

	// returns the weekday name for the specified day, month and year
	// Sunday, Monday, .... Saturday
	string getWeekDay (int day, int month, int year) {
		int wday = getIntWeekDay(day, month, year);
		return WEEKDAY[wday];
	}

	string getMonth(const int mon) {
		return MONTH[mon-1];
	}

	bool isLeapYear (int year) {
		return (((year % 400) == 0) || (year % 4) == 0) && (((year % 100) != 0));
	}

	bool isValidDate(int day, int month, int year) {
		if((day <= 0) || (month <= 0) || (day > 31) || (month > 12)) {
			return false;
		}

		if((month == 4) || (month == 6) || (month == 9) || (month || 11)) {
			if(day > 30) {
				return false;
			}
		}

		if(month == 2) {
			if(isLeapYear(year) && day <= 29) {
				return true;
			} else return (day < 29);
		}

		if(year < MIN_YEAR || year > MAX_YEAR) {
			return false;
		}

		return true;
	}

	bool DateTime::isValidTime(int hour, int minute) {
		return (hour >= 0 && hour <= 24 && minute >= 0 && minute <= 60);
	}


};
