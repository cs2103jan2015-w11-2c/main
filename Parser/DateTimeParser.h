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
	int _endDay;
	int _endMonth;
	int _endYear;
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
	//returns -1 if match is not found
	int mapWeekDay(string);

	//returns the month number if a match is found
	//returns -1 if match is not found
	int mapMonth(string);

	//sets the day, month and year from weekday input
	void setDateFromWeekDay(int weekDayIndex, int& day, int& month, int& year);

	//accounts for "next" when weekday is specified,
	//by updating the day, month and year
	void handleNextWeekDay(int& day, int& month, int& year);

	//only for use after operations such as adding 'next' to day
	//handles days greater than the number of days in the current month
	void handleDayOverflow(int& day, int& month, int& year);

	//if end day is less than start day, increment by 7
	void handleImplicitNext(int&, int&, int&, int&, int&, int&);

	//sets the month, sets the day and resets hour
	void updateHrDayMon(int monthNum, int& hour, int& day, int& month, int& year, int& itemHour);

	// returns true if the string is a date of format day/month{/year}
	bool isDelimitedDate(string);

	//returns true if the string is a possible time
	bool isPossibleTime(string input);

	//retuns true if m, p, or pm is found
	//updates the date to 24hr format
	bool is12Hour(string, int&);

	//updates the day, month and year
	//year taken as current year if not specified
	void separateDayMonthYear(string, int&, int&, int&);

	//updates the hour and minute
	void separateHourMinute(string, int&, int&);

	//verifies that the date and time is valid
	void verifyAllDateTime();

	//throws an out_of_range exception if date is invalid
	void verifyItemDate(int&, int&, int&);

	//if no start date is set, it is set to today
	void updateItemStartDate();

	//if endDate fields are empty, set them to startDate
	void updateItemEndDate();

	//throws an out_of_range exception if time is invalid
	void verifyItemTime(int&, int&);

	//ensures that the end date and time is always greater or equal to the start
	//throws an out_of_range exception otherwise, and resets the offending values
	void verifyStartEnd(	
		int startHr, 
		int startMin, 
		int& endHr, 
		int& endMin,
		int startDay,
		int startMonth,
		int startYear,
		int& endDay,
		int& endMonth,
		int& endYear);

	// returns 0 if unsuccessful (or number is 0)
	// otherwise returns the converted number
	int convertStringToInteger(string numberString);

	~DateTimeParser(void);

	//For unit testing
	Item getItem();

};

