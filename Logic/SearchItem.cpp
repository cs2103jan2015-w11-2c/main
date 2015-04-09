#pragma once

//author A0116179B
#include <iostream>
#include <vector>
#include <algorithm>
#include "Item.h"
#include "Command.h"

const int initialMinEditDist = -1;
const int powerSearchLowThreshold = 1;
const int powerSearchHighThreshold = 1;

using namespace std;

struct SEARCHRESULT {
	int editDistance;
	RESULT result;
};

class SearchItem :public Command {
private:
	Item _input;
	string _message;
	vector<RESULT> *_otherResult;
	bool _isAcronym;

public:
	SearchItem() {
		_message = "";
		_isAcronym = false;
	}
	SearchItem(const Item input, const string message, vector<RESULT> *otherResult) {
		_input = input;
		_message = message;
		_otherResult = otherResult;
		_isAcronym = false;
	}

	~SearchItem() {
	}

	int getEditDist(const string input, const string item) {
		int itemSize=item.size();
		int inputSize=input.size();
		int upLeftCell;

		if(inputSize==0)
			return itemSize;
		if(itemSize==0)
			return inputSize;

		int *difference = new int [inputSize+1];

		for(int i=0;i<=inputSize;i++) {
			difference[i]=i;
		}

		for(int i=1;i<=itemSize;i++) {
			difference[0]=i;
			upLeftCell=i-1;
			for(int j=1;j<=inputSize;j++) {
				if(input[j-1]==item[i-1]) {
					int temp=upLeftCell;
					upLeftCell=difference[j];
					difference[j]=temp;
				}
				else {
					int minimum(upLeftCell<difference[j-1]?upLeftCell:difference[j-1]);
				
					int temp=difference[j];
					if(minimum<difference[j])
						difference[j]=minimum;
					upLeftCell=temp;
					difference[j]++;
				}
			}
		}
		return difference[inputSize];
	}

	void powerSearch(vector<RESULT> *resultVector) {
		vector<SEARCHRESULT> powerSearchLow;
		vector<SEARCHRESULT> normalSearch;
		vector<SEARCHRESULT> powerSearchHigh;

		string fullInput = _input.event;

		transform(fullInput.begin(), fullInput.end(), fullInput.begin(), ::tolower);
		
		for (unsigned int i = 0; i < resultVector->size(); i++) {
			string currentString = resultVector->at(i).event;
			transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
			
			bool isFound = false;
			int minEditDist = initialMinEditDist;

			string inputWord;
			istringstream issInput(fullInput);
			while (issInput >> inputWord) {
				size_t position = currentString.find(fullInput);
				if (!isFound && position != string::npos) {
					isFound = true;
				}

				string currentWord;
				istringstream issWord(currentString);
				while (issWord >> currentWord) {
					int editDist = getEditDist(inputWord, currentWord);
					if(minEditDist < 0 || minEditDist > editDist) {
						minEditDist = editDist;
					}
				}
			}
			SEARCHRESULT newSearchResult;
			newSearchResult.result = resultVector->at(i);
			newSearchResult.editDistance = minEditDist;
			if (minEditDist < powerSearchLowThreshold) {
				unsigned int j = 0;
				while (j < powerSearchLow.size() && powerSearchLow[j].editDistance <= minEditDist) {
					j++;
				}
				powerSearchLow.insert(powerSearchLow.begin() + j, newSearchResult);
			} else if (isFound) {
				unsigned int j = 0;
				while (j < normalSearch.size() && normalSearch[j].editDistance <= minEditDist) {
					j++;
				}
				normalSearch.insert(normalSearch.begin() + j, newSearchResult);
			} else if (minEditDist < powerSearchHighThreshold) {
				unsigned int j = 0;
				while (j < powerSearchHigh.size() && powerSearchHigh[j].editDistance <= minEditDist) {
					j++;
				}
				powerSearchHigh.insert(powerSearchHigh.begin() + j, newSearchResult);
			}
		}

		vector<SEARCHRESULT> temp;
		temp.insert(temp.end(), powerSearchLow.begin(), powerSearchLow.end());
		temp.insert(temp.end(), normalSearch.begin(), normalSearch.end());
		temp.insert(temp.end(), powerSearchHigh.begin(), powerSearchHigh.end());

		resultVector->clear();
		for (unsigned int i = 0; i < temp.size(); i++) {
			resultVector->push_back(temp[i].result);
		}
	}

	bool isSameStartDateAndTime(const Item item, const Item input) {
		for (int i = 0; i < 3; i++) {
			if(item.eventDate[i] != input.eventDate[i]) {
				return false;
			}
		}

		for (int i = 0; i < 2; i++) {
			if(input.eventStartTime[i] != 0 && item.eventStartTime[i] != input.eventStartTime[i]) {
				return false;
			}
		}
		return true;
	}

	int compareDateEarlierThan(const int date1[3], const int date2[3]) {
		if (date1[2] < date2[2]) {
			return -1;
		} else if (date1[2] == date2[2]) {
			if (date1[1] < date2[1]) {
				return -1;
			} else if (date1[1] == date2[1]) {
				if (date1[0] < date2[0]) {
					return -1;
				} else if (date1[0] == date2[0]) {
					return 0;
				}
			}
		}
		return 1;
	}

	int compareTimeEarlierThan(const int time1[2], const int time2[2]) {
		if (time1[1] < time2[1]) {
			return -1;
		} else if (time1[1] == time2[0]) {
			if (time1[0] < time2[0]) {
				return -1;
			} else if (time1[0] == time2[0]) {
					return 0;
			}
		}
		return 1;
	}

	bool timeIsSpecified(const int timeArray[2]) {
		for (int i = 0; i < 2; i++) {
			if (timeArray[i] != 0) {
				return true;
			}
		}
		return false;
	}

	bool isWithinRange(const Item item, const Item input) {
		
		//check if item starts earlier than input start date
		if(compareDateEarlierThan(item.eventDate, input.eventDate) == -1) {
			return false;
		}
		
		//check if item ends later than input end date
		if(compareDateEarlierThan(item.eventDate, input.eventDate) == 1) {
			return false;
		}

		//check if item starts earlier than input start time and is specified
		if (!timeIsSpecified(item.eventStartTime) 
			|| compareTimeEarlierThan(item.eventStartTime, input.eventStartTime) == -1) {
				return false;
		}
		
		//check if item ends later than input end time and is specified
		if (!timeIsSpecified(item.eventEndTime) 
			|| compareTimeEarlierThan(item.eventEndTime, input.eventEndTime) == 1) {
				return false;
		}
		return true;
	}

	void filterDateAndTime(vector<Item> &vectorStore, bool hasStartDate, bool hasEndDate) {
		_otherResult->clear();

		if (!hasStartDate && !hasEndDate) {
			for (unsigned int i = 0; i < vectorStore.size(); i++) {
				RESULT temp;
				temp.event = vectorStore[i].event;
				temp.date = vectorStore[i].dateToString();
				temp.time = vectorStore[i].timeToString();
				temp.lineNumber = to_string(i + 1) + ".";
				
				_otherResult->push_back(temp);
			}
		}else if (!hasEndDate) {
			for (unsigned int i = 0; i < vectorStore.size(); i++) {
				if (isSameStartDateAndTime(vectorStore[i], _input)) {
					RESULT temp;
					temp.event = vectorStore[i].event;
					temp.date = vectorStore[i].dateToString();
					temp.time = vectorStore[i].timeToString();
					temp.lineNumber = to_string(i + 1) + ".";
				
					_otherResult->push_back(temp);
				}
			}
		} else if (hasStartDate) {
			for (unsigned int i = 0; i < vectorStore.size(); i++) {
				if (isWithinRange(vectorStore[i], _input)) {
					RESULT temp;
					temp.event = vectorStore[i].event;
					temp.date = vectorStore[i].dateToString();
					temp.time = vectorStore[i].timeToString();
					temp.lineNumber = to_string(i + 1) + ".";
				
					_otherResult->push_back(temp);
				}
			}
		}
	}

	void filterForFloating(vector<Item> &vectorStore) {
		_otherResult->clear();

		for (unsigned int i = 0; i < vectorStore.size(); i++) {
			if (vectorStore[i].isFloating()) {
				RESULT temp;
				temp.event = vectorStore[i].event;
				temp.date = vectorStore[i].dateToString();
				temp.time = vectorStore[i].timeToString();
				temp.lineNumber = to_string(i + 1) + ".";
				
				_otherResult->push_back(temp);
			}
		}
	}

	void executeAction(vector<Item> &vectorStore) {
		char buffer[1000];
		
		if(_input.event != "") {
			if (_input.isFloating()) {
				filterDateAndTime(vectorStore, false, false);
			} else if (_input.isDeadline()) {
				filterDateAndTime(vectorStore, true, false);
			} else {
				filterDateAndTime(vectorStore, true, true);
			}
			powerSearch(_otherResult);
		} else {
			if (_input.isFloating()) {
				filterForFloating(vectorStore);
			} else if (_input.isDeadline()) {
				filterDateAndTime(vectorStore, true, false);
			} else {
				filterDateAndTime(vectorStore, true, true);
			}
		}

		if (vectorStore.size() == 0) {
			sprintf_s(buffer, ERROR_SEARCH_ITEM_NOT_FOUND.c_str(), _message.c_str());
		}
		else {
			sprintf_s(buffer, SUCCESS_SEARCH.c_str(), _message.c_str());
		}
		_message = buffer;
		
	}

	string getMessage() {
		return _message;
	}

	void negateAction(vector<Item> &vectorStore) {
	}
};
