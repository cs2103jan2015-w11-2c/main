#include "DateTime.h"

DateTime::DateTime() {
	_now = time(0);
	localtime_s(&_today, &_now);
}

int DateTime::getCurrentDay() {
	return (_today.tm_mday);
}

int DateTime::getCurrentMonth() {
	return (_today.tm_mon + 1);
}

int DateTime::getCurrentYear() {
	return (1900 + _today.tm_year);
}

string DateTime::getCurrentWeekDay() {
	return (WEEKDAY[_today.tm_wday]);
}

int DateTime::getCurrentMinute() {
	return (_today.tm_min);
}

int DateTime::getCurrentHour() {
	return (1 + _today.tm_hour);
}

// returns the weekday for the specified day, month and year
// Sunday = 0, Saturday = 6;
int DateTime::getIntWeekDay (int day, int month, int year) {
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	year -= month < 3;
	int wday = (year + year/4 - year/100 + year/400 + t[month - 1] + day) % 7;
	return wday;
}

// returns the weekday name for the specified day, month and year
// Sunday, Monday, .... Saturday
string DateTime::getWeekDay (int day, int month, int year) {
	int wday = getIntWeekDay(day, month, year);
	return WEEKDAY[wday];
}

string DateTime::getMonth(const int mon) {
	return MONTH[mon-1];
}

bool DateTime::isLeapYear (int year) {
	return (((year % 400) == 0) || (year % 4) == 0) && (((year % 100) != 0));
}

int DateTime::numDaysInMonth(const int& month, const int& year) {
	if((month <= 0) || (month > 12)) {
		return 0;
	} else if((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
		return 30;
	} else if(month == 2 && isLeapYear(year)) {
		return 29;
	} else if(month == 2 && !isLeapYear(year)) {
		return 28;
	} else {
		return 31;
	}
}

bool DateTime::isValidDate(int day, int month, int year) {
	if(year < MIN_YEAR || year > MAX_YEAR) {
		return false;
	}

	if((month <= 0) || (month > 12)) {
		return false;
	}

	if((day <= 0) || (day > numDaysInMonth(month, year))) {
		return false;
	}

	return true;
}

bool DateTime::isValidTime(int hour, int minute) {
	return (hour >= 0 && hour <= 24 && minute >= 0 && minute <= 60);
}

bool DateTime::isGreate(DateTime& other) {
	if      ( this->getCurrentHour() < other.getCurrentHour() ){
		return true;
	}
	else if ( this->getCurrentHour() > other.getCurrentHour() ){
		return false;
	}
	else if ( this->getCurrentMinute() < other.getCurrentMinute() ){
		return  true;
	}
	else if ( this->getCurrentMinute() > other.getCurrentMinute() ){
		return false;
	}
	else{
		return  true;
	}
}