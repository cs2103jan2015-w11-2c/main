#pragma once

#include <iostream>
#include <vector>
#include "Item.h"
#include "Command.h"

using namespace std;

class AddItem :public Command {
private:
	Item _input;

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
	}

	AddItem(const Item input) {
		_input = input;
	}

	~AddItem() {
	}

	void executeAction(vector<Item> &vectorStore) {
		vectorStore.push_back(_input);
	}

	string getMessage() {
		char buffer[1000];
		sprintf_s(buffer, SUCCESS_ADDED.c_str(), _input.event.c_str());
		
		return buffer;
	}
};
