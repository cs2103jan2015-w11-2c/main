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
const string ERROR_INVALID_SLOT_LENGTH = "Invalid slot length!";

static const string DEADLINE_HEADING = "Deadline Events";

using namespace std;

struct SEARCHRESULT {
	int editDistance;
	RESULT result;
};

class SearchItem : public Command {
private:
	Item _input;
	string _message;
	vector<RESULT> *_otherResult;
	int _sleepTime[2][2];
	bool _searchFree;
	bool _is12HourFormat;
public:
	SearchItem() {
		_message = "";
		_sleepTime[0][0] = 0;
		_sleepTime[0][1] = 0;
		_sleepTime[1][0] = 0;
		_sleepTime[1][1] = 0;
		_searchFree = false;
	}

	SearchItem(const Item input, const string message, vector<RESULT> *otherResult, int sleepTime[][2], bool free, bool is12HourFormat) {
		_input = input;
		_message = message;
		_otherResult = otherResult;
		_sleepTime[0][0] = sleepTime[0][0];
		_sleepTime[0][1] = sleepTime[0][1];
		_sleepTime[1][0] = sleepTime[1][0];
		_sleepTime[1][1] = sleepTime[1][1];
		_searchFree = free;
		_is12HourFormat = is12HourFormat;
	}

	~SearchItem() {
	}

	long getTimePos(const int date[3], const int time[2]) {
		long timePos = 0;

		timePos += date[2];
		timePos *= 12;
		timePos += date[1];
		timePos *= 31;
		timePos += date[0];
		timePos *= 24;
		timePos += time[0];
		timePos *= 60;
		timePos += time[1];

		return timePos;
	}

	bool checkIsClash(Item item1, Item item2) {
		long startTimePos1 = getTimePos(item1.eventDate, item1.eventStartTime);
		long endTimePos1;
		if (!dateIsSpecified(item1.eventEndDate)) {
			if (item1.eventEndDate[0] == 0 && item1.eventEndDate[1] == 0) {
				endTimePos1 =  getTimePos(item1.eventDate, item1.eventStartTime);
			} else {
				endTimePos1 =  getTimePos(item1.eventDate, item1.eventEndTime);
			}
		} else {
			if (item1.eventEndDate[0] == 0 && item1.eventEndDate[1] == 0) {
				endTimePos1 =  getTimePos(item1.eventEndDate, item1.eventStartTime);
			} else {
				endTimePos1 =  getTimePos(item1.eventEndDate, item1.eventEndTime);
			}
		}

		long startTimePos2 = getTimePos(item2.eventDate, item2.eventStartTime);
		long endTimePos2;
		if (!dateIsSpecified(item2.eventEndDate)) {
			if (item2.eventEndDate[0] == 0 && item2.eventEndDate[1] == 0) {
				endTimePos2 =  getTimePos(item2.eventDate, item2.eventStartTime);
			} else {
				endTimePos2 =  getTimePos(item2.eventDate, item2.eventEndTime);
			}
		} else {
			if (item2.eventEndDate[0] == 0 && item2.eventEndDate[1] == 0) {
				endTimePos2 =  getTimePos(item2.eventEndDate, item2.eventStartTime);
			} else {
				endTimePos2 =  getTimePos(item2.eventEndDate, item2.eventEndTime);
			}
		}

		bool isDeadline1 = item1.isDeadline();
		bool isDeadline2 = item2.isDeadline();

		if (isDeadline1 && isDeadline2) {
			if (startTimePos1 != startTimePos2) {
				return false;
			}
			return true;
		} else if (isDeadline1) {
			if (startTimePos1 <= startTimePos2 || startTimePos1 >= endTimePos2) {
				return false;
			} 
			return true;
		} else if (isDeadline2) {
			if (startTimePos2 <= startTimePos1 || startTimePos2 >= endTimePos1) {
				return false;
			} 
			return true;
		}

		if (endTimePos2 <= startTimePos1) {
			return false;
		}
		if (endTimePos1 <= startTimePos2) {
			return false;
		}
		return true;
	}

	bool checkIsExpired(Item item) {
		DateTime dateTime;

		if (item.isDeadline()) {
			if (item.eventDate[2] < dateTime.getCurrentYear()) {
				return true;
			} else if (item.eventDate[2] == dateTime.getCurrentYear()) {
				if (item.eventDate[1] < dateTime.getCurrentMonth()) {
					return true;
				} else if (item.eventDate[1] == dateTime.getCurrentMonth()) {
					if (item.eventDate[0] < dateTime.getCurrentDay()) {
						return true;
					} else if (item.eventDate[0] == dateTime.getCurrentDay()) {
						if (item.eventStartTime[0] < dateTime.getCurrentHour()) {
							return true;
						} else if (item.eventStartTime[0] == dateTime.getCurrentHour()) {
							if (item.eventStartTime[1] < dateTime.getCurrentMinute()) {
								return true;
							} else {
								return false;
							}
						}
					}
				}
			}
		} else if (!dateIsSpecified(item.eventEndDate)){
			if (item.eventDate[2] < dateTime.getCurrentYear()) {
				return true;
			} else if (item.eventDate[2] == dateTime.getCurrentYear()) {
				if (item.eventDate[1] < dateTime.getCurrentMonth()) {
					return true;
				} else if (item.eventDate[1] == dateTime.getCurrentMonth()) {
					if (item.eventDate[0] < dateTime.getCurrentDay()) {
						return true;
					} else if (item.eventDate[0] == dateTime.getCurrentDay()) {
						if (item.eventEndTime[0] < dateTime.getCurrentHour()) {
							return true;
						} else if (item.eventEndTime[0] == dateTime.getCurrentHour()) {
							if (item.eventEndTime[1] < dateTime.getCurrentMinute()) {
								return true;
							} else {
								return false;
							}
						}
					}
				}
			}
		} else if (item.eventEndTime[0] == 0 && item.eventEndTime[1] == 0){
			if (item.eventEndDate[2] < dateTime.getCurrentYear()) {
				return true;
			} else if (item.eventEndDate[2] == dateTime.getCurrentYear()) {
				if (item.eventEndDate[1] < dateTime.getCurrentMonth()) {
					return true;
				} else if (item.eventEndDate[1] == dateTime.getCurrentMonth()) {
					if (item.eventEndDate[0] < dateTime.getCurrentDay()) {
						return true;
					} else {
						return false;
					}
				}
			}
		} else {
			if (item.eventEndDate[2] < dateTime.getCurrentYear()) {
				return true;
			} else if (item.eventEndDate[2] == dateTime.getCurrentYear()) {
				if (item.eventEndDate[1] < dateTime.getCurrentMonth()) {
					return true;
				} else if (item.eventEndDate[1] == dateTime.getCurrentMonth()) {
					if (item.eventEndDate[0] < dateTime.getCurrentDay()) {
						return true;
					} else if (item.eventEndDate[0] == dateTime.getCurrentDay()) {
						if (item.eventEndTime[0] < dateTime.getCurrentHour()) {
							return true;
						} else if (item.eventEndTime[0] == dateTime.getCurrentHour()) {
							if (item.eventEndTime[1] < dateTime.getCurrentMinute()) {
								return true;
							} else {
								return false;
							}
						}
					}
				}
			}
		}
		return false;
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

	bool isSameStartOrEndDateAndTime(const Item item, const Item input) {
		DateTimeParser dateTimeParser;
		
		bool isEndMatched = true;

		int day = input.eventDate[0];
		int mon = input.eventDate[1];
		int year = input.eventDate[2];

		dateTimeParser.getNextDayDate(day, mon, year);
		int nextDayDate[3] = {day, mon, year};

		dateTimeParser.getNextDayDate(day, mon, year);
		int afterNextDayDate[3] = {day, mon, year};

		//If deadline event, allow comparisons up to 2 days ahead, else only current day
		if (item.isDeadlineTask) {
			for (int i = 0; i < 3; i++) {
				if(item.eventDate[i] != input.eventDate[i] &&
					item.eventDate[i] != nextDayDate[i] &&
					item.eventDate[i] != afterNextDayDate[i]) {
					return false;
				}
			}
		} else {
			for (int i = 0; i < 3; i++) {
				if(item.eventDate[i] != input.eventDate[i] && item.eventEndDate[i] != input.eventDate[i]) {
					return false;
				}
			}
		}

		//check to see whether if the start or the end of the timed task is matched with the input
		for (int i = 0; i < 3; i++) {
			if (item.eventEndDate[i] != input.eventDate[i]) {
				isEndMatched = false;
				break;
			}
		}

		//if IsDeadline, check against next 2 days, else check if if time matches the previously matched date
		if (item.isDeadlineTask) {
			for (int i = 0; i < 2; i++) {
				if((input.eventStartTime[i] != 0 && item.eventStartTime[i] != input.eventStartTime[i]) &&
					item.eventDate[i] != nextDayDate[i] &&
					item.eventDate[i] != afterNextDayDate[i]) {
					return false;
				}
			}
		} else {
			for (int i = 0; i < 2; i++) {
				if(input.eventStartTime[i] != 0 
					&& ((!isEndMatched && item.eventStartTime[i] != input.eventStartTime[i])
					|| (isEndMatched && item.eventEndTime[i] != input.eventStartTime[i]))) {
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
	
	//Returns true if any date field is non_zero to check if date has been set
	bool dateIsSpecified(const int dateArray[3]) {
		for (int i = 0; i < 2; i++) {
			if (dateArray[i] != 0) {
				return true;
			}
		}
		return false;
	}
	
	//Returns true if any time field is non_zero to check if time has been set
	bool timeIsSpecified(const int timeArray[3]) {
		for (int i = 0; i < 2; i++) {
			if (timeArray[i] != 0) {
				return true;
			}
		}
		return false;
	}

	//Returns true if an item occurs within range specified by input
	bool isWithinRange(const Item item, const Item input) {

		DateTimeParser dateTimeParser;

		int day;
		int mon;
		int year;


		if (!dateIsSpecified(item.eventEndDate)) {
			day = input.eventDate[0];
			mon = input.eventDate[1];
			year = input.eventDate[2];
		} else {
			day = input.eventEndDate[0];
			mon = input.eventEndDate[1];
			year = input.eventEndDate[2];
		}

		dateTimeParser.getNextDayDate(day, mon, year);
		int nextDayDate[3] = {day, mon, year};

		dateTimeParser.getNextDayDate(day, mon, year);
		int afterNextDayDate[3] = {day, mon, year};

		//If item is a deadline task, then check against the next 2 days. If there is a match, return true 
		if (item.isDeadlineTask) {
			bool isMatch = true;
		
			for ( int i = 0; i < 3; i++) {
				if (item.eventDate[i] != nextDayDate[i] && item.eventDate[i] != afterNextDayDate[i]) {
					isMatch = false;
					break;
				}
			}
			if (isMatch) {
				return true;
			}
		}

		//check if item starts earlier than input start date
		if(compareDateEarlierThan(item.eventDate, input.eventDate) == -1) {
			return false;
		}

		//check if item ends later than input end date. If end date unspecified, then compare with input start date
		if (!dateIsSpecified(input.eventEndDate)) {
			if(compareDateEarlierThan(item.eventEndDate, input.eventDate) == 1) {
				return false;
			}
		} else {
			if(compareDateEarlierThan(item.eventEndDate, input.eventEndDate) == 1) {
				return false;
			}
		}

		//check if item starts earlier than input start time and is specified
		if (timeIsSpecified(input.eventStartTime) && 
			(!timeIsSpecified(item.eventStartTime) 
			|| compareTimeEarlierThan(item.eventStartTime, input.eventStartTime) == -1)) {
				return false;
		}
		
		//check if item ends later than input end time and is specified
		if (timeIsSpecified(input.eventEndTime)) { 
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
	void filterDateAndTime(vector<Item> vectorStore, bool hasStartDate, bool hasEndDate) {
		vector<RESULT> floatingRes;
		vector<RESULT> deadlineRes;
		vector<RESULT> otherRes;

		_otherResult->clear();

		if (!hasStartDate && !hasEndDate) {
			for (unsigned int i = 0; i < vectorStore.size(); i++) {
				RESULT temp;
				temp.event = vectorStore[i].event;
				temp.lineNumber = to_string(i + 1) + ".";
				temp.isClash = false;
				for (unsigned int j = 0; j < vectorStore.size(); j++) {
					if (i != j && checkIsClash(vectorStore[i], vectorStore[j])) {
						temp.isClash = true;
						break;
					}
				}
				temp.isExpired = checkIsExpired(vectorStore[i]);
				if (vectorStore[i].isDeadline()) {
					temp.date = DEADLINE_HEADING;
					vectorStore[i].eventEndDate[0] = vectorStore[i].eventDate[0];
					vectorStore[i].eventEndDate[1] = vectorStore[i].eventDate[1];
					vectorStore[i].eventEndDate[2] = vectorStore[i].eventDate[2];
					if (_is12HourFormat) {
						temp.time = vectorStore[i].timeAndEndDateToString();
					} else {
						temp.time = vectorStore[i].timeTo24HrString();
					}
					deadlineRes.push_back(temp);
				} else {
					temp.date = vectorStore[i].dateToString();
					if (_is12HourFormat) {
						temp.time = vectorStore[i].timeAndEndDateToString();
					} else {
						temp.time = vectorStore[i].timeTo24HrString();
					}
					if (vectorStore[i].isFloating()) {
						floatingRes.push_back(temp);
					} else {
						_otherResult->push_back(temp);
					}				
				}
			}
		} else if (!hasEndDate) {
			for (unsigned int i = 0; i < vectorStore.size(); i++) {
				if (isSameStartOrEndDateAndTime(vectorStore[i], _input)) {
					RESULT temp;
					temp.event = vectorStore[i].event;
					temp.lineNumber = to_string(i + 1) + ".";
					
					temp.isClash = false;
					for (unsigned int j = 0; j < vectorStore.size(); j++) {
						if (i != j && checkIsClash(vectorStore[i], vectorStore[j])) {
							temp.isClash = true;
							break;
						}
					}
					
					temp.isExpired = checkIsExpired(vectorStore[i]);
					
					if (vectorStore[i].isDeadline()) {
						temp.date = DEADLINE_HEADING;
						vectorStore[i].eventEndDate[0] = vectorStore[i].eventDate[0];
						vectorStore[i].eventEndDate[1] = vectorStore[i].eventDate[1];
						vectorStore[i].eventEndDate[2] = vectorStore[i].eventDate[2];
						if (_is12HourFormat) {
							temp.time = vectorStore[i].timeAndEndDateToString();
						} else {
							temp.time = vectorStore[i].timeTo24HrString();
						}
						deadlineRes.push_back(temp);
					} else {
						temp.date = vectorStore[i].dateToString();
						if (_is12HourFormat) {
							temp.time = vectorStore[i].timeAndEndDateToString();
						} else {
							temp.time = vectorStore[i].timeTo24HrString();
						}
						if (vectorStore[i].isFloating()) {
							floatingRes.push_back(temp);
						} else {
							_otherResult->push_back(temp);
						}				
					}
				}
			}	
		} else if (hasStartDate) {
			for (unsigned int i = 0; i < vectorStore.size(); i++) {
				if (isWithinRange(vectorStore[i], _input)) {
					RESULT temp;
					temp.event = vectorStore[i].event;
					temp.lineNumber = to_string(i + 1) + ".";
					temp.isClash = false;
					for (unsigned int j = 0; j < vectorStore.size(); j++) {
						if (i != j && checkIsClash(vectorStore[i], vectorStore[j])) {
							temp.isClash = true;
							break;
						}
					}
			
					temp.isExpired = checkIsExpired(vectorStore[i]);
					
					if (vectorStore[i].isDeadline()) {
						temp.date = DEADLINE_HEADING;
						vectorStore[i].eventEndDate[0] = vectorStore[i].eventDate[0];
						vectorStore[i].eventEndDate[1] = vectorStore[i].eventDate[1];
						vectorStore[i].eventEndDate[2] = vectorStore[i].eventDate[2];
						if (_is12HourFormat) {
							temp.time = vectorStore[i].timeAndEndDateToString();
						} else {
							temp.time = vectorStore[i].timeTo24HrString();
						}
						deadlineRes.push_back(temp);
					} else {
						temp.date = vectorStore[i].dateToString();
						if (_is12HourFormat) {
							temp.time = vectorStore[i].timeAndEndDateToString();
						} else {
							temp.time = vectorStore[i].timeTo24HrString();
						}
						if (vectorStore[i].isFloating()) {
							floatingRes.push_back(temp);
						} else {
							_otherResult->push_back(temp);
						}				
					}
				}
			}
		}
		_otherResult->insert(_otherResult->begin(), deadlineRes.begin(), deadlineRes.end());
		_otherResult->insert(_otherResult->begin(), floatingRes.begin(), floatingRes.end());
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
						
						tempItem.eventEndDate[0] = 0;
						tempItem.eventEndDate[1] = 0;
						tempItem.eventEndDate[2] = 0;

						int duration = i - startMin;
						temp.event = to_string(duration/60) + TIME_UNIT_HOURS;
						if (duration % 60 != 0) {
							temp.event += ", " + to_string(duration % 60) + TIME_UNIT_MINUTES;
						}
						temp.date = _input.dateToString();
						if (_is12HourFormat) {
							temp.time = tempItem.timeAndEndDateToString();
						} else {
							temp.time = tempItem.timeTo24HrString();
						}
						temp.lineNumber = to_string(index + 1) + ".";
						
						temp.isClash = false;
						temp.isDeadline = false;
						temp.isExpired = false;

						index++;

						_otherResult->push_back(temp);
					}
				}
				count = 0;
			}
		}
	}

	void updateItemToNextDay() {
		int day = _input.eventDate[0];
		int mon = _input.eventDate[1];
		int year = _input.eventDate[2];

		DateTimeParser dateTimeParser;

		dateTimeParser.getNextDayDate(day, mon, year);

		_input.eventDate[0] = day;
		_input.eventDate[1] = mon;
		_input.eventDate[2] = year;
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
				} else if (!dateIsSpecified(_input.eventEndDate)) {
					filterDateAndTime(vectorStore, true, false);
				} else {
					filterDateAndTime(vectorStore, true, true);
				}
				search(_otherResult);
			} else {
				if (_input.isFloating()) {
					filterForFloating(vectorStore);
				} else if (!dateIsSpecified(_input.eventEndDate)) {
					filterDateAndTime(vectorStore, true, false);
				} else {
					filterDateAndTime(vectorStore, true, true);
				}
			}
		} else {
			int blockLength = stoi(_input.event, NULL, 10);
			int maxLength = _sleepTime[0][1] - _sleepTime[1][1];
			maxLength += 60 * (_sleepTime[0][0] - _sleepTime[1][0]);
			maxLength /= 60;
			if (blockLength > maxLength) {
				throw std::out_of_range(ERROR_INVALID_SLOT_LENGTH);
			}

			do {
				filterForFree(vectorStore);
				updateItemToNextDay();
			} while (_otherResult->empty());
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
