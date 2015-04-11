#pragma once

//@author A0111951N
#include <iostream>
#include <vector>
#include <sstream>
#include "Item.h"
#include "Command.h"

using namespace std;

class DisplayItems :public Command {	
public:
	DisplayItems() {
	}

	~DisplayItems() {
	}

	void executeAction(const vector<Item> vectorStore, string &output) {
		if (vectorStore.empty()) {
			output = ERROR_FILE_EMPTY;
			return;
		}
		else {
			ostringstream oss;
			for (unsigned int i = 0; i < vectorStore.size(); i++) {
				DateTime *myDateTime = new DateTime;

				string event = vectorStore[i].event;
				int day = vectorStore[i].eventDate[0];
				int mon = vectorStore[i].eventDate[1];
				int hour = vectorStore[i].eventStartTime[0];
				int min = vectorStore[i].eventStartTime[1];
				int year = myDateTime->getCurrentYear();
		
				//NUMERICAL FORMAT
				//oss << (i + 1) << ". " << event;
				//oss << " [" << day << "/" << mon;
				//oss << ", " << hour << ":" << min << "]";
				//oss << endl << endl;

				//WRITTEN FORMAT
				oss << (i + 1) << ". " << event;
				oss << " on " << myDateTime->getWeekDay(day, mon, year);
				oss << ", " << day << " " << myDateTime->getMonth(mon);
				oss << " at " << hour << ":" << min;
				oss << endl << endl;
			}
		
		output =  oss.str();
		}
	}

	string getMessage() {
		return "";
	}
};
