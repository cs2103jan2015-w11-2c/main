#include "MessageManager.h"


MessageManager::MessageManager(void) {
	magicMemo = new Controller();
	_resultVector = new vector<RESULT>;

	_numberHighlight = new vector<HIGHLIGHT>;
	_dateHighlight = new vector<HIGHLIGHT>;
	_eventHighlight = new vector<HIGHLIGHT>;
	_completedHighlight = new vector<HIGHLIGHT>;
	
	_userInput = "";
	_successMessage = "";
	_todayTaskBoxMessage = "";
	_allTaskBoxMessage = "";
	_inputBoxMessage = "";
}

Void MessageManager::generateMessageOutputs(String^ textFromUser) {
	*_resultVector = magicMemo->executeCommand(convertToStdString(textFromUser));
	_successMessage = convertToSystemString(magicMemo->getSuccessMessage());
	//_todayTaskBoxMessage = convertToSystemString(magicMemo->
	//_allTaskBoxMessage = convertToSystemString(magicMemo->displayAll());
}

Void MessageManager::calculateNumberIndex() {
}

Void MessageManager::calculateDateIndex() {
}

Void MessageManager::calculateEventIndex() {
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

String^ MessageManager::convertToSystemString(string inputString) {
	return gcnew String(inputString.c_str());
}

string MessageManager::convertToStdString(String^ inputString) {
	return msclr::interop::marshal_as< std::string >(inputString);
}
