#include "Item.h"

const string Item::MESSAGE_UNDATED_TASK = "Floating";

Item::Item() {
	initializeItem();
}

Item::~Item() {
}

void Item::initializeItem() {
	// Load configuration from file
	el::Configurations conf("logging.conf");

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

//@author A0111951N
int Item::getHour(int hour) {
	if(hour == 24) {
		return 0;
	} else if(hour > 12 && hour < 24) {
		return hour - 12;
	} else {
		return hour;
	}
}

string Item::getDateDuration() {
	int startDate = itemDate.rataDieConvert(eventDate[0], eventDate[1], eventDate[2]);
	int endDate = itemDate.rataDieConvert(eventEndDate[0], eventEndDate[1], eventEndDate[2]);
	return to_string(endDate - startDate);
}

string Item::getMinute(int minute) {
	if(minute == 0) {
		return "";
	} else if (minute < 10) {
		return (":0" + to_string(minute));
	} else {
		return (":" + to_string(minute));
	}
}

string Item::get24HrMinute(int minute) {
	if(minute == 0) {
		return "";
	} else if (minute < 10) {
		return ("0" + to_string(minute));
	} else {
		return (to_string(minute));
	}
}

string Item::getPM(int hour) {
	if (hour >= 12 && hour < 24) {
		return "p";
	} else {
		return "";
	}
}

//@author A0114613U
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

string Item::endDateToString() {
	if((eventEndDate[0] == 0) && (eventEndDate[1] == 0) && (eventEndDate[1] == 0)) {
		return MESSAGE_UNDATED_TASK;
	} else {
		ostringstream oss;
		oss << eventEndDate[0] << "/" << eventEndDate[1] << "/" << eventEndDate[2];
		return oss.str();
	}
}

//@author A0111951N
string Item::timeToString() {
	ostringstream oss;
	string startTimeOfDay = getPM(eventStartTime[0]);
	string endTimeOfDay = getPM(eventEndTime[0]);

	if(eventStartTime[0] == 0) {
		return "";
	} else {
		oss << "[" << getHour(eventStartTime[0]);
		oss << getMinute(eventStartTime[1]) <<  startTimeOfDay;

		if(eventEndTime[0] != 0) {
			oss << "-" << getHour(eventEndTime[0]);
			oss << getMinute(eventEndTime[1]) << endTimeOfDay;
		}
		oss << "]";

		return oss.str();
	}
}

string Item::timeTo24HrString() {
	ostringstream oss;

	if(eventStartTime[0] == 0) {
		return "";
	} else {
		oss << "[" << getHour(eventStartTime[0]) << get24HrMinute(eventStartTime[1]);;
		if(eventEndTime[0] != 0) {
			oss << "-" << getHour(eventEndTime[0]) << get24HrMinute(eventEndTime[1]);
		}
		oss << "]";

		return oss.str();
	}
}

string Item::durationToString() {
	if(getDateDuration() != "0") {
		return ("[+" + getDateDuration() + "]");
	} else {
		return "";
	}
}

string Item::toString() {
	ostringstream oss;
	oss << event << ": "<< dateToString() << " " << timeToString();
	return oss.str();
}


void Item::logItemValues() {
	LOG(DEBUG) << event;
	LOG(DEBUG) << eventDate[0];
	LOG(DEBUG) << eventDate[1];
	LOG(DEBUG) << eventDate[2];
	LOG(DEBUG) << eventEndDate[0];
	LOG(DEBUG) << eventEndDate[1];
	LOG(DEBUG) << eventEndDate[2];
	LOG(DEBUG) << eventStartTime[0];
	LOG(DEBUG) << eventStartTime[1];
	LOG(DEBUG) << eventEndTime[0];
	LOG(DEBUG) << eventEndTime[1];
}