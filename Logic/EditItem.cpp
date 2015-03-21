#pragma once

#include <iostream>
#include <vector>
#include "Item.h"
#include "Command.h"

using namespace std;

class EditItem :public Command {
private:
	int _lineNumber;
	Item _input;
	string _message;

public:
	EditItem() {
		_lineNumber = 0;
		_message = "";
	}

	EditItem(int lineNumber, Item input) {
		_lineNumber = lineNumber;
		_input = input;
		_message = "";
	}

	~EditItem() {
	}

	void executeAction(vector<Item> &vectorStore) {
		if(_lineNumber == 0   || _lineNumber > vectorStore.size()) {
			_message = ERROR_INVALID_LINE_NUMBER + " ";
			_message += ('0'+_lineNumber);
			_message += "\n";
		} else {
			char buffer[1000];
			sprintf_s(buffer, SUCCESS_EDITED.c_str(), 
				vectorStore[_lineNumber-1].toString(), 
				_input.toString().c_str());
			_message=buffer;

			vectorStore[_lineNumber-1] = _input;
		}
	}

	string getMessage() {

		return _message;
	}
};
