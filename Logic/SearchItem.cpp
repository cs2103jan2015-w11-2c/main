#pragma once

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

public:
	SearchItem() {
		_message = "";
	}
	SearchItem(const Item input, vector<RESULT> *otherResult) {
		_input = input;
		_message = "";
		_otherResult = otherResult;
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

	bool isSameDateAndTime(const Item item, const Item input) {
		for (int i = 0; i < 3; i++) {
			if(input.eventDate[i] != 0 && item.eventDate[i] != input.eventDate[i]) {
				return false;
			}
		}
		for (int i = 0; i < 3; i++) {
			if(input.eventEndDate[i] != 0 && item.eventEndDate[i] != input.eventEndDate[i]) {
				return false;
			}
		}
		for (int i = 0; i < 2; i++) {
			if(input.eventStartTime[i] != 0 && item.eventStartTime[i] != input.eventStartTime[i]) {
				return false;
			}
		}
		for (int i = 0; i < 2; i++) {
			if(input.eventEndTime[i] != 0 && item.eventEndTime[i] != input.eventEndTime[i]) {
				return false;
			}
		}
		return true;
	}

	void filterDateAndTime(vector<Item> &vectorStore) {
		_otherResult->clear();

		for (unsigned int i = 0; i < vectorStore.size(); i++) {
			if (isSameDateAndTime(vectorStore[i], _input)) {
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
		filterDateAndTime(vectorStore);
		if(_input.event != "") {
			powerSearch(_otherResult);
		}

		char buffer[1000];
		if (vectorStore.size() == 0) {
			sprintf_s(buffer, ERROR_SEARCH_ITEM_NOT_FOUND.c_str(), _input.toString().c_str());
		}
		else {
			sprintf_s(buffer, SUCCESS_SEARCH.c_str(), _input.toString().c_str());
		}
		_message = buffer;
	}

	string getMessage() {
		return _message;
	}

	void negateAction(vector<Item> &vectorStore) {
	}
};
