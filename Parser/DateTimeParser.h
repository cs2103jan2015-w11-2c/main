#pragma once
//@author A0111951N

#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
#include "Item.h"
#include "DateTime.h"
#include "..\EasyLoggingpp\easylogging++.h"
using namespace std;

const int DATE_KEYWORDS_SIZE = 56;
const string DATE_KEYWORDS[]= {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday",
						"sunday", "mon", "tue", "tues", "wed", "thur", "thurs", "fri", "sat", "sun",
						"january", "february", "march", "april", "may", "june", "july", "august",
						 "september", "october", "november", "december", "jan", "feb", "mar",
						 "apr", "may", "jun", "jul", "aug", "sep", "sept", "oct", "nov", "novem",
						 "dec", "decem", "today", "floating", "float", "tomorrow", "tom", "tmr",
						 "to", "-", "next", "nex", "p", "pm", "m"};


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
	static const int NUM_DATE;

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

	bool _updateDateFlag;
	bool _updateTimeFlag;
	bool _isFloating;
	bool _isDateChangedFromFloat;
	bool _isDeadlineEvent;

public:
	DateTimeParser(void);

	//takes as input the raw date & time string, the Item
	//to be updated, and boolean isDeadline
	//updates the item with correct Date and Time fields
	void updateItemDateTime(string, Item &, bool);

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
	//returns 0 for today, -2 for tomorrow and -3 for floating 
	//returns -1 if match is not found
	int mapWeekDay(string);

	//returns the month number if a match is found
	//returns -1 if match is not found
	int mapMonth(string);

	//sets the day, month and year from weekday input
	//-2 taken as tomorrow, -3 taken as floating
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

	//returns true if the word is a possible date/time keyword
	bool isDateKeyword(string);

	// returns true if the string is a date of format day/month{/year}
	bool isDelimitedDate(string);

	//returns true if the string is a possible time
	bool isPossibleTime(string input);

	//returns true if m, or p is found
	//updates the date to 24hr format
	bool is12Hour(char, int&);

	//overloaded function
	//returns true if m, p, or pm is found
	//updates the date to 24hr format
	bool is12Hour(string, int&);

	//sets the end time based on start time + duration
	//max duration allowed is 24 hours
	void addDuration(
		int duration,
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
	//unless the date is implicitly specified as "floating"
	void updateItemStartDate();

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

	void getNextDayDate(int &, int &, int &);

	// returns 0 if unsuccessful (or number is 0)
	// otherwise returns the converted number
	int convertStringToInteger(string numberString);

	~DateTimeParser(void);

	//For unit testing
	Item getItem();

	bool getUpdateDateFlag();

	bool getUpdateTimeFlag();

	bool getIsDateUpdatedFromFloat();
};

