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
	_endDay = 0;
	_endMonth = 0;
	_endYear = 0;
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
	_endDay = 0;
	_endMonth = 0;
	_endYear = 0;
	_startHour = 0;
	_startMinute = 0;
	_endHour = 0;
	_endMinute = 0;
}

void DateTimeParser::resetItemDateTime() {
	_item.eventDate[0] = 0;
	_item.eventDate[1] = 0;
	_item.eventDate[2] = 0;
	_item.eventEndDate[0] = 0;
	_item.eventEndDate[1] = 0;
	_item.eventEndDate[2] = 0;
	_item.eventStartTime[0] = 0;
	_item.eventStartTime[1] = 0;
	_item.eventEndTime[0] = 0;
	_item.eventEndTime[1] = 0;
}

void DateTimeParser::updateItemFields() {
	if(_item.eventDate[0] == 0) {
		_item.eventDate[0] = _day;
	}
	if(_item.eventDate[1] == 0) {
		_item.eventDate[1] = _month;
	}
	if(_item.eventDate[2] == 0) {
		_item.eventDate[2] = _year;
	}
	if(_item.eventEndDate[0] == 0) {
		_item.eventEndDate[0] = _endDay;
	}
	if(_item.eventEndDate[1] == 0) {
		_item.eventEndDate[1] = _endMonth;
	}
	if(_item.eventEndDate[2] == 0) {
		_item.eventEndDate[2] = _endYear;
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

	LOG(INFO) << "Item values after update:";
	_item.logItemValues();
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
	string demarcateDateTime[9];
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
	bool isStartTime = true;
	bool isEndTime = false;
	bool isStartDate = true;
	bool isEndDate = false;
	resetDateTime();
	resetItemDateTime();

	for(int i = 0; i < arrSize; i++) {
		LOG(INFO) << "Starting to extract DateTime, round: " << i;
		/*
		// throws exception if weekday is expected but not given
		if(isNextWeek && _day == 0) {
		isNextWeek = false;
		throw std::out_of_range(ERROR_NO_DAY_SPECIFIED);
		}

		// throws exception if time is expected but not given
		if(hasDash && !separateHourMinute(inputArray[i], _endHour, _endMinute)) {
		hasDash = false;
		throw std::out_of_range(ERROR_NO_TIME_SPECIFIED);
		}
		*/

		// "next" keyword
		if((inputArray[i] == "next") || (inputArray[i] == "nex")) {
			isNextWeek = true;
			LOG(INFO) << "NEXT";
			// "-" or "to" keyword
		} else if((inputArray[i] == "-") || (inputArray[i] == "to")) {
			hasDash = true;
			LOG(INFO) << "DASH";
			// weekday (e.g. Friday), start date
		} else if(isStartDate && mapMonth(inputArray[i]) != 0) {
			isStartDate = false;
			isStartTime = true;
			updateHrDayMon(mapMonth(inputArray[i]), _startHour, _day, _month, _year, _item.eventStartTime[0]);
			LOG(INFO) << "MONTH";
		} else if(isStartDate && (mapWeekDay(inputArray[i]) != 0)) {
			isStartDate = false;
			setDateFromWeekDay(mapWeekDay(inputArray[i]), _day, _month, _year);
			if(isNextWeek) {
				handleNextWeekDay(_day, _month, _year);
			}
			LOG(INFO) << "START WEEKDAY";
			// date/month/year, start date
		} else if(isStartDate && isDelimitedDate(inputArray[i])) {
			isStartDate = false;
			separateDayMonthYear(inputArray[i], _day, _month, _year);
			LOG(INFO) << "START DELIMITED DATE";
		} else if(!isStartDate && mapMonth(inputArray[i]) != 0) {
			if(isEndTime) {
				updateHrDayMon(mapMonth(inputArray[i]), _endHour, _endDay, _endMonth, _endYear, _item.eventEndTime[0]);
			} else {
				updateHrDayMon(mapMonth(inputArray[i]), _startHour, _endDay, _endMonth, _endYear, _item.eventStartTime[0]);
			}
			isEndTime = false;
			LOG(INFO) << "MONTH";
			// weekday, end date
		} else if(!isStartDate && (mapWeekDay(inputArray[i]) != 0)) {
			setDateFromWeekDay(mapWeekDay(inputArray[i]), _endDay, _endMonth, _endYear);
			if(isNextWeek) {
				handleNextWeekDay(_endDay, _endMonth, _endYear);
				handleImplicitNext(_day, _month, _year, _endDay, _endMonth, _endYear);
			}
			LOG(INFO) << "END WEEKDAY";
			// date/month/year, end date
		} else if(!isStartDate && isDelimitedDate(inputArray[i])) {
			separateDayMonthYear(inputArray[i], _endDay, _endMonth, _endYear);
			LOG(INFO) << "END DELIMITED DATE";
			// start time
		} else if(isStartTime && isPossibleTime(inputArray[i])) {
			isStartTime = false;
			separateHourMinute(inputArray[i], _startHour, _startMinute);
			LOG(INFO) << "START TIME";
			// end time
		} else if(!isStartTime && hasDash && isPossibleTime(inputArray[i])) {
			isEndTime = true;
			separateHourMinute(inputArray[i], _endHour, _endMinute);
			LOG(INFO) << "END TIME";
			// duration entered instead of end time
		} else if(!isStartTime && !hasDash && (convertStringToInteger(inputArray[i]) > 0)) {
			isEndTime = true;
			int duration = convertStringToInteger(inputArray[i]);
			_startHour == 24 ? _endHour = 1 : _endHour = _startHour + duration;
			_endMinute = _startMinute;
			LOG(INFO) << "DURATION ADDED FROM START";
			// "m", "p", or "pm" keywords
		} else if(!isEndTime && is12Hour(inputArray[i], _startHour)) {
			LOG(INFO) << "PM OR M, Start Hour";
		} else if(is12Hour(inputArray[i], _endHour)) {
			LOG(INFO) << "PM OR M, End Hour";
		}
		LOG(INFO) << "********************************************";

		//try {
		/*} catch(exception &e) {
		LOG(ERROR) << "Exception Triggered!";
		LOG(ERROR) << e.what();
		}*/

		updateItemFields();
	}
	verifyAllDateTime();

}

int DateTimeParser::mapWeekDay(string weekDay) {
	int weekDayIndex = 0;

	std::map<string, int> weekDays;
	weekDays["monday"] = 1;
	weekDays["mon"] = 1;
	weekDays["tuesday"] = 2;
	weekDays["tue"] = 2;
	weekDays["tues"] = 2;
	weekDays["wednesday"] = 3;
	weekDays["wed"] = 3;
	weekDays["thursday"] = 4;
	weekDays["thur"] = 4;
	weekDays["thurs"] = 4;
	weekDays["friday"] = 5;
	weekDays["fri"] = 5;
	weekDays["saturday"] = 6;
	weekDays["sat"] = 6;
	weekDays["sunday"] = 7;
	weekDays["sun"] = 7;

	std::map<string, int>::iterator iter = weekDays.begin(); 
	bool isMatch = false;
	while((iter != weekDays.end()) && (!isMatch)){
		if(iter->first == weekDay){
			weekDayIndex = iter->second;
			isMatch = true;
		}
		iter++;
	}
	return weekDayIndex;
}

int DateTimeParser::mapMonth(string inputMonth) {
	std::map<string, int> month;
	month["january"] = 1;
	month["jan"] = 1;
	month["february"] = 2;
	month["feb"] = 2;
	month["march"] = 3;
	month["mar"] = 3;
	month["april"] = 4;
	month["apr"] = 4;
	month ["may"] = 5;
	month["june"] = 6;
	month["jun"] = 6;
	month["july"] = 7;
	month["jul"] = 7;
	month["august"] = 8;
	month["aug"] = 8;
	month["september"]= 9;
	month["sep"] = 9;
	month["sept"] = 9;
	month["october"] = 10;
	month["oct"] = 10;
	month["november"] = 11;
	month["nov"] = 11;
	month["novem"] = 11;
	month ["december"] = 12;
	month["dec"] = 12;
	month["decem"] = 12;

	int returnValue = 0;
	bool isFound = false;
	std::map<string, int>::iterator it = month.begin(); 
	while((it!=month.end()) && (!isFound)){
		if(it->first == inputMonth){
			returnValue = it->second;
			isFound = true;
		}
		it++;
	}

	return returnValue;
}

void DateTimeParser::setDateFromWeekDay(int weekDayIndex, int& day, int& month, int& year) {
	day = _dateTime.getCurrentDay();
	month = _dateTime.getCurrentMonth();
	year = _dateTime.getCurrentYear();
	int currentWeekDayIndex = _dateTime.getIntWeekDay(day, month, year);

	int diffInDay;
	if(weekDayIndex == currentWeekDayIndex) {
		diffInDay = 7;
	} else {
		diffInDay = (weekDayIndex - currentWeekDayIndex + 7) % 7;
	}

	day += diffInDay; 
	handleDayOverflow(day, month, year);
}

void DateTimeParser::handleNextWeekDay(int& day, int& month, int& year) {
	day += 7;
	handleDayOverflow(day, month, year);
}

void DateTimeParser::handleDayOverflow(int& day, int& month, int& year) {
	if((day > 31) && (month == 12)) {
		day -= 31;
		month = 1;
		year++;
	}

	if(day > _dateTime.numDaysInMonth(month, year)) {
		day -= _dateTime.numDaysInMonth(month, year);
		month++;
	}
}

void DateTimeParser::handleImplicitNext(
	int& startDay,
	int& startMonth, 
	int& startYear,
	int& endDay,
	int& endMonth,
	int& endYear) {
		if((startDay > endDay) && (startMonth >= endMonth) && (startYear >= endYear)) {
			endDay += 7;
			handleDayOverflow(endDay, endMonth, endYear);
		}
}

void DateTimeParser::updateHrDayMon(int monthNum, int& hour, int& day, int& month, int& year, int& itemHour) {
	year = _dateTime.getCurrentYear();
	day = hour;
	month = monthNum;
	hour = 0;

	itemHour = 0;
}

bool DateTimeParser::isDelimitedDate(string input) {
	size_t dateDelimiterPos = findDateDelimiters(input);
	if (dateDelimiterPos == string::npos) {
		return false;
	}
	return true;
}

bool DateTimeParser::isPossibleTime(string input) {
	if(isDelimitedDate(input)) {
		return false;
	}
	return (convertStringToInteger(input) != 0) ? true : false;
}

bool DateTimeParser::is12Hour(string input, int& hour) {
	if (((input == "p") || (input == "pm")) || ((input == "m") && (hour == 12))) {
		hour += 12;
		//ignores p for 24hr input
		if (!_dateTime.isValidTime(hour, 0) || (hour == 24 && input != "m")) {
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

void DateTimeParser::separateHourMinute(string hourMinute, int& hour, int& minute) {
	char *intEnd;
	hour = (int)strtol(hourMinute.c_str(), &intEnd, 10);
	minute = (int)strtol(intEnd + 1, &intEnd, 10);

	if(*intEnd != 0) {
		minute = 0;
	}
}

void DateTimeParser::verifyAllDateTime() {
	verifyItemDate(_item.eventDate[0], _item.eventDate[1], _item.eventDate[2]);
	verifyItemDate(_item.eventEndDate[0], _item.eventEndDate[1], _item.eventEndDate[2]);
	verifyItemTime(_item.eventStartTime[0], _item.eventStartTime[1]);
	verifyItemTime(_item.eventEndTime[0], _item.eventEndTime[1]);
	updateItemStartDate();
	updateItemEndDate();
	verifyStartEnd(
		_item.eventStartTime[0], 
		_item.eventStartTime[1], 
		_item.eventEndTime[0], 
		_item.eventEndTime[1], 
		_item.eventDate[0], 
		_item.eventDate[1], 
		_item.eventDate[2], 
		_item.eventEndDate[0], 
		_item.eventEndDate[1], 
		_item.eventEndDate[2]);
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
		//throw std::out_of_range(ERROR_INVALID_DATE_INPUT);
	}
}

void DateTimeParser::updateItemStartDate() {
	if((_item.eventDate[0] == 0) && (_item.eventDate[1] == 0) && (_item.eventDate[2] == 0)) {
		_item.eventDate[0] = _dateTime.getCurrentDay();
		_item.eventDate[1] = _dateTime.getCurrentMonth();
		_item.eventDate[2] = _dateTime.getCurrentYear();
	}
}

void DateTimeParser::updateItemEndDate() {
	if((_item.eventEndDate[0] == 0) && (_item.eventEndDate[1] == 0) && (_item.eventEndDate[2] == 0)) {
		_item.eventEndDate[0] = _item.eventDate[0];
		_item.eventEndDate[1] = _item.eventDate[1];
		_item.eventEndDate[2] = _item.eventDate[2];
	}
}

void DateTimeParser::verifyItemTime(int& hour, int& minute) {
	if (!_dateTime.isValidTime(hour, minute)) {
		hour = 0;
		minute = 0;
		//throw std::out_of_range(ERROR_INVALID_TIME_INPUT);
	}
}

void DateTimeParser::verifyStartEnd(
	int startHr, 
	int startMin, 
	int& endHr, 
	int& endMin,
	int startDay,
	int startMonth,
	int startYear,
	int& endDay,
	int& endMonth,
	int& endYear) {

		int tempStartHr = (startHr == 24) ? 0 : startHr;

		//end (year, month, day, hour, minute) isLess than start
		bool isLess[5];
		isLess[0] = (endYear < startYear);
		isLess[1] = (endMonth < startMonth);
		isLess[2] = (endDay < startDay);
		isLess[3] = (endHr < tempStartHr);
		isLess[4] = (endMin < startMin);

		//end (year, month, day, hour, minute) is less or equal to start
		bool isLessEq[5];
		isLessEq[0] = (endYear <= startYear);
		isLessEq[1] = (endMonth <= startMonth);
		isLessEq[2] = (endDay <= startDay);
		isLessEq[3] = (endHr <= tempStartHr);
		isLessEq[4] = (endMin <= startMin);

		bool isError = false;
		if(isLess[0] || (isLessEq[0] && isLess[1]) || (isLessEq[0] && isLessEq[1] && isLess[2])) {
			endYear = 0;
			endMonth = 0;
			endDay = 0;
			isError = true;
		}

		if ((isLessEq[0] && isLessEq[1] && isLessEq[2] && isLess[3]) ||
			(isLessEq[0] && isLessEq[1] && isLessEq[2] && isLessEq[3] && isLess[4])) {
				endHr = 0;
				endMin = 0;
				isError = true;
		}

		if(isError) {
			//throw std::out_of_range(ERROR_INVALID_END_TIME);
		}
}

bool DateTimeParser::isDateKeyWord(string inputWrod){
	bool isKeyWord = false;
	std::vector<string> weekDayContainer;
	weekDayContainer.at(0) = "monday";
	weekDayContainer.at(1) = "tuesday";
	weekDayContainer.at(2) = "wednesday";
	weekDayContainer.at(3) = "thursday";
	weekDayContainer.at(4) = "friday";
	weekDayContainer.at(5) = "saturday";
	weekDayContainer.at(6) = "sunday";

    std::vector<string> monthContainer;
	monthContainer.at(0) = "january";
	monthContainer.at(1) = "february";
	monthContainer.at(3) = "march";
	monthContainer.at(4) = "april";
	monthContainer.at(2) = "may";
	monthContainer.at(2) = "june";
	monthContainer.at(2) = "july";
	monthContainer.at(2) = "august";
	monthContainer.at(2) = "september";
	
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

