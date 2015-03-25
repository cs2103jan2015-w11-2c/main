#pragma once

#include <ctime>
#include <string>
#include <sstream>

using namespace std;

//constants
const int MIN_YEAR = 2000;
const int MAX_YEAR = 3000;
const string WEEKDAY[]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const string MONTH[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

class DateTime {

private:
	time_t _now;
	tm _today;

public:
	DateTime();

	int getCurrentDay();

	int getCurrentMonth();

	int getCurrentYear();

	string getCurrentWeekDay();

	int getCurrentMinute();

	int getCurrentHour();

	// returns the weekday for the specified day, month and year
	// Sunday = 0, Saturday = 6;
	int getIntWeekDay (int, int, int);

	// returns the weekday name for the specified day, month and year
	// Sunday, Monday, .... Saturday
	string getWeekDay (int, int, int);

	string getMonth(const int);

	bool isLeapYear (int);

	int numDaysInMonth(const int& month, const int& year);

	bool isValidDate(int, int, int);

	bool isValidTime(int, int);
};
