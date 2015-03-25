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
	LOG(INFO) << "Before update";

	LOG(INFO) << _item.eventDate[0];
	LOG(INFO) << _item.eventDate[1];
	LOG(INFO) << _item.eventDate[2];
	LOG(INFO) << _item.eventStartTime[0];
	LOG(INFO) << _item.eventStartTime[1];
	LOG(INFO) << _item.eventEndTime[0];
	LOG(INFO) << _item.eventEndTime[1];

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
	LOG(INFO) << "After update:";
	LOG(INFO) << _item.eventDate[0];
	LOG(INFO) << _item.eventDate[1];
	LOG(INFO) << _item.eventDate[2];
	LOG(INFO) << _item.eventStartTime[0];
	LOG(INFO) << _item.eventStartTime[1];
	LOG(INFO) << _item.eventEndTime[0];
	LOG(INFO) << _item.eventEndTime[1];
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
	bool isSecondTimeInstance = false;
	resetDateTime();
	resetItemDateTime();

	for(int i = 0; i < arrSize; i++) {
		LOG(INFO) << "Starting to extract DateTime, round: " << i;

		/*
		// throws exception if weekday is expected but not given
		if(isNextWeek && _day == 0) {
		isNextWeek = false;
		throw std::out_of_range(ERROR_NO_DAY_SPECIFIED);
		}*/

		// throws exception if time is expected but not given
		if(hasDash && !separateHourMinute(inputArray[i], _endHour, _endMinute)) {
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
		} else if(mapWeekDay(inputArray[i], _day, _month, _year)) {
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
			isFirstTimeInstance = false;
			LOG(INFO) << "START TIME";
			// end time
		} else if(!isFirstTimeInstance && hasDash && separateHourMinute(inputArray[i], _endHour, _endMinute)) {
			hasDash = false;
			isSecondTimeInstance = true;
			LOG(INFO) << "END TIME";
			// duration entered instead of end time
		} else if(!isFirstTimeInstance && !hasDash && (convertStringToInteger(inputArray[i]) > 0)) {
			int duration = convertStringToInteger(inputArray[i]);
			_startHour == 24 ? _endHour = 1 : _endHour = _startHour + duration;
			_endMinute = _startMinute;
			isSecondTimeInstance = true;
			LOG(INFO) << "DURATION ADDED FROM START";
			// "m", "p", or "pm" keywords
		} else if(!isSecondTimeInstance && is12Hour(inputArray[i], _startHour)) {
			LOG(INFO) << "PM OR M, Start Hour";
		} else if(is12Hour(inputArray[i], _endHour)) {
			LOG(INFO) << "PM OR M, End Hour";
		}
		LOG(INFO) << "********************************************";

		/*} catch(exception &e) {
		LOG(ERROR) << "Exception Triggered!";
		LOG(ERROR) << e.what();
		}*/

		updateItemFields();
	}
	verifyAllDateTime();

}


bool DateTimeParser::mapWeekDay(string weekDay, int& _date, int &_month, int &_year) {
	int currentMonth= _dateTime.getCurrentMonth();
	int currentYear = _dateTime.getCurrentYear();
	int currentDay = _dateTime.getCurrentDay();
	int weekDayIndex = 0;
	int currentWeekDayIndex = _dateTime.getIntWeekDay(currentDay, currentMonth, currentYear);

	int spacePos = weekDay.find_first_of(" ");

	int diffInDay;

	std::map<string,int> weekDays;
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

	std::map<string,int>::iterator iter = weekDays.begin(); 
	bool isMatch = false;
	while((iter != weekDays.end()) && (!isMatch)){
		if(iter->first == weekDay){
			weekDayIndex = iter->second;
			isMatch = true;
		}
		iter++;
	}

	diffInDay = (weekDayIndex - currentWeekDayIndex + 7) % 7;

	currentDay = currentDay + diffInDay; 

	if((currentDay > 31) && (currentMonth == 12)) {
		currentDay -= 31;
		currentMonth = 1;
		currentYear++;
	}

	if(currentDay > _dateTime.numDaysInMonth(currentMonth, currentYear)) {
		currentMonth++;
		currentDay -= _dateTime.numDaysInMonth(currentMonth, currentYear);
	}

	if(isMatch) {
		_month = currentMonth;
		_year = currentYear;
		_date = currentDay;
		return true;
	} else {
		return false;
	}
}
/*
bool DateTimeParser::mapWeekDay(string weekDay,int&_date,int&_month,int&_year) {
	string currentWeekDay = _dateTime.getCurrentWeekDay();
	int currentMonth= _dateTime.getCurrentMonth();
	int currentYear = _dateTime.getCurrentYear();
	int currentDay = _dateTime.getCurrentDay();
	string weekString = "";
	int weekIndex;
	int weekDayIndex = 0;
	int currentWeekDayIndex  = 0;
	int diffInDay;

	int spacePos = weekDay.find_first_of(" ");
	if(spacePos != string::npos){
		weekString = weekDay.substr(0,spacePos);
		weekDay = weekDay.substr(spacePos+1);
	}
	if ((weekString == "next")||(weekString == "Next")){
		weekIndex = 7;
	}else {
		weekIndex = 0;
	}


	std::map<string,int> weekday;
	weekday["monday"] = 1;
	weekday["Monday"] = 1;
	weekday["mon"] = 1;
	weekday["tuesday"] = 2;
	weekday["Tuesday"] = 2;
	weekday["tue"] = 2;
	weekday["tues"] = 2;
	weekday["wednesday"] = 3;
	weekday["Wednesday"] = 3;
	weekday["wed"] = 3;
	weekday["thursday"] = 4;
	weekday["Thursday"] = 4;
	weekday["thur"] = 4;
	weekday["thurs"] = 4;
	weekday["friday"] = 5;
	weekday["Friday"] = 5;
	weekday["fri"] = 5;
	weekday["saturday"] = 6;
	weekday["Saturday"] = 6;
	weekday["sat"] = 6;
	weekday["sunday"] = 7;
	weekday["Sunday"] = 7;
	weekday["sun"] = 7;

	std::map<string,int>::iterator it1=weekday.begin(); 
	bool isMatch=false;
	while((it1!=weekday.end()) && (!isMatch)){
		if(it1->first == weekDay){
			weekDayIndex = it1->second;
			isMatch = true;
		}
		it1++;
	}


	std::map<string,int>::iterator it2=weekday.begin(); 
	bool isFound = false;
	while((it2!=weekday.end()) && (!isFound)){
		if(it2->first == currentWeekDay){
			currentWeekDayIndex = it2->second;
			isFound = true;
		}
		it2++;
	}

	if(weekDayIndex > currentWeekDayIndex){
		if(weekIndex == 7){
			diffInDay = weekDayIndex - currentWeekDayIndex +7;
		}else if(weekIndex == 0){
			diffInDay = weekDayIndex - currentWeekDayIndex;
		}
	}else if(weekDayIndex == currentWeekDayIndex){
		diffInDay = 7;
	}else{
		diffInDay = weekDayIndex - currentWeekDayIndex + 7;
	}
	currentDay = currentDay + diffInDay; 
	//if the current month have 30 days
	if((currentMonth == 4) || (currentMonth == 6) || (currentMonth == 9) || (currentMonth == 11)) {
		if(currentDay > 30) {
			currentDay = currentDay - 30;
			currentMonth = currentMonth + 1;
		}
		//if current month is feburary in a leap year, there are 29 days,else there are 28 days 
	}else if(currentMonth == 2){
		if(currentYear%4 == 0){
			if(currentDay > 29) {
				currentDay = currentDay - 29;
				currentMonth = currentMonth + 1;
			}
		}else if(currentYear%4 != 0){
			if(currentDay > 28) {
				currentDay = currentDay - 28;
				currentMonth = currentMonth + 1;
			}
		}
		//
	}else if(currentMonth == 12){
		if(currentDay>31){
			currentDay = currentDay - 30;
			currentMonth = 1;
			currentYear=currentYear+1;
		}
	}else{
		if(currentDay > 31) {
			currentDay = currentDay - 31;
			currentMonth = currentMonth + 1;
		}
	}

	_month = currentMonth;
	_year = currentYear;
	_date = currentDay;
	return isMatch;
}

int DateTimeParser::mapMonth(string inputMonth) {
	std::map<string,int> month;
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

	int returnValue;
	bool isFound = false;
	std::map<string,int>::iterator it = month.begin(); 

	while((it!=month.end()) && (!isFound)){
		if(it->first == inputMonth){
			returnValue = it->second;
			isFound = true;}
		it++;
	}

	if (isFound) {
		return returnValue;
	} else {
		return 0;
        }

}*/

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

bool DateTimeParser::separateHourMinute(string hourMinute, int& hour, int& minute) {
	char *intEnd;
	hour = (int)strtol(hourMinute.c_str(), &intEnd, 10);
	minute = (int)strtol(intEnd + 1, &intEnd, 10);

	if(*intEnd != 0) {
		minute = 0;
	}

	return (hour != 0);
}

void DateTimeParser::verifyAllDateTime() {
	verifyItemDate(_item.eventDate[0], _item.eventDate[1], _item.eventDate[2]);
	verifyItemTime(_item.eventStartTime[0], _item.eventStartTime[1]);
	verifyItemTime(_item.eventEndTime[0], _item.eventEndTime[1]);
	verifyStartEndTime(_item.eventStartTime[0], _item.eventStartTime[1], _item.eventEndTime[0], _item.eventEndTime[1]);
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

void DateTimeParser::verifyItemTime(int& hour, int& minute) {
	if (!_dateTime.isValidTime(hour, minute)) {
		hour = 0;
		minute = 0;
		//throw std::out_of_range(ERROR_INVALID_TIME_INPUT);
	}
}

void DateTimeParser::verifyStartEndTime(int startHr, int startMin, int& endHr, int& endMin) {
	int tempStartHr;
	startHr == 24 ? tempStartHr = 0 : tempStartHr = startHr;
	if((endHr < tempStartHr) || ((endHr <= tempStartHr) && (endMin <= startMin))) {
		endHr = 0;
		endMin = 0;
		//throw std::out_of_range(ERROR_INVALID_END_TIME);
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
