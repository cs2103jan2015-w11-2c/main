#include "DateTimeParser.h"

const string DateTimeParser::ERROR_NO_DAY_SPECIFIED = "Invalid input: No day specified after \"next\"";
const string DateTimeParser::ERROR_NO_TIME_SPECIFIED = "Invalid input: Time expected after \"-\"";
const string DateTimeParser::ERROR_INVALID_DATE_INPUT = "Invalid date input!";
const string DateTimeParser::ERROR_INVALID_TIME_INPUT = "Invalid time input!";
const string DateTimeParser::ERROR_INVALID_END_TIME = "Invalid end time: end time must be greater than start time";

DateTimeParser::DateTimeParser(void) {
	_day = 0;
	_month = 0;
	_year = 0;
	_startHour = 0;
	_startMinute = 0;
	_endHour = 0;
	_endMinute = 0;
}


void DateTimeParser::updateItemDateTime(string rawDateTime, Item& item) {
	_item = item;
	resetDateTime();
	calculateDateTime(rawDateTime);
	item = _item;
}

void DateTimeParser::resetDateTime() {
	_day = 0;
	_month = 0;
	_year = 0;
	_startHour = 0;
	_startMinute = 0;
	_endHour = 0;
	_endMinute = 0;
}

void DateTimeParser::resetItemDateTime() {
	_item.eventDate[0] = 0;
	_item.eventDate[1] = 0;
	_item.eventDate[2] = 0;
	_item.eventStartTime[0] = 0;
	_item.eventStartTime[1] = 0;
	_item.eventEndTime[0] = 0;
	_item.eventEndTime[1] = 0;
}

void DateTimeParser::updateItemFields() {
	LOG(INFO) << 	"Before update";

	LOG(INFO) << 	_item.eventDate[0];
	LOG(INFO) << 	_item.eventDate[1];
	LOG(INFO) << 	_item.eventDate[2];
	LOG(INFO) << 	_item.eventStartTime[0];
	LOG(INFO) << 	_item.eventStartTime[1];
	LOG(INFO) << 	_item.eventEndTime[0];
	LOG(INFO) << 	_item.eventEndTime[1];

	if(_item.eventDate[0] == 0) {
		_item.eventDate[0] = _day;
	}
	if(_item.eventDate[1] == 0) {
		_item.eventDate[1] = _month;
	}
	if(_item.eventDate[2] == 0) {
		_item.eventDate[2] = _year;
	}
	if((_item.eventStartTime[0] == 0) || ((_item.eventStartTime[0] + 12) == _startHour)) {
		_item.eventStartTime[0] = _startHour;
	}
	if(_item.eventStartTime[1] == 0) {
		_item.eventStartTime[1] = _startMinute;
	}
	if((_item.eventEndTime[0] == 0) || ((_item.eventEndTime[0] + 12) == _endHour)) {
		_item.eventEndTime[0] = _endHour;
	}
	if(_item.eventEndTime[1] == 0) {
		_item.eventEndTime[1] = _endMinute;
	}
	LOG(INFO) << 	"After update:";
	LOG(INFO) << 	_item.eventDate[0];
	LOG(INFO) << 	_item.eventDate[1];
	LOG(INFO) << 	_item.eventDate[2];
	LOG(INFO) << 	_item.eventStartTime[0];
	LOG(INFO) << 	_item.eventStartTime[1];
	LOG(INFO) << 	_item.eventEndTime[0];
	LOG(INFO) << 	_item.eventEndTime[1];
	LOG(INFO) << "";
}

void DateTimeParser::setDate(int day, int month, int year) {
	_item.eventDate[0] = day;
	_item.eventDate[1] = month;
	_item.eventDate[2] = year;
}

size_t DateTimeParser::findDateDelimiters(string inputLine) {
	return (inputLine.find_first_of("/._"));
}

void DateTimeParser::calculateDateTime(string input) {
	istringstream iss(input);
	string demarcateDateTime[7];
	int i = 0;
	while (iss >> demarcateDateTime[i]) {
		i++;
	}

	if(i == 0) {
		setDate(_dateTime.getCurrentDay(), _dateTime.getCurrentMonth(), _dateTime.getCurrentYear());
	} else {
		extractDateTime(demarcateDateTime, i);
	}
}

void DateTimeParser::extractDateTime(string inputArray[], int arrSize) {
	bool isNextWeek= false;
	bool hasDash = false;
	bool isFirstTimeInstance = true;
	resetDateTime();
	resetItemDateTime();

	for(int i = 0; i < arrSize; i++) {
		LOG(INFO) << "Starting to extract DateTime, round: " << i;

		_day = mapWeekDay(inputArray[i]);

		// throws exception if weekday is expected but not given
		if(isNextWeek && _day == 0) {
			isNextWeek = false;
			throw std::out_of_range(ERROR_NO_DAY_SPECIFIED);
		}

		// throws exception if time is expected but not given
		if(hasDash && !separateHourMinute(inputArray[i], _startHour, _startMinute)) {
			hasDash = false;
			throw std::out_of_range(ERROR_NO_TIME_SPECIFIED);
		}

		// "next" keyword
		if(inputArray[i] == "next") {
			LOG(INFO) << "NEXT";
			isNextWeek = true;
			// "-" keyword
		} else if(inputArray[i] == "-") {
			LOG(INFO) << "DASH";
			hasDash = true;
			// weekday (e.g. Friday)
		} else if(_day != 0) {
			LOG(INFO) << "WEEKDAY";
			if(isNextWeek) {
				_day += 7;
				isNextWeek = false;
			} 
			// date/month/year
		} else if(isDelimitedDate(inputArray[i])) {
			LOG(INFO) << "DELIMITED DATE";
			// start time
		} else if(isFirstTimeInstance && separateHourMinute(inputArray[i], _startHour, _startMinute)) {
			LOG(INFO) << "START TIME";
			isFirstTimeInstance = false;
			// end time
		} else if(!isFirstTimeInstance && hasDash && separateHourMinute(inputArray[i], _endHour, _endMinute)) {
			LOG(INFO) << "END TIME";
			isFirstTimeInstance = true;
			hasDash = false;
			// duration entered instead of end time
		} else if(!isFirstTimeInstance && !hasDash && convertStringToInteger(inputArray[i]) > 0) {
			LOG(INFO) << "DURATION ADDED FROM START";
			isFirstTimeInstance = true;
			_endHour = _startHour + convertStringToInteger(inputArray[i]);
			_endMinute = _startMinute;
			// "m", "p", or "pm" keywords
		} else if(!isFirstTimeInstance && is12Hour(inputArray[i], _startHour)) {
			LOG(INFO) << "PM OR M, Start Hour";
		} else if(is12Hour(inputArray[i], _endHour)) {
			LOG(INFO) << "PM OR M, End Hour";
		}
		LOG(INFO) << "********************************************";
		//verifyAllDateTime();
		updateItemFields();
	}
}

// TO BE EDITED!!!!
int DateTimeParser::mapWeekDay(string day) {
	if(day == "Fri") {
		return 1;
	} else return 0;
}

bool DateTimeParser::isDelimitedDate(string input) {

	size_t dateDelimiterPos = findDateDelimiters(input);

	if (dateDelimiterPos != string::npos) {
		separateDayMonthYear(input, _day, _month, _year);
		
		try {
			verifyItemDate(_day, _month, _year);
		} catch (const out_of_range& e) {
			LOG(ERROR) << "isDelimitedDate Error: " << e.what();
			clog << e.what();
		}
		return true;
	}
	return false;
}

bool DateTimeParser::is12Hour(string input, int& hour) {
	if ((input == "p") || (input == "pm") || ((input == "m") && (hour == 12))) {
		hour += 12;
		//ignores p for 24hr input
		if (!_dateTime.isValidTime(hour, 0)) {
			hour -= 12;
		}
		return true;
	}
	return false;
}

void DateTimeParser::separateDayMonthYear(string input, int& day, int& month, int& year) {
	char *intEnd;
	day = (int)strtol(input.c_str(), &intEnd, 10);
	month = (int)strtol(intEnd + 1, &intEnd, 10);
	year = (int)strtol(intEnd + 1, &intEnd, 10);
	
	if((*intEnd != 0) || (year == 0)) {
		year = _dateTime.getCurrentYear();
	}
}

bool DateTimeParser::separateHourMinute(string hourMinute, int& hour, int& minute) {
	char *intEnd;
	hour = (int)strtol(hourMinute.c_str(), &intEnd, 10);
	minute = (int)strtol(intEnd + 1, &intEnd, 10);
	return (hour != 0);
}

void DateTimeParser::verifyAllDateTime() {
	verifyItemDate(_day, _month, _year);
	verifyItemTime(_startHour, _startMinute);
	verifyItemTime(_endHour, _endMinute);
	verifyStartEndTime(_startHour, _startMinute, _endHour, _endMinute);
}

void DateTimeParser::verifyItemDate(int& day, int& month, int& year) {
	if (year == 0) {
		year = _dateTime.getCurrentYear();
	} else if (year < 2000) {
		year += 2000;
	}
	
	if (!_dateTime.isValidDate(day, month, year)) {
		day = 0;
		month = 0;
		year = 0;
		throw std::out_of_range(ERROR_INVALID_DATE_INPUT);
	}
}

void DateTimeParser::verifyItemTime(int& hour, int& minute) {
	if (!_dateTime.isValidTime(hour, minute)) {
		hour = 0;
		minute = 0;
		throw std::out_of_range(ERROR_INVALID_TIME_INPUT);
	}
}

void DateTimeParser::verifyStartEndTime(int startHr, int startMin, int& endHr, int& endMin) {
	if((endHr < startHr) || ((endHr == startHr) && (endMin <= startMin))) {
		endHr = 0;
		endMin = 0;
		throw std::out_of_range(ERROR_INVALID_END_TIME);
	}
}

int DateTimeParser::convertStringToInteger(string numberString) {
	char *end;
	return (int)strtol(numberString.c_str(), &end, 10);
}

DateTimeParser::~DateTimeParser(void) {
}

Item DateTimeParser::getItem() {
	return _item;
}
