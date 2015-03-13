#include "MessageManager.h"


MessageManager::MessageManager(void) {
	magicMemo = new Controller();
	_userInput = "";
	_successMessage = "";
	_mainOutputBoxMessage = "";
	_inputBoxMessage = "";
}

MessageManager::MessageManager(String^ textFromUser) {
	magicMemo = new Controller();
	_userInput = textFromUser;
	generateMessageOutputs();
}

Void MessageManager::generateMessageOutputs() {
}

String^ MessageManager::getSuccessMessage() {
	return _successMessage;
}

String^ MessageManager::getMainOutputBoxMessage() {
	return _mainOutputBoxMessage;
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
