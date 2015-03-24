#include <map>
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
	resetDateTime();
	resetItemDateTime();

	for(int i = 0; i < arrSize; i++) {
		LOG(INFO) << "Starting to extract DateTime, round: " << i;

		/*_day = mapToGetDate(inputArray[i],_month);*/

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
bool DateTimeParser::mapWeekDay(string weekDay,int&_month,int&_date,int&_year) {
	string currentWeekDay = _dateTime.getCurrentWeekDay();
	int currentMonth= _dateTime.getCurrentMonth();
	int currentYear = _dateTime.getCurrentYear();
	int spacePos = weekDay.find_first_of(" ");
	int diffInDay;
	bool isMatch = true;

	int currentDay = _dateTime.getCurrentDay();
	//initialise current weekday Number as 0
	int currentWeekDayNo = 0;
	if(currentWeekDay =="monday"){
		currentWeekDayNo = 1;
	}else if(currentWeekDay =="tuesday"){
		currentWeekDayNo = 2;
	}else if(currentWeekDay =="wednesday"){
		currentWeekDayNo = 3;
	}else if(currentWeekDay =="thursday"){
		currentWeekDayNo = 4;
	}else if(currentWeekDay =="friday"){
		currentWeekDayNo = 5;
	}else if(currentWeekDay =="saturday"){
		currentWeekDayNo = 6;
	}else if(currentWeekDay =="sunday"){
		currentWeekDayNo = 7;
	}
	//initialise weekDayNo with a zero value
	int weekDayNo = 0;
	if((weekDay =="monday")||(weekDay =="mon")){
		weekDayNo = 1;
	}else if((weekDay =="tuesday")||(weekDay =="tue")||(weekDay == "tues")){
		weekDayNo = 2;
	}else if((weekDay =="wednesday")||(weekDay =="wed")){
		weekDayNo = 3;
	}else if((weekDay =="thursday")||(weekDay =="thurs")||(weekDay =="thur")){
		weekDayNo = 4;
	}else if((weekDay =="friday")||(weekDay =="fri")){
		weekDayNo = 5;
	}else if((weekDay =="saturday")||(weekDay =="sat")){
		weekDayNo = 6;
	}else if((weekDay =="sunday")||(weekDay =="sun")){
		weekDayNo = 7;
	}else{
		isMatch = false;
	}


	diffInDay = weekDayNo - currentWeekDayNo;

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

int DateTimeParser::mapMonth(string inputMonth){
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
	bool isFound=false;
	std::map<string,int>::iterator it=month.begin(); 

	while((it!=month.end()) && (!isFound)){
		if(it->first == inputMonth){
		returnValue = it->second;
		isFound = true;}
		it++;
	}
    
	if (isFound){
	return returnValue;
	}else{
	return 0;}


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

	if(*intEnd != 0) {
		minute = 0;
	}

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
