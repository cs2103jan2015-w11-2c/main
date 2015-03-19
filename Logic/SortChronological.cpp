#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Item.h"
#include "Command.h"

using namespace std;

class SortChronological : public Command {
private:
	string _message;

public:
	SortChronological() {
		_message = "";
	}

	~SortChronological() {
	}

	
	void swap(Item& item1, Item& item2) {
		Item tempItem = item1;
		item1 = item2;
		item2 = tempItem;
	}

	int compareEarlierThan(const Item item1, const Item item2) {
		if (item1.eventDate[2] < item2.eventDate[2]) {
			return -1;
		} else if (item1.eventDate[2] == item2.eventDate[2]) {
			if (item1.eventDate[1] < item2.eventDate[1]) {
				return -1;
			} else if (item1.eventDate[1] == item2.eventDate[1]) {
				if (item1.eventDate[0] < item2.eventDate[0]) {
					return -1;
				} else if (item1.eventDate[0] == item2.eventDate[0]) {
					if (item1.eventStartTime[0] < item2.eventStartTime[0]) {
						return -1;
					} else if (item1.eventStartTime[0] == item2.eventStartTime[0]) {
						if (item1.eventStartTime[1] < item2.eventStartTime[1]) {
							return -1;
						} else if (item1.eventStartTime[1] == item2.eventStartTime[1]) {
							return 0;
						}
					}
				}
			}
		}
		return 1;
	}

	void selectionSort(vector<Item>& vectorStore) {
		for (unsigned int i = 0; i < (vectorStore.size() - 1); i++) {
			int minIndex = i;
			for (unsigned int j = i + 1; j < vectorStore.size(); j++) {
				if (compareEarlierThan(vectorStore[j], vectorStore[minIndex]) < 0) {
					minIndex=j;
				}
			}
			if(minIndex != i) {
				swap(vectorStore[minIndex],vectorStore[i]);
			}
		}
	}

	void executeAction(vector<Item>& vectorStore) {
		if (vectorStore.empty()) {
			_message = ERROR_FILE_EMPTY;
		}
		else {
			selectionSort(vectorStore);
			_message = SUCCESS_SORTED_CHRONO;
		}
	}

	string getMessage() {
		return _message;
	}
};
