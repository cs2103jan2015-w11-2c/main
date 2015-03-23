#pragma once

#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
#include "Item.h"
#include "DateTime.h"
#include "..\EasyLoggingpp\easylogging++.h"
using namespace std;

//updateItemDateTime takes in the ITEM to be updated
//and the unedited date and time input from the user
//it then updates the date and time fields in ITEM directly
class DateTimeParser {

private:
	static const string ERROR_NO_DAY_SPECIFIED;
	static const string ERROR_NO_TIME_SPECIFIED;
	static const string ERROR_INVALID_DATE_INPUT;
	static const string ERROR_INVALID_TIME_INPUT;
	static const string ERROR_INVALID_END_TIME;

	Item _item;
	DateTime _dateTime;
	
	int _day;
	int _month;
	int _year;
	int _startHour;
	int _startMinute;
	int _endHour;
	int _endMinute;

public:
	DateTimeParser(void);

	//updates the item with correct Date and Time fields
	//only changes fields that are originally 0
	void updateItemDateTime(string, Item &);

	//reset private date and time fields
	void resetDateTime();

	//reset item date and time
	void resetItemDateTime();

	void updateItemFields();

	void setDate(int day, int month, int year);

	size_t findDateDelimiters(string);

	void calculateDateTime(string);

	// extracts the date and time into item's date/time attributes
	// throws exceptions if invalid dates and times are input
	void extractDateTime(string [], int);

	//returns the weekday number if a match is found
	//returns 0 if match is not found
	int mapWeekDay(string);

	// returns true if the string is a date of format day/month{/year}
	bool isDelimitedDate(string input);

	//retuns true if m, p, or pm is found
	//updates the date to 24hr format
	bool is12Hour(string, int&);

	void separateDayMonthYear(string, int&, int&, int&);

	//updates the hour and minute
	//returns false if invalid time
	bool separateHourMinute(string, int&, int&);

	//verifies that the date and time is valid
	void verifyAllDateTime();

	//throws an out_of_range exception if date is invalid
	void verifyItemDate(int&, int&, int&);

	//throws an out_of_range exception if time is invalid
	void verifyItemTime(int&, int&);

	//ensures that the end time is always greater than the start time
	//throws an out_of_range exception otherwise
	void verifyStartEndTime(int, int, int&, int&);

	// returns 0 if unsuccessful (or number is 0)
	// otherwise returns the converted number
	int convertStringToInteger(string numberString);

	~DateTimeParser(void);

	//For unit testing
	Item getItem();

};