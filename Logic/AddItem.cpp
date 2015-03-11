#pragma once

#include <iostream>
#include <vector>
#include "Command.cpp"

using namespace std;

//CONSTANTS
const string SUCCESS_ADDED = "Added line: \"%s\"\n";

class AddItem : public Command {
private:
	string _input;
	vector<string> _vectorStore;

public:
	AddItem() {
		_input="";
	}

	AddItem(vector<string> vectorStore, const string input) {
		_input = input;
		_vectorStore = vectorStore;
	}

	~AddItem() {
	}

	vector<string> executeAction() {
		_vectorStore.push_back(_input);
		
		return _vectorStore;
	}

	string getMessage() {
		char buffer[1000];
		sprintf_s(buffer, SUCCESS_ADDED.c_str(), _input.c_str());
		
		return buffer;
	}
};
