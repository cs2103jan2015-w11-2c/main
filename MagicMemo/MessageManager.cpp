#include "MessageManager.h"


MessageManager::MessageManager(void) {
	magicMemo = new Controller();
	_allTaskVector = new vector<RESULT>;
	_todayTaskVector = new vector<RESULT>;

	_allNumberHighlight = new vector<HIGHLIGHT>;
	_allDateHighlight = new vector<HIGHLIGHT>;
	_allTimeHighlight = new vector<HIGHLIGHT>;
	_allEventHighlight = new vector<HIGHLIGHT>;
	_allCompletedHighlight = new vector<HIGHLIGHT>;

	_todayNumberHighlight = new vector<HIGHLIGHT>;
	_todayDateHighlight = new vector<HIGHLIGHT>;
	_todayTimeHighlight = new vector<HIGHLIGHT>;
	_todayEventHighlight = new vector<HIGHLIGHT>;
	_todayCompletedHighlight = new vector<HIGHLIGHT>;

	_userInput = "";
	_successMessage = "";
	_todayTaskBoxMessage = "";
	_allTaskBoxMessage = "";
	_inputBoxMessage = "";
}

Void MessageManager::generateMessageOutputs(String^ textFromUser) {
	magicMemo->executeCommand(convertToStdString(textFromUser));
	*_allTaskVector = magicMemo->getOtherResult();
	*_todayTaskVector = magicMemo->getTodayResult();
	_successMessage = convertToSystemString(magicMemo->getSuccessMessage());

	calculateAllTaskIndexes();
	_allTaskBoxMessage = toString(_allTaskVector);
	
	calculateTodayTaskIndexes();
	_todayTaskBoxMessage = toString(_todayTaskVector);

}

Void MessageManager::calculateAllTaskIndexes() {
	clearAllTaskIndexVectors();
	string prevDate = "";
	int indexCount = 0;
	for(unsigned int i = 0; i < _allTaskVector->size(); i++) {
		HIGHLIGHT temp;

		if(_allTaskVector->at(i).date != prevDate) {
			temp.index = indexCount;
			temp.length = _allTaskVector->at(i).date.length();
			_allDateHighlight->push_back(temp);
			indexCount = indexCount + temp.length + 1;
			prevDate = _allTaskVector->at(i).date;
		}

		temp.index = indexCount;
		temp.length = _allTaskVector->at(i).lineNumber.length();
		_allNumberHighlight->push_back(temp);

		temp.index = temp.index + temp.length + 1;
		temp.length = _allTaskVector->at(i).time.length();
		_allTimeHighlight->push_back(temp);

		if(temp.index > 0) {
			temp.index += 1;
		}

		temp.index = temp.index + temp.length;
		temp.length = _allTaskVector->at(i).event.length();
		_allEventHighlight->push_back(temp);
		
		indexCount = temp.index + temp.length + 1;
	}
}

Void MessageManager::calculateTodayTaskIndexes() {
	clearTodayTaskIndexVectors();
	string prevDate = "";
	int indexCount = 0;
	for(unsigned int i = 0; i < _todayTaskVector->size(); i++) {
		HIGHLIGHT temp;

		if(_todayTaskVector->at(i).date != prevDate) {
			temp.index = indexCount;
			temp.length = _todayTaskVector->at(i).date.length();
			_todayDateHighlight->push_back(temp);
			indexCount = indexCount + temp.length + 1;
			prevDate = _todayTaskVector->at(i).date;
		}

		temp.index = indexCount;
		temp.length = _todayTaskVector->at(i).lineNumber.length();
		_todayNumberHighlight->push_back(temp);

		temp.index = temp.index + temp.length + 1;
		temp.length = _todayTaskVector->at(i).time.length();
		_todayTimeHighlight->push_back(temp);

		if(temp.index > 0) {
			temp.index += 1;
		}

		temp.index = temp.index + temp.length;
		temp.length = _todayTaskVector->at(i).event.length();
		_todayEventHighlight->push_back(temp);
		
		indexCount = temp.index + temp.length + 1;
	}
}

String^ MessageManager::toString(vector<RESULT>* taskVector) {
	ostringstream oss;
	string prevDate = "";
	for(unsigned int i = 0; i < taskVector->size(); i++) {
		if(taskVector->at(i).date != prevDate) {
			oss << taskVector->at(i).date << endl;
			prevDate = taskVector->at(i).date;
		}
		oss << taskVector->at(i).lineNumber << " ";
		oss << taskVector->at(i).time << " ";
		oss << taskVector->at(i).event << endl;
	}

	return convertToSystemString(oss.str());
}

String^ MessageManager::getSuccessMessage() {
	return _successMessage;
}

String^ MessageManager::getTodayTaskBoxMessage() {
	return _todayTaskBoxMessage;
}

String^ MessageManager::getAllTaskBoxMessage() {
	return _allTaskBoxMessage;
}

String^ MessageManager::getInputBoxMessage() {
	return _inputBoxMessage;
}

vector<HIGHLIGHT>* MessageManager::getAllNumberHighlight() {
	return _allNumberHighlight;
}

vector<HIGHLIGHT>* MessageManager::getAllTimeHighlight() {
	return _allTimeHighlight;
}

vector<HIGHLIGHT>* MessageManager::getAllDateHighlight() {
	return _allDateHighlight;
}

vector<HIGHLIGHT>* MessageManager::getAllEventHighlight() {
	return _allEventHighlight;
}

vector<HIGHLIGHT>* MessageManager::getTodayNumberHighlight() {
	return _todayNumberHighlight;
}

vector<HIGHLIGHT>* MessageManager::getTodayTimeHighlight() {
	return _todayTimeHighlight;
}

vector<HIGHLIGHT>* MessageManager::getTodayDateHighlight() {
	return _todayDateHighlight;
}

vector<HIGHLIGHT>* MessageManager::getTodayEventHighlight() {
	return _todayEventHighlight;
}

Void MessageManager::clearAllTaskIndexVectors() {
	_allNumberHighlight->clear();
	_allDateHighlight->clear();
	_allTimeHighlight->clear();
	_allEventHighlight->clear();
	_allCompletedHighlight->clear();
}

Void MessageManager::clearTodayTaskIndexVectors() {
	_todayNumberHighlight->clear();
	_todayDateHighlight->clear();
	_todayTimeHighlight->clear();
	_todayEventHighlight->clear();
	_todayCompletedHighlight->clear();
}


String^ MessageManager::convertToSystemString(string inputString) {
	return gcnew String(inputString.c_str());
}

string MessageManager::convertToStdString(String^ inputString) {
	return msclr::interop::marshal_as< std::string >(inputString);
}
