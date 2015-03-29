#include "Item.h"

const string Item::MESSAGE_UNDATED_TASK = "Undated";

Item::Item() {
	initializeItem();
}

Item::~Item() {
}

void Item::initializeItem() {
	event = "";
	eventDate[0] = 0;
	eventDate[1] = 0;
	eventDate[2] = 0;
	eventEndDate[0] = 0;
	eventEndDate[1] = 0;
	eventEndDate[2] = 0;
	eventStartTime[0] = 0;
	eventStartTime[1] = 0;
	eventEndTime[0] = 0;
	eventEndTime[1] = 0;
	colour = 0;
	bold = false;
}

int Item::getHour(int hour) {
	if(hour == 24) {
		return 0;
	} else if(hour > 12 && hour < 24) {
		return hour - 12;
	} else {
		return hour;
	}
}

string Item::getMinute(int minute) {
	if (minute < 10) {
		return ("0" + to_string(minute));
	} else {
		return to_string(minute);
	}
}

string Item::getAMPM(int hour) {
	if (hour >= 12 && hour < 24) {
		return "pm";
	} else {
		return "am";
	}
}

string Item::dateToString() {
	if((eventDate[0] == 0) && (eventDate[1] == 0) && (eventDate[1] == 0)) {
		return MESSAGE_UNDATED_TASK;
	} else {
		ostringstream oss;
		oss << itemDate.getWeekDay(eventDate[0], eventDate[1], eventDate[2]);
		oss << ", " << eventDate[0] << " ";
		oss << itemDate.getMonth(eventDate[1]) << " " << eventDate[2];
		return oss.str();
	}
}

string Item::timeToString() {
	ostringstream oss;
	if(eventStartTime[0] == 0) {
		return "";
	} else {
		string startTimeOfDay = getAMPM(eventStartTime[0]);
		oss << "[" << getHour(eventStartTime[0]) << ":" ;
		oss << getMinute(eventStartTime[1]) << " " << startTimeOfDay;

		if(eventEndTime[0] != 0) {
			string endTimeOfDay = getAMPM(eventEndTime[0]);
			oss << " - " << getHour(eventEndTime[0]) << ":";
			oss << getMinute(eventEndTime[1]) << " " << endTimeOfDay;
		}
		oss << "]";

		return oss.str();
	}
}

string Item::toString() {
	ostringstream oss;
	oss << event << ": "<< dateToString() << " " << timeToString();
	return oss.str();
}

void Item::logItemValues() {
	LOG(INFO) << event;
	LOG(INFO) << eventDate[0];
	LOG(INFO) << eventDate[1];
	LOG(INFO) << eventDate[2];
	LOG(INFO) << eventEndDate[0];
	LOG(INFO) << eventEndDate[1];
	LOG(INFO) << eventEndDate[2];
	LOG(INFO) << eventStartTime[0];
	LOG(INFO) << eventStartTime[1];
	LOG(INFO) << eventEndTime[0];
	LOG(INFO) << eventEndTime[1];
}