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
	Item _editedItem;
	bool _eventHasChanged;
	bool _timeHasChanged;
	bool _dateHasChanged;

public:
	EditItem() {
		_lineNumber = 0;
		_message = "";
		_eventHasChanged = false;
		_timeHasChanged = false;
		_dateHasChanged = false;
	}

	EditItem(int lineNumber, Item input) {
		_lineNumber = lineNumber;
		_input = input;
		_message = "";
		_eventHasChanged = false;
		_timeHasChanged = false;
		_dateHasChanged = false;
	}

	~EditItem() {
	}

	bool isFloatingDate() {
		for (int i = 0; i < 3; i++) {
			if (_input.eventDate[i] != 0) {
				return false;
			}
			if (_input.eventEndDate[i] !=	0) {
				return false;
			}
		}
		return true;
	}

	bool isFloatingTime() {
		for (int i = 0; i < 2; i++) {
			if (_input.eventStartTime[i] != 0) {
				return false;
			}
			if (_input.eventStartTime[i] !=	0) {
				return false;
			}
		}
		return true;
	}

	void executeAction(vector<Item> &vectorStore) {
		if(_lineNumber == 0   || _lineNumber > (int)vectorStore.size()) {
			_message = ERROR_INVALID_LINE_NUMBER + " ";
			_message += ('0' + _lineNumber);
			_message += "\n";
		} else {
			char buffer[1000];
			sprintf_s(buffer, SUCCESS_EDITED.c_str(), 
				vectorStore[_lineNumber - 1].toString().c_str(), 
				_input.toString().c_str());
			_message=buffer;
			_editedItem = vectorStore[_lineNumber - 1];
			if (_input.event == "") {
				if (!isFloatingDate()) {
					for (int i = 0; i < 3; i++) {
						vectorStore[_lineNumber-1].eventDate[i] = _input.eventDate[i];
						vectorStore[_lineNumber-1].eventEndDate[i] = _input.eventEndDate[i];
						_dateHasChanged = true;
					}
				}
				if (!isFloatingTime()) {
					for (int i = 0; i < 1; i++) {
						vectorStore[_lineNumber-1].eventStartTime[i] = _input.eventStartTime[i];
						vectorStore[_lineNumber-1].eventEndTime[i] = _input.eventEndTime[i];
						_timeHasChanged = true;
					}
				}
			} else {
				vectorStore[_lineNumber - 1].event = _input.event;
				_eventHasChanged = true;

				if (!isFloatingDate()) {
					for (int i = 0; i < 3; i++) {
						vectorStore[_lineNumber-1].eventDate[i] = _input.eventDate[i];
						vectorStore[_lineNumber-1].eventEndDate[i] = _input.eventEndDate[i];
						_dateHasChanged = true;
					}
				}
				if (!isFloatingTime()) {
					for (int i = 0; i < 1; i++) {
						vectorStore[_lineNumber-1].eventStartTime[i] = _input.eventStartTime[i];
						vectorStore[_lineNumber-1].eventEndTime[i] = _input.eventEndTime[i];
						_timeHasChanged = true;
					}
				}
			}
		}
	}

	string getMessage() {

		return _message;
	}

	bool isMatch(const Item item1, const Item item2) {
		if (_eventHasChanged && item1.event != item2.event) {
			return false;
		}
		if (_dateHasChanged) {
			for (int i = 0; i < 3; i++) {
				if (item1.eventDate[i] != item2.eventDate[i]) {
					return false;
				}
			}
			for (int i = 0; i < 3; i++) {
				if (item1.eventEndDate[i] != item2.eventEndDate[i]) {
					return false;
				}
			}
		}
		if (_timeHasChanged) {
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
		for (unsigned int i = 0; i < vectorStore.size(); i++) {
			if (isMatch(_input, vectorStore[i])) {
				vectorStore[i] = _editedItem;
				break;
			}
		}
	}
};
