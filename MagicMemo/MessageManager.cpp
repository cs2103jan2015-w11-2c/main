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

