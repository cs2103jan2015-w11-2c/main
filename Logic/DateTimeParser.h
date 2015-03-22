#pragma once

#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
#include "Item.h"
#include "DateTime.cpp"
#include "..\EasyLoggingpp\easylogging++.h"
using namespace std;

//updateItemDateTime takes in the ITEM to be updated
//and the unedited date and time input from the user
//it then updates the date and time fields in ITEM directly
class DateTimeParser {

private:
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
	void extractDateTime(string [], int);

	//void handleOneDateInput(string []);

	//void handleTwoDateInput(string inputArray []);

	//returns the weekday number if a match is found
	//returns 0 if match is not found
	int mapWeekDay(string);
	
	//return the month from the weekday given
	int mapToGetMonth(string);
	
	//return the date from the weekday given
	int mapToGetDate(string);

	// returns true if the string is a date of format day/month{/year}
	bool isDelimitedDate(string input);

	//retuns true if m, p, or pm is found
	//updates the date to 24hr format
	bool is12Hour(string, int&);

	void separateDayMonthYear(string, int&, int&, int&);

	//updates the hour and minute
	//returns false if invalid time
	bool separateHourMinute(string, int&, int&);

	//ensure that date is correct. If not, throws an exception
	void verifyItemDate(int&, int&, int&);

	void verifyItemTime(int&, int&);

	// returns 0 if unsuccessful (or number is 0)
	// otherwise returns the converted number
	int convertStringToInteger(string numberString);

	~DateTimeParser(void);

	//For unit testing
	Item getItem();

};

