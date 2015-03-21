#pragma once

#include <iostream>
#include <vector>
#include "Item.h"
#include "Command.h"

using namespace std;

class AddItem :public Command {
private:
	Item _input;
	string _message;

public:
	AddItem() {
		_input.event = "";
		_input.eventDate[0] = 0;
		_input.eventDate[1] = 0;
		_input.eventDate[2] = 0;
		_input.eventStartTime[0] = 0;
		_input.eventStartTime[1] = 0;
		_input.eventEndTime[0] = 0;
		_input.eventEndTime[1] = 0;
		_input.colour = 7; 
		_input.bold = false;

		_message = "";
	}

	AddItem(const Item input) {
		_input = input;
		_message = "";
	}

	~AddItem() {
	}

	void executeAction(vector<Item> &vectorStore) {
		vectorStore.push_back(_input);

		char buffer[1000];
		sprintf_s(buffer, SUCCESS_ADDED.c_str(), _input.toString().c_str());
		_message = buffer;
	}

	string getMessage() {
		return _message;
	}

	bool isMatch(const Item item1, const Item item2) {
		if (item1.event != item2.event) {
			return false;
		}
		if (item1.eventDate[0] != item2.eventDate[0]) {
			return false;
		}
		if (item1.eventDate[1] != item2.eventDate[1]) {
			return false;
		}
		if (item1.eventDate[2] != item2.eventDate[2]) {
			return false;
		}
		if (item1.eventStartTime[0] != item2.eventStartTime[0]) {
			return false;
		}
		if (item1.eventStartTime[1] != item2.eventStartTime[1]) {
			return false;
		}
		if (item1.eventEndTime[0] != item2.eventEndTime[0]) {
			return false;
		}
		if (item1.eventEndTime[1] != item2.eventEndTime[1]) {
			return false;
		}
		if (item1.colour != item2.colour) {
			return false;
		}
		if (item1.bold != item2.bold) {
			return false;
		}

		return true;
	}

	void negateAction(vector<Item> &vectorStore) {
		vector<Item>::iterator iter;

		for (iter = vectorStore.begin(); iter != vectorStore.end(); iter++) {
			if (isMatch(*iter, _input)) {
				vectorStore.erase(iter);
				break;
			}
		}
	}

};