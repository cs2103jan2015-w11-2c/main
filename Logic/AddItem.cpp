#pragma once

#include <iostream>
#include <vector>
#include "Item.cpp"
#include "Command.h"

using namespace std;

//CONSTANTS
const string SUCCESS_ADDED = "Added line: \"%s\"\n";

class AddItem :public Command {
private:
	ITEM _input;
	vector<ITEM> _vectorStore;

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

	AddItem(vector<ITEM> vectorStore, const ITEM input) {
		_input = input;
		_vectorStore = vectorStore;
	}

	~AddItem() {
	}

	vector<ITEM> executeAction() {
		_vectorStore.push_back(_input);
		
		return _vectorStore;
	}

	string getMessage() {
		char buffer[1000];
		sprintf_s(buffer, SUCCESS_ADDED.c_str(), _input.event.c_str());
		
		return buffer;
	}
};
