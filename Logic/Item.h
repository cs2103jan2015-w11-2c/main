#pragma once
#include <iostream>
#include <sstream>
#include "DateTime.cpp"

using namespace std;

class Item
{
public:
	DateTime itemDate;
	string event;
	int eventDate[3];
	int eventStartTime[2];
	int eventEndTime[2];
	int colour;
	bool bold;

	Item(void);
	~Item(void);
	string getMinute(int minute);
	string dateToString();
	string timeToString();
	string toString();
};

