#pragma once
#include <iostream>
#include <sstream>
#include "DateTime.cpp"

using namespace std;

class Item {
public:
	DateTime itemDate;
	string event;
	int eventDate[3];
	// 24 hour format, 
	// hour: 1 - 24 (0 == undefined, 24 == 12 am)
	// minute: 0 - 59
	int eventStartTime[2];
	int eventEndTime[2];
	int colour;
	bool bold;

	Item(void);
	~Item(void);

	void initilizeItem();
	int getHour(int hour);
	string getMinute(int minute);
	string getAMPM(int hour);
	string dateToString();
	string timeToString();
	string toString();
};

