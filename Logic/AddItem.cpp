#pragma once

//author A0116179B
#include <iostream>
#include <vector>
#include "Item.h"
#include "Command.h"

using namespace std;

class AddItem : public Command {
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
		sprintf_s(buffer, SUCCESS_ADDED.c_str(), _input.event.c_str());
		_message = buffer;
	}

	string getMessage() {
		return _message;
	}

	bool isMatch(const Item item1, const Item item2) {
		if (item1.event != item2.event) {
			return false;
		}
		for (int i = 0; i < 3; i++) {
			if (item1.eventDate[i] != item2.eventDate[i]) {
				return false;
			}
		}
		for (int i = 0; i < 2; i++) {
			if (item1.eventStartTime[i] != item2.eventStartTime[i]) {
				return false;
			}
		}
		for (int i = 0; i < 2; i++) {
			if (item1.eventEndTime[i] != item2.eventEndTime[i]) {
				return false;
			}
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