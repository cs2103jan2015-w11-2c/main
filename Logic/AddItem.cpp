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
};
