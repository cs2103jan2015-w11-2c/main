#pragma once

#include <ctime>
#include <string>
#include <sstream>

using namespace std;

//constants
const string WEEKDAY[]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

class DateTime {

private:
	time_t now;
	tm today;

public:
	DateTime() {
		now = time(0);
		localtime_s(&today, &now);
	}

	int getCurrentDay() {
		return (today.tm_mday);
	}

	int getCurrentMonth() {
		return (today.tm_mon);
	}

	int getCurrentYear() {
		return (1990 + today.tm_year);
	}

	string getCurrentWeekDay() {
		return (WEEKDAY[today.tm_wday]);
	}

	int getCurrentMinute() {
		return (today.tm_min);
	}

	int getCurrentHour() {
		return (1 + today.tm_hour);
	}

	// returns the weekday for the specified day, month and year
	string getWeekDay (int day, int month, int year) {
		static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
		year -= month < 3;
		int wday = (year + year/4 - year/100 + year/400 + t[month - 1] + day) % 7;
		return WEEKDAY[wday];
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

		return true;
	}

};