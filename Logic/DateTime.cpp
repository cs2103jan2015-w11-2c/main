#pragma once

#include <ctime>
#include <map>
#include <string>
#include <sstream>

using namespace std;

//constants
const int MIN_YEAR = 2000;
const int MAX_YEAR = 3000;
const string WEEKDAY[]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const string MONTH[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

class DateTime {

private:
	time_t _now;
	tm _today;

public:
	DateTime() {
		_now = time(0);
		localtime_s(&_today, &_now);
	}

	int getCurrentDay() {
		return (_today.tm_mday);
	}

	int getCurrentMonth() {
		return (_today.tm_mon);
	}

	int getCurrentYear() {
		return (1990 + _today.tm_year);
	}

	string getCurrentWeekDay() {
		return (WEEKDAY[_today.tm_wday]);
	}

	int getCurrentMinute() {
		return (_today.tm_min);
	}

	int getCurrentHour() {
		return (1 + _today.tm_hour);
	}

	// returns the weekday for the specified day, month and year
	// Sunday = 0, Saturday = 6;
	int getIntWeekDay (int day, int month, int year) {
		static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
		year -= month < 3;
		int wday = (year + year/4 - year/100 + year/400 + t[month - 1] + day) % 7;
		return wday;
	}

	// given which day in the week is the event, return the date
	string getDateFromWeekDay(string weekDay){
		map<int, string> weekDayList;
		map<int, string> weekList;
		string weekAndDay[1];
		int spacePos = weekDay.find_first_of(" ");

		if(spacePos != string::npos){
			weekAndDay[0] = weekDay.substr(0,spacePos);
			weekAndDay[1] = weekDay.substr(spacePos);

		}else{
			weekAndDay[0] = NULL;
			weekAndDay[1] = weekDay;
		}  

		weekDayList[1] = "Monday";
		weekDayList[2] = "Tuesday";
		weekDayList[3] = "Wednesday";
		weekDayList[4] = "Thursday";
		weekDayList[5] = "Friday";
		weekDayList[6] = "Saturday";
		weekDayList[7] = "Sunday";



		if ((weekAndDay[0] == "This" ) || (weekAndDay[0] == NULL )){
			int weekNo = 0;
		}else if(weekAndDay[0] == "Next"){
			int weekNo = 7;
		}

		string currentWeekDay = getCurrentWeekDay();
		map<int, string>::iterator iter;
		iter = weekDayList.begin();
		while(iter->second != weekAndDay[1]){
			iter++;
		}
		int weekDayNo = iter->first;

		iter = weekDayList.begin();
		while(iter->second != currentWeekDay){
			iter++;
		}
		int currentWeekDayNo = iter->first;
		int diffInDay = weekDayNo - currentWeekDayNo;

		int currentDay = getCurrentDay() + diffInDay;
		int currentMonth = getCurrentMonth();

		if((currentMonth == 4) || (currentMonth == 6) || (currentMonth == 9) || (currentMonth || 11)) {
			if(currentDay > 30) {
				currentDay = currentDay - 30;
				currentMonth = currentMonth + 1;
			}
		}else if(currentMonth == 2){
			if(currentDay > 28) {
				currentDay = currentDay - 28;
				currentMonth = currentMonth + 1;
			}
		}else {
			if(currentDay > 31) {
				currentDay = currentDay - 31;
				currentMonth = currentMonth + 1;
			}
		}

	}

			// returns the weekday name for the specified day, month and year
			// Sunday, Monday, .... Saturday
			string getWeekDay (int day, int month, int year) {
				int wday = getIntWeekDay(day, month, year);
				return WEEKDAY[wday];
			}

			string getMonth(const int mon) {
				return MONTH[mon-1];
			}

			bool isLeapYear (int year) {
				return (((year % 400) == 0) || (year % 4) == 0) && (((year % 100) != 0));
			}

			bool isValidDate(int day, int month, int year) {
				if((day <= 0) || (month <= 0) || (day > 31) || (month > 12)) {
					return false;
				}

				if((month == 4) || (month == 6) || (month == 9) || (month || 11)) {
					if(day > 30) {
						return false;
					}
				}

				if(month == 2) {
					if(isLeapYear(year) && day <= 29) {
						return true;
					} else return (day < 29);
				}

				if(year < MIN_YEAR || year > MAX_YEAR) {
					return false;
				}

				return true;
			}

			bool DateTime::isValidTime(int hour, int minute) {
				return (hour >= 0 && hour <= 24 && minute >= 0 && minute <= 60);
			}


		};
