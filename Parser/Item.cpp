#include "Item.h"
//@author A0111951N
const string Item::MESSAGE_UNDATED_TASK = "Floating";

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

string Item::get24HrHour(int hour) {
	if (hour == 24) {
		return "00";
	} else if (hour < 10) {
		return "0" + to_string(hour);
	} else {
		return to_string(hour);
	}
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
		return "00";
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

string Item::startDateToString() {
	ostringstream oss;
	oss << "[" << eventDate[0] << "/" << eventDate[1] << "/" << eventDate[2] << "]";
	return oss.str();
}

string Item::endDateToString() {
	if (eventEndDate[0] == 0 && eventEndDate[1] == 0 && eventEndDate[2] == 0) {
		return "";
	}
	ostringstream oss;
	oss << "[" << eventEndDate[0] << "/" << eventEndDate[1] << "]";
	return oss.str();
}

//@author A0115452N
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

string Item::timeAndEndDateToString() {
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

		if (eventEndDate[0] != 0 && eventEndDate[1] != 0 && eventEndDate[2] != 0) {
			oss << ", " << eventEndDate[0] << "/" << eventEndDate[1];
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
		oss << "[" << get24HrHour(eventStartTime[0]) << get24HrMinute(eventStartTime[1]);;
		if(eventEndTime[0] != 0) {
			oss << "-" << get24HrHour(eventEndTime[0]) << get24HrMinute(eventEndTime[1]);
		}

		if (eventEndDate[0] != 0 && eventEndDate[1] != 0 && eventEndDate[2] != 0) {
			oss << ", " << eventEndDate[0] << "/" << eventEndDate[1];
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


//@author A0116179B
bool Item::isFloating() {
	for (int i = 0; i < 3; i++) {
		if (eventDate[i] != 0 || eventEndDate[i] != 0) {
			return false;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (eventStartTime[i] != 0 || eventEndTime[i] != 0) {
			return false;
		}
	}
	return true;
}

bool Item::isDeadline() {
	for (int i = 0; i < 3; i++) {
		if (eventEndDate[i] != 0) {
			return false;
		}
	}

	for (int i = 0; i < 2; i++) {
		if (eventEndTime[i] != 0) {
			return false;
		}
	}

	for (int i = 0; i < 3; i++) {
		if (eventDate[i] != 0) {
			return true;
		}
	}
	return false;
}

//@author A0111951N
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
