#pragma once
#include <sstream>
#include "DateTime.cpp"

using namespace std;

class ITEM {
public:
	DateTime itemDate;
	string event;
	int eventDate[3];
	int eventStartTime[2];
	int eventEndTime[2];
	int colour;
	bool bold;

public:
	ITEM() {
		event = "";
		eventDate[0] = 0;
		eventDate[1] = 0;
		eventDate[2] = 0;
		eventStartTime[0] = -1;
		eventStartTime[1] = 0;
		eventEndTime[0] = -1;
		eventEndTime[1] = 0;
		colour = 0;
		bold = false;

	}

	~ITEM() {
	}

	string getMinute(int minute) {
		if(minute == 0) {
			return "00";
		} else {
			return to_string(minute);
		}
	}

	// add event to string
    string eventToString(){
	
	
	}

	
	
	string dateToString() {
		if((eventDate[0] == 0) && (eventDate[1] == 0) && (eventDate[2] == 0)) {
			return "";

		} else if((eventDate[0] == 0)&&(eventDate[1] != 0) && (eventDate[2] != 0)) {
			ostringstream oss;
			oss << ", " << eventDate[0] << "/" << eventDate[1] << "/" << eventDate[2];
			return oss.str();
		}else if(){
		ostringstream oss;
			oss << "on" << itemDate.getWeekDay(eventDate[0], eventDate[1], eventDate[2]);
			oss << ", " << eventDate[0] << "/" << eventDate[1] << "/" << eventDate[2];
			return oss.str();}
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