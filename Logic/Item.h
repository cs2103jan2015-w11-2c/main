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
	// 24 hour format
	int eventStartTime[2];
	int eventEndTime[2];
	int colour;
	bool bold;

	Item(void);
	~Item(void);

	int getHour(int hour);
	string getMinute(int minute);
	string getAMPM(int hour);
	string dateToString();
	string timeToString();
	string toString();
};

