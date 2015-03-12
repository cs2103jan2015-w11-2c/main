#pragma once
#include <sstream>
#include "DateTime.cpp"

using namespace std;

struct ITEM {
public:
	DateTime itemDate;
	string event;
	int eventDate[3];
	int eventStartTime[2];
	int eventEndTime[2];
	int colour;
	bool bold;

public:
	string getMinute(int minute) {
		if(minute == 0) {
			return "00";
		} else {
			return to_string(minute);
		}
	}

	string dateToString() {
		if((eventDate[0] == 0) && (eventDate[1] == 0) && (eventDate[1] == 0)) {
			return "";
		} else {
			ostringstream oss;
			oss << "on" << itemDate.getWeekDay(eventDate[0], eventDate[1], eventDate[2]);
			oss << ", " << eventDate[0] << "/" << eventDate[1] << "/" << eventDate[2];
			return oss.str();
		}
	}

	string timeToString() {
		ostringstream oss;
		if(eventStartTime[0] == -1) {
			return "";
		} else if(eventEndTime[0] != -1) {
			oss << ", from " << eventStartTime[0] << ":" << getMinute(eventStartTime[1]);
			oss << " - " << eventEndTime[0] << ":" << getMinute(eventEndTime[1]);
			return oss.str();
		} else {
			oss << ", at " << eventStartTime[0] << ":" << getMinute(eventStartTime[1]);
		}
	}

	string toString() {
		ostringstream oss;
		oss << event << dateToString() << timeToString() << endl;
		return oss.str();
	}

};