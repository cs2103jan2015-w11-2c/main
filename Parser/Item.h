#pragma once
#include <iostream>
#include <sstream>
#include "..\EasyLoggingpp\easylogging++.h"
#include "DateTime.h"

using namespace std;

class Item {
public:
	static const string MESSAGE_UNDATED_TASK;

	DateTime itemDate;
	string event;
	int eventDate[3];
	int eventEndDate[3];
	// 24 hour format, 
	// hour: 1 - 24 (0 == undefined, 24 == 12 am)
	// minute: 0 - 59
	int eventStartTime[2];
	int eventEndTime[2];

	Item(void);

	~Item(void);

	void initializeItem();

	int getHour(int hour);

	string get24HrHour(int hour);

	string getMinute(int minute);

	string get24HrMinute(int minute);

	string getPM(int hour);

	string startDateToString();

	string endDateToString();

	string dateToString();

	string timeToString();

	string timeTo24HrString();

	string toString();

	//for logging
	void logItemValues();
};
