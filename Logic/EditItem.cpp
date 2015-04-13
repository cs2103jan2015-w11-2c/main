#pragma once

//@author A0116179B

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
		if(_lineNumber <= 0   || _lineNumber > (int)vectorStore.size()) {
			_message = ERROR_INVALID_LINE_NUMBER + " " + to_string(_lineNumber);
			throw std::out_of_range(_message);
		} else {
			char buffer[1000];
			sprintf_s(buffer, SUCCESS_EDITED.c_str(), 
				vectorStore[_lineNumber - 1].toString().c_str(), 
				_input.toString().c_str());
			_message=buffer;
			_editedItem = vectorStore[_lineNumber - 1];

			if (_input.event != "") {
				vectorStore[_lineNumber - 1].event = _input.event;
			}
			if (!isFloatingDate()) {
				for (int i = 0; i < 3; i++) {
					vectorStore[_lineNumber - 1].eventDate[i] = _input.eventDate[i];
					vectorStore[_lineNumber - 1].eventEndDate[i] = _input.eventEndDate[i];
					_dateHasChanged = true;
				}
			} else {
				if (_input.isDeadlineTask && 
					(vectorStore[_lineNumber - 1].eventEndDate[0] != 0 
					|| vectorStore[_lineNumber - 1].eventEndDate[1] != 0
					|| vectorStore[_lineNumber - 1].eventEndDate[2] != 0)) {
							
					vectorStore[_lineNumber - 1].eventDate[0] = vectorStore[_lineNumber - 1].eventEndDate[0];
					vectorStore[_lineNumber - 1].eventDate[1] = vectorStore[_lineNumber - 1].eventEndDate[1];
					vectorStore[_lineNumber - 1].eventDate[2] = vectorStore[_lineNumber - 1].eventEndDate[2];

					_dateHasChanged = true;
				}
			}
			if (!isFloatingTime()) {
				for (int i = 0; i < 1; i++) {
					vectorStore[_lineNumber-1].eventStartTime[i] = _input.eventStartTime[i];
					vectorStore[_lineNumber-1].eventEndTime[i] = _input.eventEndTime[i];
					_timeHasChanged = true;
				}
			} else {
				if (_input.isDeadlineTask && 
					(vectorStore[_lineNumber - 1].eventEndTime[0] != 0 
					|| vectorStore[_lineNumber - 1].eventEndTime[1] != 0)) {
							
					vectorStore[_lineNumber - 1].eventStartTime[0] = vectorStore[_lineNumber - 1].eventEndTime[0];
					vectorStore[_lineNumber - 1].eventStartTime[1] = vectorStore[_lineNumber - 1].eventEndTime[1];
					vectorStore[_lineNumber - 1].eventEndTime[0] = 0;
					vectorStore[_lineNumber - 1].eventEndTime[1] = 0;
					_timeHasChanged = true;
				}
			}
			vectorStore[_lineNumber - 1].isDeadlineTask = _input.isDeadlineTask;

			_input.event = vectorStore[_lineNumber - 1].event;
			_input.eventDate[0] = vectorStore[_lineNumber - 1].eventDate[0];
			_input.eventDate[1] = vectorStore[_lineNumber - 1].eventDate[1];
			_input.eventDate[2] = vectorStore[_lineNumber - 1].eventDate[2];
			_input.eventEndDate[0] = vectorStore[_lineNumber - 1].eventEndDate[0];
			_input.eventEndDate[1] = vectorStore[_lineNumber - 1].eventEndDate[1];
			_input.eventEndDate[2] = vectorStore[_lineNumber - 1].eventEndDate[2];
			_input.eventStartTime[0] = vectorStore[_lineNumber - 1].eventStartTime[0]; 
			_input.eventStartTime[1] = vectorStore[_lineNumber - 1].eventStartTime[1];
			_input.eventEndTime[0] = vectorStore[_lineNumber - 1].eventEndTime[0];
			_input.eventEndTime[1] = vectorStore[_lineNumber - 1].eventEndTime[1];
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
