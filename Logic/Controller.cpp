#include "Controller.h"

#include "easylogging++.h"
#define ELPP_THREAD_SAFE
#define ELPP_DISABLE_LOGS
#include "easylogging++.h"

const string Controller::ERROR_FILE_OPERATION_FAILED = "File updating failed!\n";

INITIALIZE_EASYLOGGINGPP;


Controller::Controller(void) {
	// Load configuration from file
	el::Configurations conf("logging.conf");
	// Reconfigure single logger
	el::Loggers::reconfigureLogger("default", conf);
	// Actually reconfigure all loggers instead
	el::Loggers::reconfigureAllLoggers(conf);
	// Now all the loggers will use configuration from file

	_parser = new Parser;
	_outputFile = FileStorage::getInstance();
	_invoker = new CommandInvoker;
	initializeVector();
	_isSearch = false;
	_isWide = true;
}

void Controller::executeCommand(string inputText) {
	string userCommand = "";
	string commandData = "";
	string searchQuery = "";
	Item data;

	_parser->setStringToParse(inputText);
	_parser->extractUserCommand();

	searchQuery = _parser->getItem().event;
	_parser->extractDateAndTime();

	userCommand = _parser->getUserCommand();
	data = _parser->getItem();
	commandData = data.event;

	if(userCommand != "") {
		addToInputBank();
	}


	if(userCommand == "search") {
		_isSearch = true;
	} else {
		_isSearch = false;
	}

	if (userCommand == "display") {
		displayAll();
	} else if (userCommand == "add") {
		addData(data);
	} else if (userCommand == "delete") {
		deleteData();
	} else if (userCommand == "clear") {
		clearAll();
	} else if (userCommand == "sort") {
		sortAlphabetical();
	} else if (userCommand == "search") {
		search(data, searchQuery);
	} else if (userCommand == "copy") {
		copy(data);
	} else if (userCommand == "edit") {
		edit(data);
	} else if (userCommand == "rename") {
		rename(commandData);
	} else if (userCommand == "move" || userCommand == "save") {
		move(commandData);
	} else if (userCommand == "undo") {
		undo();
	} else if (userCommand == "redo") {
		redo();
	} else if (userCommand == "view" || userCommand == "more") {
		toggleIsWide();
	} else if (userCommand == "12") {
		setClockTo12Hour();
	} else if (userCommand == "24") {
		setClockTo24Hour();
	} else if (userCommand == "exit") {
		setSuccessMessage("exit");
	}
}

//API for UI (Main Text Box)
string Controller::getInputBoxMessage() {
	return _inputBoxMessage;
}

//API for UI (Message Box)
string Controller::getSuccessMessage() {
	return _successMessage;
}

void Controller::setInputBoxMessage(string message) {
	_inputBoxMessage = message;
}

void Controller::setSuccessMessage(string message) {
	_successMessage = message;
}

void Controller::initializeVector() {
	_vectorStore = _outputFile->getAllFileData();
}

long Controller::getTimePos(const int date[3], const int time[2]) {
	long timePos = 0;

	timePos += date[2];
	timePos *= 12;
	timePos += date[1];
	timePos *= 31;
	timePos += date[0];
	timePos *= 24;
	timePos += time[1];
	timePos *= 60;
	timePos += time[0];

	return timePos;
}

bool Controller::checkIsClash(const Item item1, const Item item2) {
	long startTimePos1 = getTimePos(item1.eventDate, item1.eventStartTime);
	long endTimePos1 =  getTimePos(item1.eventEndDate, item1.eventEndTime);
	long startTimePos2 = getTimePos(item2.eventDate, item2.eventStartTime);
	long endTimePos2 =  getTimePos(item2.eventEndDate, item2.eventEndTime);
	bool isDeadline1 = checkIsDeadline(item1);
	bool isDeadline2 = checkIsDeadline(item2);
	
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

bool Controller::checkIsDeadline(const Item item) {
	for (int i = 0; i < 2; i++) {
		if (item.eventEndTime[i] != 0) {
			return false;
		}
	}
	return true;
}

bool Controller::checkIsExpired(const Item item) {
	DateTime dateTime;

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
					} else if (item.eventStartTime[1] == dateTime.getCurrentMinute()) {
						return false;
					}
				}
			}
		}
	}
	return false;
}

void Controller::generateResults(vector<Item> inputVector) {
	vector<RESULT> todayResult;
	vector<RESULT> otherResult;
	bool isClashed = false;
	DateTime newDateTime;

	for (unsigned int i = 0; i < inputVector.size(); i++) {
		RESULT temp;

		temp.isDeadline = checkIsDeadline(inputVector[i]);
		if (i < inputVector.size() - 1) {
			bool willClash = checkIsClash(inputVector[i], inputVector[i + 1]);
			if (isClashed || willClash) {
				temp.isClash = true;
			} else {
				temp.isClash = false;
			}
			isClashed = willClash;
		} 

		temp.lineNumber = to_string(i + 1) + ".";
		temp.date = inputVector[i].dateToString();
		_is12HourFormat ? temp.time = inputVector[i].timeToString() : temp.time = inputVector[i].timeTo24HrString();
		temp.endDate = inputVector[i].endDateToString();
		temp.event = inputVector[i].event;
		temp.isExpired = checkIsExpired(inputVector[i]);
		if ((inputVector[i].eventDate[0] == newDateTime.getCurrentDay() ||
			inputVector[i].eventDate[0] == newDateTime.getCurrentDay() + 1) &&
			inputVector[i].eventDate[1] == newDateTime.getCurrentMonth() &&
			inputVector[i].eventDate[2] == newDateTime.getCurrentYear()) {
				todayResult.push_back(temp);
		} else {
			otherResult.push_back(temp);
		}
	}
	_todayResult = todayResult;
	_otherResult = otherResult;

}

bool Controller::rewriteFile() {
	_outputFile->clearFile();
	for (unsigned int i = 0; i < _vectorStore.size(); i++) {
		_outputFile->addLineToFile(_vectorStore[i]);
	}
	return true;
}

void Controller::commandOptions(string command) {

}

void Controller::addData(Item item) {
	AddItem *addItemCommand = new AddItem(item);
	_invoker->executeCommand(_vectorStore, addItemCommand, _successMessage);

	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::deleteData() {
	DeleteItem *deleteItemCommand = new DeleteItem(_parser->getLineOpNumber());

	_invoker->executeCommand(_vectorStore, deleteItemCommand, _successMessage);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

int Controller::getLineNumberForOperation() {
	unsigned int lineNumber = 0;
	try {
		//lineNumber = _parser->getLineOpNumber();
		if (lineNumber <= 0 || lineNumber > _vectorStore.size()) {
			return 0;
		}
	} catch (const out_of_range& e) {
		LOG(ERROR) << "getLinenumberForOperation() throws: " << e.what();
		clog << e.what();
		return 0;
	}

	return lineNumber;

}

void Controller::displayAll() {
	generateResults(_vectorStore);
	//return _vectorStore;
}

void Controller::clearAll() {
	ClearItems *clearItemsCommand = new ClearItems;

	_invoker->executeCommand(_vectorStore, clearItemsCommand, _successMessage);

	if(!_outputFile->clearFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::sortAlphabetical() {
	SortAlphabetical *sortAlphabeticalCommand = new SortAlphabetical();
	_invoker->executeCommand(_vectorStore, sortAlphabeticalCommand, _successMessage);

	generateResults(_vectorStore);
}

void Controller::search(Item data, string message) {
	vector<Item> tempVector = _vectorStore;

	_parser->extractSearchQuery(data);

	SearchItem *searchItemCommand = new SearchItem(data, message, &_otherResult);
	_invoker->disableUndo();
	_invoker->executeCommand(tempVector, searchItemCommand, _successMessage);
}

bool Controller::isSearch() {
	return _isSearch;
}

void Controller::toggleIsWide() {
	_isWide = !_isWide;
}

bool Controller::isWide() {
	return _isWide;
}

void Controller::copy(Item input) {
	CopyItem *copyItemCommand = new CopyItem(_parser->getLineOpNumber()[0], input);
	_invoker->executeCommand(_vectorStore, copyItemCommand, _successMessage);

	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::edit(Item data) {
	int lineNumber = _parser->getLineOpNumber()[0];

	_parser->extractUserCommand();
	Item item = _parser->getItem();

	EditItem *editItemCommand = new EditItem(lineNumber, item);

	_invoker->executeCommand(_vectorStore, editItemCommand, _successMessage);

	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::rename(string newFileName) {
	RenameFile *renameFileCommand = new RenameFile(newFileName);
	_invoker->executeCommand(_outputFile, renameFileCommand, _successMessage);

}

void Controller::move(string newFileLocation) {
	MoveFileLocation *moveFileCommand = new MoveFileLocation;
	_invoker->executeCommand(_outputFile, moveFileCommand, _successMessage);
}

void Controller::undo() {
	_invoker->undo(_vectorStore, _successMessage);
	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::redo() {
	_invoker->redo(_vectorStore, _successMessage);

	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

string Controller::getHelp() {
	ostringstream oss;
	oss << "Command:		Program execution:\n";
	oss << "add xxx		(line xxx is added to the text file with a line number)\n";
	oss << "delete #		(the line with the corresponding # is deleted)\n";
	oss << "display		(all data in the file is displayed)\n";
	oss << "clear			(all data in the file is deleted)\n";
	oss << "sort			(all data in the file sorted alphabetically)\n";
	oss << "search xxx		(all lines with xxx displayed)\n";
	oss << "exit			(program quits)\n";
	return oss.str();
}

vector<RESULT> Controller::getTodayResult() {
	return _todayResult;
}

vector<RESULT> Controller::getOtherResult() {
	return _otherResult;
}

void Controller::swap(Item &item1, Item &item2) {
	Item tempItem = item1;
	item1 = item2;
	item2 = tempItem;
}

int Controller::compareEarlierThan(const Item item1, const Item item2) {
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

void Controller::chronoSort(vector<Item> &vectorStore) {
	for (int i = 0; i < ((int)vectorStore.size() - 1); i++) {
		int minIndex = i;
		for (unsigned int j = i + 1; j < vectorStore.size(); j++) {
			if (compareEarlierThan(vectorStore[j], vectorStore[minIndex]) < 0) {
				minIndex = j;
			}
		}
		if(minIndex != i) {
			swap(vectorStore[minIndex], vectorStore[i]);
		}
	}
}

void Controller::addToInputBank() {
	vector<string> fragEvent = _parser->getFragmentedEvent();
	vector<string>::iterator iter1;
	vector<string>::iterator iter2;

	for(iter1 = fragEvent.begin(); iter1 != fragEvent.end(); iter1++) {
		bool isFound = false;
		for(iter2 = _inputBank.begin(); iter2 != _inputBank.end(); iter2++) {
			if (*iter1 == *iter2) {
				isFound = true;
				break;
			}
		}
		if (!isFound) {
			_inputBank.push_back(*iter1);
		}
	}
}

vector<string> Controller::getInputBank() {
	return _inputBank;
}

void Controller::setClockTo12Hour() {
	_is12HourFormat = true;
}

void Controller::setClockTo24Hour() {
	_is12HourFormat = false;
}

Controller::~Controller(void) {
}