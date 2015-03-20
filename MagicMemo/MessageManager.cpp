#include "MessageManager.h"


MessageManager::MessageManager(void) {
	magicMemo = new Controller();
	_userInput = "";
	_successMessage = "";
	_todayTaskBoxMessage = "";
	_allTaskBoxMessage = "";
	_inputBoxMessage = "";
}

Void MessageManager::generateMessageOutputs(String^ textFromUser) {
	//string operationSucess = magicMemo->executeCommand(convertToStdString(textFromUser));
	//_successMessage = convertToSystemString(operationSucess);
	//_todayTaskBoxMessage = convertToSystemString(magicMemo->
	//_allTaskBoxMessage = convertToSystemString(magicMemo->displayAll());
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
