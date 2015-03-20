#include "MessageManager.h"


MessageManager::MessageManager(void) {
	magicMemo = new Controller();
	_resultVector = new vector<RESULT>;

	_numberHighlight = new vector<HIGHLIGHT>;
	_dateHighlight = new vector<HIGHLIGHT>;
	_timeHighlight = new vector<HIGHLIGHT>;
	_eventHighlight = new vector<HIGHLIGHT>;
	_completedHighlight = new vector<HIGHLIGHT>;

	_userInput = "";
	_successMessage = "";
	_todayTaskBoxMessage = "";
	_allTaskBoxMessage = "";
	_inputBoxMessage = "";

	indexCount = 0;
}

Void MessageManager::generateMessageOutputs(String^ textFromUser) {
	*_resultVector = magicMemo->executeCommand(convertToStdString(textFromUser));
	_successMessage = convertToSystemString(magicMemo->getSuccessMessage());
	//_todayTaskBoxMessage = convertToSystemString(magicMemo->
	//_allTaskBoxMessage = convertToSystemString(magicMemo->displayAll());

	calculateIndexes();
	_allTaskBoxMessage = toString();
}

Void MessageManager::calculateIndexes() {
	string prevDate = "";

	for(unsigned int i = 0; i < _resultVector->size(); i++) {
		HIGHLIGHT temp;
		indexCount += i;

		if(_resultVector->at(i).date != prevDate) {
			temp.index = indexCount;
			temp.length = sizeof(_resultVector->at(i).date);
			_dateHighlight->push_back(temp);
			indexCount += temp.length;
			prevDate = _resultVector->at(i).date;
		}

		temp.index = indexCount;
		temp.length = sizeof(_resultVector->at(i).lineNumber);
		_numberHighlight->push_back(temp);

		temp.index = temp.index + temp.length;
		temp.length = sizeof(_resultVector->at(i).time);
		_timeHighlight->push_back(temp);

		temp.index = temp.index + temp.length;
		temp.length = sizeof(_resultVector->at(i).event);
		_eventHighlight->push_back(temp);
		
		indexCount = temp.index + temp.length;;
	}
}

Void MessageManager::calculateDateIndex() {
}

Void MessageManager::calculateEventIndex() {
}

String^ MessageManager::toString() {
	ostringstream oss;

	for(unsigned int i = 0; i < _resultVector->size(); i++) {
		oss << _resultVector->at(i).lineNumber << ". ";
		oss << _resultVector->at(i).time << " ";
		oss << _resultVector->at(i).event << endl;
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

vector<HIGHLIGHT>* MessageManager::getNumberHighlight() {
	return _numberHighlight;
}

vector<HIGHLIGHT>* MessageManager::getTimeHighlight() {
	return _timeHighlight;
}

vector<HIGHLIGHT>* MessageManager::getDateHighlight() {
	return _dateHighlight;
}

vector<HIGHLIGHT>* MessageManager::getEventHighlight() {
	return _eventHighlight;
}

String^ MessageManager::convertToSystemString(string inputString) {
	return gcnew String(inputString.c_str());
}

string MessageManager::convertToStdString(String^ inputString) {
	return msclr::interop::marshal_as< std::string >(inputString);
}
