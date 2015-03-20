#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "Item.h"
#include "Command.h"

using namespace std;

class SearchItem :public Command {
private:
	string _input;
	string _message;

public:
	SearchItem() {
		_input = "";
	}
	SearchItem(const string input) {
		_input = input;
	}

	~SearchItem() {
	}

	void executeAction(vector<Item> &vectorStore) {
		vector<Item> temp;

		transform(_input.begin(), _input.end(), _input.begin(), ::tolower);
		
		for (unsigned int i = 0; i < vectorStore.size(); i++) {
			string currentString = vectorStore[i].event;
			transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
			size_t position = currentString.find(_input);
			if (position != string::npos) {
				temp.push_back(vectorStore[i]);
			}
		}

		char buffer[1000];
		_message = "";
		if (temp.size()==0) {
			sprintf_s(buffer, ERROR_SEARCH_ITEM_NOT_FOUND.c_str(), _input.c_str());
			_message = buffer;
		}
	}

	string getMessage() {
		return _message;
	}
};
