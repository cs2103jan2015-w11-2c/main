#pragma once

//@author A0116179B
#include <iostream>
#include <vector>
#include <algorithm>
#include "Item.h"
#include "Command.h"

const int initialMinEditDist = -1;
const int powerSearchLowThreshold = 1;
const int DAY_NUM_MINS = 1440;

const string TIME_UNIT_HOURS = " hours";
const string TIME_UNIT_MINUTES = " minutes";
const string STRING_FREE_SLOTS = "Free Slots";

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
	int _sleepTime[2][2];
	bool _searchFree;

public:
	SearchItem() {
		_message = "";
		_sleepTime[0][0] = 0;
		_sleepTime[0][1] = 0;
		_sleepTime[1][0] = 0;
		_sleepTime[1][1] = 0;
		_searchFree = false;
	}

	SearchItem(const Item input, const string message, vector<RESULT> *otherResult, int sleepTime[][2], bool free) {
		_input = input;
		_message = message;
		_otherResult = otherResult;
		_sleepTime[0][0] = sleepTime[0][0];
		_sleepTime[0][1] = sleepTime[0][1];
		_sleepTime[1][0] = sleepTime[1][0];
		_sleepTime[1][1] = sleepTime[1][1];
		_searchFree = free;
	}

	~SearchItem() {
	}

	//Levenshtein's Algorithm to get edit distance between two words
	int getEditDist(const string input, const string item) {
		int itemSize = item.size();
		int inputSize = input.size();
		int upLeftCell;

		if(inputSize == 0)
			return itemSize;
		if(itemSize == 0)
			return inputSize;

		int *difference = new int [inputSize+1];

		for(int i = 0; i <= inputSize; i++) {
			difference[i] = i;
		}

		for(int i = 1; i <= itemSize; i++) {
			difference[0] = i;
			upLeftCell = i - 1;

			for(int j = 1; j <= inputSize; j++) {
				if(input[j-1] == item[i-1]) {
					int temp = upLeftCell;
					upLeftCell = difference[j];
					difference[j] = temp;
				}
				else {
					int minimum(upLeftCell < difference[j-1] ? upLeftCell : difference[j-1]);
				
					int temp = difference[j];
					if(minimum < difference[j])
						difference[j] = minimum;
					upLeftCell = temp;
					difference[j]++;
				}
			}
		}
		return difference[inputSize];
	}

	//Search to carry out if search query is an acronym, returns true if full acronym is found
	bool acroSearch(const string currentString, const string acronym) {
		string currentWord;
		istringstream issWord(currentString);
		size_t pos;
		bool isFound = false;

		pos = 0;
		while (issWord >> currentWord) {
			if (!isFound) {
				if (currentWord[0] == acronym[pos]) {
					isFound = true;
				}
			}
			if (isFound && currentWord[0] == acronym[pos]) {
				pos++;
			} else {
					isFound = false;
					pos = 0;
			}
			if (pos == acronym.size()) {
				return true;
			}
		}
		return false;
	}

	//Search returns true if query is a subset of the an event
	bool normalSearch(const string currentString, const string fullInput) {
		size_t position = currentString.find(fullInput);
		if (position != string::npos) {
			return true;
		}
		return false;
	}

	//gets the minimum edit distance (Levenshtein's Algorithm) between all the words in the query and the event
	//minimum edit distance is returned via pass by reference
	void powerSearch(const string currentString, const string inputWord, int &minEditDist) {
		string currentWord;
		istringstream issWord(currentString);

		while (issWord >> currentWord) {
			int editDist = getEditDist(inputWord, currentWord);
			if(minEditDist < 0 || minEditDist > editDist) {
				minEditDist = editDist;
			}
		}
	}

	//Method that executes all search algorithms, results returned via pass by pointer
	void search(vector<RESULT> *resultVector) {
		vector<SEARCHRESULT> powerSearchLowRes;
		vector<SEARCHRESULT> normalSearchRes;
		vector<SEARCHRESULT> powerSearchAcroRes;

		string fullInput = _input.event;
		transform(fullInput.begin(), fullInput.end(), fullInput.begin(), ::tolower);
		
		for (unsigned int i = 0; i < resultVector->size(); i++) {
			string acronym = "";
			string currentString = resultVector->at(i).event;

			transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
			
			bool isFound = false;
			bool isAcronym = false;
			bool fullAcronymFound = false;
			int minEditDist = initialMinEditDist;

			string inputWord;
			istringstream issInput(fullInput);
			while (issInput >> inputWord) {
				if (inputWord.size() == 1) {
					isAcronym = true;
				} else {
					isAcronym = false;
				}

				if (isAcronym) {
					acronym += inputWord;
				} else {
					//ACRONYM SEARCH
					if (acronym.size() != 0) {
						fullAcronymFound = acroSearch(currentString, acronym);
					}
					//POWER SEARCH
					powerSearch(currentString, inputWord, minEditDist);

					//NORMAL SEARCH
					if (minEditDist > powerSearchLowThreshold && !isFound) {
						isFound = normalSearch(currentString, fullInput);
					}
					
				}
			}
			if (isAcronym) {
				//ACRONYM SEARCH
				if (acronym.size() != 0) {
					fullAcronymFound = acroSearch(currentString, acronym);
				} else {
					powerSearch(currentString, inputWord, minEditDist);
				}

			}

			SEARCHRESULT newSearchResult;
			newSearchResult.result = resultVector->at(i);
			newSearchResult.editDistance = minEditDist;

			if (minEditDist != -1 && minEditDist <= powerSearchLowThreshold) {
				unsigned int j = 0;
				while (j < powerSearchLowRes.size() && powerSearchLowRes[j].editDistance <= minEditDist) {
					j++;
				}
				powerSearchLowRes.insert(powerSearchLowRes.begin() + j, newSearchResult);
			} else if (isFound) {
				unsigned int j = 0;
				while (j < normalSearchRes.size() && normalSearchRes[j].editDistance <= minEditDist) {
					j++;
				}
				normalSearchRes.insert(normalSearchRes.begin() + j, newSearchResult);
			} else if (fullAcronymFound) {
				powerSearchAcroRes.push_back(newSearchResult);
			}
		}

		vector<SEARCHRESULT> temp;
		temp.insert(temp.end(), powerSearchLowRes.begin(), powerSearchLowRes.end());
		temp.insert(temp.end(), normalSearchRes.begin(), normalSearchRes.end());
		temp.insert(temp.end(), powerSearchAcroRes.begin(), powerSearchAcroRes.end());

		resultVector->clear();

		for (unsigned int i = 0; i < temp.size(); i++) {
			resultVector->push_back(temp[i].result);
		}
	}

	bool isSameStartDateAndTime(const Item item, const Item input) {
		DateTimeParser dateTimeParser;
		
		int day = input.eventDate[0];
		int mon = input.eventDate[1];
		int year = input.eventDate[2];

		dateTimeParser.getNextDayDate(day, mon, year);
		int nextDayDate[3] = {day, mon, year};

		dateTimeParser.getNextDayDate(day, mon, year);
		int afterNextDayDate[3] = {day, mon, year};

		//If deadline event, allow comparisons up to 2 days ahead, else only current day
		if (item.eventEndTime[0] == 0 && item.eventEndTime[1] == 0) {
			for (int i = 0; i < 3; i++) {
				if(item.eventDate[i] != input.eventDate[i] &&
					item.eventDate[i] != nextDayDate[i] &&
					item.eventDate[i] != afterNextDayDate[i]) {
					return false;
				}
			}
		} else {
			for (int i = 0; i < 3; i++) {
				if(item.eventDate[i] != input.eventDate[i]) {
					return false;
				}
			}
		}

		if (item.eventEndTime[0] == 0 && item.eventEndTime[1] == 0) {
			for (int i = 0; i < 2; i++) {
				if((input.eventStartTime[i] != 0 && item.eventStartTime[i] != input.eventStartTime[i]) &&
					item.eventDate[i] != nextDayDate[i] &&
					item.eventDate[i] != afterNextDayDate[i]) {
					return false;
				}
			}
		} else {
			for (int i = 0; i < 2; i++) {
				if(input.eventStartTime[i] != 0 && item.eventStartTime[i] != input.eventStartTime[i]) {
					return false;
				}
			}
		}
		return true;
	}

	//Returns -1 if first argument is an earlier date, 1 if second argument is earlier, 0 if identical
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

	//Returns -1 if first argument is an earlier time, 1 if second argument is earlier, 0 if identical
	int compareTimeEarlierThan(const int time1[2], const int time2[2]) {
		if (time1[1] < time2[1]) {
			return -1;
		} else if (time1[1] == time2[1]) {
			if (time1[0] < time2[0]) {
				return -1;
			} else if (time1[0] == time2[0]) {
					return 0;
			}
		}
		return 1;
	}

	//Returns true if any time field is non_zero to check if time has been set
	bool timeIsSpecified(const int timeArray[2]) {
		for (int i = 0; i < 2; i++) {
			if (timeArray[i] != 0) {
				return true;
			}
		}
		return false;
	}

	//Returns true if an item occurs within range specified by input
	bool isWithinRange(const Item item, const Item input) {
		
		//check if item starts earlier than input start date
		if(compareDateEarlierThan(item.eventDate, input.eventDate) == -1) {
			return false;
		}

		//check if item ends later than input end date
		if (input.eventEndDate[0] == 0 && input.eventEndDate[1] == 0 && input.eventEndDate[2] == 0) {
			if(compareDateEarlierThan(item.eventEndDate, input.eventDate) == 1) {
				return false;
			}
		} else {
			if(compareDateEarlierThan(item.eventEndDate, input.eventEndDate) == 1) {
				return false;
			}
		}

		//If the item is a deadline item, then the start date is compared to the input end date
		if (!timeIsSpecified(item.eventEndTime)) {
			if (input.eventEndDate[0] == 0 && input.eventEndDate[1] == 0 && input.eventEndDate[2] == 0) {
				if (compareDateEarlierThan(item.eventDate, input.eventDate) == 1) {
					return false;
				}
			} else {
				if (compareDateEarlierThan(item.eventDate, input.eventEndDate) == 1) {
					return false;
				}
			}
		}

		//check if item starts earlier than input start time and is specified
		if (timeIsSpecified(input.eventStartTime) && 
			(!timeIsSpecified(item.eventStartTime) 
			|| compareTimeEarlierThan(item.eventStartTime, input.eventStartTime) == -1)) {
				return false;
		}
		
		//check if item ends later than input end time and is specified
		//Since parser automatically sets the end time to be 1 hour after start, we use the start time
		//as an indicator for whether time has been specified
		if (timeIsSpecified(input.eventStartTime)) { //there is an end time
			if (timeIsSpecified(item.eventEndTime)) {
				if (compareTimeEarlierThan(item.eventEndTime, input.eventEndTime) == 1) {
					return false;
				}
			} else {
				if (compareTimeEarlierThan(item.eventStartTime, input.eventEndTime) == 1) {
					return false;
				}
			}
		}
		return true;
	}

	//Excludes items from search result depending on the specified time and date
	//Both start and end specified: excludes events not within range
	//Only start specified, searches for items with the same start time
	//Nothing specified, nothing excluded
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
		} else if (!hasEndDate) {
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

	//Only includes floating items for the case where search query is "float" or "floating"
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

	void initializeTimeFrame(bool timeFrames[DAY_NUM_MINS]) {
		for (int i = 0; i < 1440; i++) {
			timeFrames[i] = false;
		}
	}

	void blockTimeFrame(bool timeFrame[DAY_NUM_MINS], 
		const int startHr, 
		const int startMin, 
		const int endHr, 
		const int endMin) {

		int startTime = startHr * 60 + startMin;
		int endTime = endHr * 60 + endMin;
		
		for (int i = startTime; i < endTime; i++) {
			if (!timeFrame[i]) {
				timeFrame[i] = true;
			}
		}
	}

	void updateResultsForFree(const bool timeFrames[DAY_NUM_MINS]) {
		int blockLength = stoi(_input.event, NULL, 10);
		blockLength *= 60;

		bool isFound = false;
		int count = 0;
		int index = 0;
		int startMin = 0;

		_otherResult->clear();
		
		for (int i = 0; i < 1440; i++) {
			if (!timeFrames[i]) {
				if(!isFound && i < 1439) {
					isFound = true;
					startMin = i;
				}
				count++;
			} else {
				if (isFound) {
					isFound = false;
					if (count >= blockLength) {
						RESULT temp;
						Item tempItem;
					
						tempItem.eventStartTime[0] = startMin / 60;
						if (tempItem.eventStartTime[0] == 0) {
							tempItem.eventStartTime[0] = 24;
						}
						tempItem.eventStartTime[1] = startMin % 60;
					
						tempItem.eventEndTime[0] = i / 60;
						if (tempItem.eventEndTime[0] == 0) {
							tempItem.eventEndTime[0] = 24;
						}
						tempItem.eventEndTime[1] = i % 60;
						tempItem.eventEndDate[0] = _input.eventDate[0];
						tempItem.eventEndDate[1] = _input.eventDate[1];
						tempItem.eventEndDate[2] = _input.eventDate[2];

						int duration = i - startMin;
						temp.event = to_string(duration/60) + TIME_UNIT_HOURS;
						if (duration % 60 != 0) {
							temp.event += ", " + to_string(duration%60) + TIME_UNIT_MINUTES;
						}
						temp.date = STRING_FREE_SLOTS;
						temp.time = tempItem.timeAndEndDateToString();
						temp.lineNumber = to_string(index + 1) + ".";
					
						index++;

						_otherResult->push_back(temp);
					}
				}
				count = 0;
			}
		}
	}

	void filterForFree(vector<Item> vectorStore) {
		//Boolean array to represent every minute of a 24hour day
		bool timeFrames[DAY_NUM_MINS];
		//Sets entire time frame to free
		initializeTimeFrame(timeFrames);

		vector<Item> tempVector;

		for (unsigned int i = 0; i < vectorStore.size(); i++) {
			if (!vectorStore[i].isDeadline() && !vectorStore[i].isFloating()) {
				if (compareDateEarlierThan(vectorStore[i].eventEndDate, _input.eventDate) == 0 ||
					compareDateEarlierThan(vectorStore[i].eventDate, _input.eventDate) == 0) {
						tempVector.push_back(vectorStore[i]);
				} 
			}
		}

		blockTimeFrame(timeFrames, _sleepTime[0][0] % 24, _sleepTime[0][1], 24, 0);
		blockTimeFrame(timeFrames, 0, 0, _sleepTime[1][0] % 24, _sleepTime[1][1]);


		for (unsigned int i = 0; i < tempVector.size(); i++) {
			int startHour = 0;
			int startMinute = 0;
			int endHour = 0;
			int endMinute = 0;
		
			if (compareDateEarlierThan(tempVector[i].eventDate, _input.eventDate) == 0) {
				startHour = tempVector[i].eventStartTime[0] % 24;
				startMinute = tempVector[i].eventStartTime[1] % 60;
			} else {
				startHour = _sleepTime[1][0];
				startMinute = _sleepTime[1][1];
			}
			
			if ((tempVector[i].eventEndDate[0] == 0 && tempVector[i].eventEndDate[1] == 0) ||
				(compareDateEarlierThan(tempVector[i].eventEndDate, _input.eventDate) == 0)) {
				endHour = tempVector[i].eventEndTime[0] % 24;
				endMinute = tempVector[i].eventEndTime[1] % 60;
			} else {
				endHour = _sleepTime[0][0];
				endMinute = _sleepTime[0][1];
			}
			
			blockTimeFrame(timeFrames, startHour, startMinute, endHour, endMinute);

		}

		updateResultsForFree(timeFrames);
		
	}

	void executeAction(vector<Item> &vectorStore) {
		char buffer[1000];
		
		if (!_searchFree) {
			if(_input.event != "") {
				if (_input.isFloating()) {
					filterDateAndTime(vectorStore, false, false);
				} else if (_input.isDeadline()) {
					filterDateAndTime(vectorStore, true, false);
				} else {
					filterDateAndTime(vectorStore, true, true);
				}
				search(_otherResult);
			} else {
				if (_input.isFloating()) {
					filterForFloating(vectorStore);
				} else if (_input.isDeadline()) {
					filterDateAndTime(vectorStore, true, false);
				} else {
					filterDateAndTime(vectorStore, true, true);
				}
			}
		} else {
			filterForFree(vectorStore);
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
