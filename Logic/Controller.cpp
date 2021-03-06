#include "Controller.h"
//@author A0116179B
#include "easylogging++.h"
#define ELPP_THREAD_SAFE
#define ELPP_DISABLE_LOGS
#include "easylogging++.h"

const string Controller::SUCCESS_12_HR = "Date format changed to 12-hr format";
const string Controller::SUCCESS_24_HR = "Date format changed to 24-hr format";
const string Controller::SUCCESS_NOTIFICATION_TIME_CHANGED = "Notification time changed from %d minute(s) to %d minute(s)";
const string Controller::SUCCESS_SLEEP = "Sleep time changed to %d:%s - %d:%s";
const string Controller::SUCCESS_NOTIFICATION_ON = "Notifications turned on";
const string Controller::SUCCESS_NOTIFICATION_OFF = "Notifications turned off";
const string Controller::SUCCESS_RESTORE_FILE_DEFAULTS = "Default filename and filepath restored";
const string Controller::ERROR_FILE_RESTORE_FAILED = "File restore failed, unable to overwrite existing file!\n";
const string Controller::ERROR_FILE_OPERATION_FAILED = "File updating failed!\n";
const string Controller::ERROR_INVALID_LINE_NUMBER = "getLineOpNumber() throws: ";
const string Controller::ERROR_INVALID_NOTIFICATION_TIME = "Invalid Notification time!";
const string Controller::ERROR_INCORRECT_ARGUMENTS = "Invalid arguments used";
const int Controller::MAX_NOTIFICATION = 30240;

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
	initializeOptions();
	initializeVector();
	_isSearch = false;
	_isFree = false;
	_isHelp = false;
	_isArchiveSearch = false;
	_sleepTime[0][0] = DEFAULT_SLEEP_START_HR;
	_sleepTime[0][1] = DEFAULT_SLEEP_START_MIN;
	_sleepTime[1][0] = DEFAULT_SLEEP_END_HR;
	_sleepTime[1][1] = DEFAULT_SLEEP_END_MIN;
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

	_isSearch = false;


	if((userCommand == "help") || (userCommand == "?")) {
		_isHelp = true;
	} else {
		_isHelp = false;
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
		_isSearch = true;
		search(data, searchQuery);
	} else if (userCommand == "free") {
		_isFree = true;
		searchFree(data, searchQuery);
	}else if (userCommand == "copy") {
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
	} else if (userCommand == "help" || userCommand == "?") {
		getHelp();
	} else if (userCommand == "sleep") {
		setSleepTime(data);
	} else if ((userCommand == "notify") || (userCommand == "notification")){
		toggleNotification();
	} else if ((userCommand == "reminder") || (userCommand == "remind")){
		setReminderTime();
	} else if (userCommand == "done" || userCommand == "mark") {
		markAsComplete();
	} else if (userCommand == "archive") {
		_isArchiveSearch = true;
		viewArchive(data, searchQuery);
	} else if (userCommand == "restore") {
		restoreDefaultFileInfo();
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
	_inputBank = _outputFile->getAutoCompleteFileData();
}

void Controller::initializeOptions() {
	vector<int> options = _outputFile->getOptionFileData();
	_is12HourFormat = (options[0] == 1) ? true : false;
	_isWide = (options[1] == 1) ? true : false;
	_isNotificationsOn = (options[2] == 1) ? true : false;
	_notifyTime = options[3];
	_sleepTime[0][0] = options[4];
	_sleepTime[0][1] = options[5];
	_sleepTime[1][0] = options[6];
	_sleepTime[1][1] = options[7];
}

long Controller::getTimePos(const int date[3], const int time[2]) {
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

bool Controller::checkDateIsUnset(const int date[3]) {
	for (int i = 0; i < 3; i++) {
		if (date[i] != 0) {
			return false;
		}
	}
	return true;
}

bool Controller::checkIsClash(Item item1, Item item2) {
	long startTimePos1 = getTimePos(item1.eventDate, item1.eventStartTime);
	long endTimePos1;
	if (checkDateIsUnset(item1.eventEndDate)) {
		if (item1.eventEndTime[0] == 0 && item1.eventEndTime[1] == 0) {
			endTimePos1 =  getTimePos(item1.eventDate, item1.eventStartTime);
		} else {
			endTimePos1 =  getTimePos(item1.eventDate, item1.eventEndTime);
		}
	} else {
		if (item1.eventEndTime[0] == 0 && item1.eventEndTime[1] == 0) {
			endTimePos1 =  getTimePos(item1.eventEndDate, item1.eventStartTime);
		} else {
			endTimePos1 =  getTimePos(item1.eventEndDate, item1.eventEndTime);
		}
	}

	long startTimePos2 = getTimePos(item2.eventDate, item2.eventStartTime);
	long endTimePos2;
	if (checkDateIsUnset(item2.eventEndDate)) {
		if (item2.eventEndTime[0] == 0 && item2.eventEndTime[1] == 0) {
			endTimePos2 =  getTimePos(item2.eventDate, item2.eventStartTime);
		} else {
			endTimePos2 =  getTimePos(item2.eventDate, item2.eventEndTime);
		}
	} else {
		if (item2.eventEndTime[0] == 0 && item2.eventEndTime[1] == 0) {
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

bool Controller::checkIsExpired(Item item) {
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
	} else if (checkDateIsUnset(item.eventEndDate)){
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

void Controller::generateResults(const vector<Item> vectorStore) {
	vector<Item> inputVector = vectorStore;
	vector<RESULT> todayResult;
	vector<RESULT> otherResult;
	vector<RESULT> floatResult;
	vector<RESULT> deadlineResult;
	DateTime newDateTime;

	for (unsigned int i = 0; i < inputVector.size(); i++) {
		RESULT temp;

		temp.isDeadline = inputVector[i].isDeadline();

		temp.isClash = false;
		for (unsigned int j = 0; j < inputVector.size(); j++) {
			if (i != j && checkIsClash(vectorStore[i], vectorStore[j])) {
				temp.isClash = true;
				break;
			}
		}

		temp.lineNumber = to_string(i + 1) + ".";
		temp.date = inputVector[i].dateToString();
		
		if (_is12HourFormat) {
			temp.time = inputVector[i].timeAndEndDateToString();
		} else { 
			temp.time = inputVector[i].timeTo24HrString();
		}
		if (inputVector[i].eventEndTime[0] == 0 && inputVector[i].eventEndTime[1] == 0) {
			temp.time += inputVector[i].endDateToString();
		}

		temp.event = inputVector[i].event;
		if(!inputVector[i].isFloating()) {
			temp.isExpired = checkIsExpired(inputVector[i]);
		} else {
			temp.isExpired = false;
		}
		if (inputVector[i].isFloating()) {
			floatResult.push_back(temp);
		} else if (temp.isDeadline) {
			for (int j = 0; j < 3; j++) {
				inputVector[i].eventEndDate[j] = inputVector[i].eventDate[j];
			}
			if (_is12HourFormat) {
				temp.time = inputVector[i].timeToString();
			} else {
				temp.time = inputVector[i].timeTo24HrString();
			}
			if (inputVector[i].eventEndTime[0] == 0 && inputVector[i].eventEndTime[1] == 0) {
				temp.time += inputVector[i].endDateToString();
			}
			temp.date = DEADLINE_HEADER;
			
			deadlineResult.push_back(temp);
		} else if ((inputVector[i].eventDate[0] == newDateTime.getCurrentDay() ||
			inputVector[i].eventDate[0] == newDateTime.getCurrentDay() + 1) &&
			inputVector[i].eventDate[1] == newDateTime.getCurrentMonth() &&
			inputVector[i].eventDate[2] == newDateTime.getCurrentYear()) {
				todayResult.push_back(temp);
		} else {
			otherResult.push_back(temp);
		}
	}

	otherResult.insert(otherResult.begin(), deadlineResult.begin(), deadlineResult.end());
	otherResult.insert(otherResult.begin(), floatResult.begin(), floatResult.end());
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

void Controller::addData(Item item) {
	AddItem *addItemCommand = new AddItem(item);

	try {
		_invoker->executeCommand(_vectorStore, addItemCommand, _successMessage);
	} catch (const logic_error& e) {
		setSuccessMessage(e.what());
		LOG(ERROR) << e.what();
		return;
	}

	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::deleteData() {
	DeleteItem *deleteItemCommand;
	try {
		deleteItemCommand = new DeleteItem(_parser->getLineOpNumber(), false, _outputFile);
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
		LOG(ERROR) << ERROR_INVALID_LINE_NUMBER << e.what();
		clog << e.what();
		return;
	}

	_invoker->executeCommand(_vectorStore, deleteItemCommand, _successMessage);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
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

	try {
		if (data.event != "" && data.isFloating()) { 
			_parser->extractSearchQuery(data);
		}
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
		LOG(INFO) << e.what();
		return;
	}
	SearchItem *searchItemCommand;

	searchItemCommand= new SearchItem(data, message, &_otherResult, _sleepTime, false, _is12HourFormat);

	_invoker->disableUndo();
	_invoker->executeCommand(tempVector, searchItemCommand, _successMessage);
}

void Controller::searchFree(Item data, string message) {
	vector<Item> tempVector = _vectorStore;

	int lineNumber;
	try {
		lineNumber = _parser->getLineOpNumberForFree()[0];
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
		LOG(ERROR) << ERROR_INVALID_LINE_NUMBER << e.what();
		clog << e.what();
		return;
	}

	_parser->extractUserCommand();
	Item item = _parser->getItem();
	
	if (item.event != "" || !item.isFloating()) {
		try {
			_parser->extractSearchQuery(item);
		} catch (const out_of_range& e) {
			setSuccessMessage(e.what());
			LOG(INFO) << e.what();
			return;
		}
	}

	item.event = to_string(lineNumber);

	SearchItem *searchItemCommand = new SearchItem(item, message, &_otherResult, _sleepTime, true, _is12HourFormat);
	_invoker->disableUndo();
	try {
		_invoker->executeCommand(tempVector, searchItemCommand, _successMessage);
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
		LOG(INFO) << e.what();
		return;
	}
}

bool Controller::isSearch() {
	return _isSearch;
}

bool Controller::isFree() {
	return _isFree;
}

void Controller::toggleIsWide() {
	_isWide = !_isWide;
	_outputFile->saveIsWide(_isWide);
	setSuccessMessage("");
}

bool Controller::isWide() {
	return _isWide;
}

bool Controller::isHelp() {
	return _isHelp;
}

void Controller::copy(Item input) {
	CopyItem *copyItemCommand;
	try {
		copyItemCommand = new CopyItem(_parser->getLineOpNumber()[0], input);
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
		LOG(ERROR) << ERROR_INVALID_LINE_NUMBER << e.what();
		clog << e.what();
		return;
	}
	_invoker->executeCommand(_vectorStore, copyItemCommand, _successMessage);

	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::edit(Item data) {
	bool floatDateIsOverwritten = _parser->getIsDateUpdatedFromFloat();
	
	int lineNumber;
	try {
		lineNumber = _parser->getLineOpNumber()[0];
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
		LOG(ERROR) << ERROR_INVALID_LINE_NUMBER << e.what();
		clog << e.what();
		return;
	}
	_parser->extractUserCommand();
	Item item = _parser->getItem();
	if(_parser->getIsDateUpdatedFromFloat()) {
		item.eventDate[0] = 0;
		item.eventDate[1] = 0;
		item.eventDate[2] = 0;
	}

	EditItem *editItemCommand = new EditItem(lineNumber, item);
	try {
		_invoker->executeCommand(_vectorStore, editItemCommand, _successMessage);
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
	}
	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::rename(string newFileName) {
	RenameFile *renameFileCommand = new RenameFile(newFileName);
	try {
		_invoker->executeCommand(_outputFile, renameFileCommand, _successMessage);
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
		LOG(INFO) << e.what();
	}

}

void Controller::move(string newFileLocation) {
	MoveFileLocation *moveFileCommand = new MoveFileLocation(newFileLocation);
	try {
		_invoker->executeCommand(_outputFile, moveFileCommand, _successMessage);
	} catch (const invalid_argument& e) {
		setSuccessMessage(e.what());
		LOG(INFO) << e.what();
	}
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

void Controller::getHelp() {
	vector<RESULT> otherResult;

	for (int i = 0; i < NUM_HELP_COMMANDS; i++) {
		RESULT temp;

		temp.date = HELP_COMMANDS[i];
		temp.event = "    " + HELP_DESCRIPTION[i];

		otherResult.push_back(temp);
	}
	_otherResult = otherResult;
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
			_outputFile->addLineToAutoCompleteFile(*iter1);
		}
	}
}

vector<string> Controller::getInputBank() {
	return _inputBank;
}

void Controller::setClockTo12Hour() {
	_is12HourFormat = true;
	generateResults(_vectorStore);
	_outputFile->saveIs12Hr(_is12HourFormat);
	_successMessage = SUCCESS_12_HR;
}

void Controller::setClockTo24Hour() {
	_is12HourFormat = false;
	generateResults(_vectorStore);
	_outputFile->saveIs12Hr(_is12HourFormat);
	_successMessage = SUCCESS_24_HR;
}

void Controller::setSleepTime(Item data) {
	try {
		_parser->extractSearchQuery(data);
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
		LOG(INFO) << e.what();
		return;
	}
	
	bool startIsSet = false;
	bool endIsSet = false;

	for (int i = 0; i < 2; i++) {
		if (data.eventStartTime[i] != 0) {
			startIsSet = true;
		
		}
		if (data.eventEndTime[i] != 0) {
			endIsSet = true;
		}
		if (startIsSet && endIsSet) {
			break;
		}
	}

	if (!startIsSet || !endIsSet) {
		setSuccessMessage(ERROR_INCORRECT_ARGUMENTS);
		return;
	}

	if (data.eventStartTime[0] < 12) {
		data.eventStartTime[0] += 12;
	} else if (data.eventStartTime[0] == 12) {
		data.eventStartTime[0] = 23;
		data.eventStartTime[1] = 59;
	}
	
	if (data.eventEndTime[0] > 12) {
		data.eventEndTime[0] -= 12;
	}

	_sleepTime[0][0] = data.eventStartTime[0];
	_sleepTime[0][1] = data.eventStartTime[1];
	_sleepTime[1][0] = data.eventEndTime[0];
	_sleepTime[1][1] = data.eventEndTime[1];
	
	char buffer[1000];
	string startMins = to_string(_sleepTime[0][1]);
	if (_sleepTime[0][1] < 10) {
		startMins = "0" + startMins;
	}
	string endMins = to_string(_sleepTime[1][1]);
	if (_sleepTime[1][1] < 10) {
		endMins = "0" + endMins;
	}
	
	sprintf_s(buffer, SUCCESS_SLEEP.c_str(), _sleepTime[0][0], startMins.c_str(), _sleepTime[1][0], endMins.c_str()); 
	setSuccessMessage(buffer);
	_outputFile->saveSleepTime(_sleepTime);
	
}

//@author A0111951N
bool Controller::isNotificationsOn() {
	return _isNotificationsOn;
}

string Controller::getNotifications() {
	int targetMin;
	int targetHr;
	int targetDay;
	int targetMon; 
	int targetYr;

	calculateTargetDateTime(targetMin, targetHr, targetDay, targetMon, targetYr);
	return findEventMatch(targetMin, targetHr, targetDay, targetMon, targetYr);
}

void Controller::calculateTargetDateTime (
	int& targetMin, 
	int& targetHr,
	int& targetDay,
	int& targetMon, 
	int& targetYr) {
		DateTime today;
		targetMin = today.getCurrentMinute() + _notifyTime;
		targetHr = today.getCurrentHour();
		targetDay = today.getCurrentDay();
		targetMon = today.getCurrentMonth();
		targetYr = today.getCurrentYear();

		if(targetMin >= 60) {
			targetHr += targetMin / 60;
			targetMin %= 60;
		}

		if(targetHr >= 24) {
			targetDay += targetHr / 24;
			targetHr %= 24;
		}

		if(targetDay > today.numDaysInMonth(targetMon, targetYr)) {
			targetMon++;
			targetDay -= today.numDaysInMonth(targetMon, targetYr);
		}

		if(targetMon > 12) {
			targetYr++;
			targetMon -= 12;
		}

		//convert hour to 1-24 format
		if(targetHr == 0) {
			targetHr = 24;
		}
}

string Controller::findEventMatch (
	int targetMin, 
	int targetHr,
	int targetDay,
	int targetMon, 
	int targetYr) {
		ostringstream oss;

		for(unsigned int i = 0; i < _vectorStore.size(); i++) {
			Item temp = _vectorStore[i];
			
			if(	(temp.eventDate[0] == targetDay) &&
				(temp.eventDate[1] == targetMon) &&
				(temp.eventDate[2] == targetYr) &&
				(temp.eventStartTime[0] == targetHr) &&
				(temp.eventStartTime[1] == targetMin)) {
					oss << temp.dateToString() << " ";
					oss << temp.timeAndEndDateToString() << " ";
					oss << temp.event << "\n";
			}
		}
		
		return oss.str();
}

void Controller::setReminderTime() {
	int numMinutes;
	try {
		numMinutes = _parser->getLineOpNumber()[0];
	} catch (const out_of_range& e) {
		setSuccessMessage(ERROR_INVALID_NOTIFICATION_TIME);
		LOG(ERROR) << ERROR_INVALID_NOTIFICATION_TIME << e.what();
		clog << e.what();
		return;
	}
	assert(numMinutes > 0);
	if(numMinutes > MAX_NOTIFICATION) {
		_successMessage = ERROR_INVALID_NOTIFICATION_TIME;
		return;
	}
	char buffer[1000];
	sprintf_s(buffer, SUCCESS_NOTIFICATION_TIME_CHANGED.c_str(), _notifyTime, numMinutes);
	_notifyTime = numMinutes;
	_outputFile->saveNotifications(_isNotificationsOn, _notifyTime);
	_successMessage = buffer;
}

void Controller::toggleNotification() {
	_isNotificationsOn = !_isNotificationsOn;
	_outputFile->saveNotifications(_isNotificationsOn, _notifyTime);

	if(_isNotificationsOn) {
		_successMessage = SUCCESS_NOTIFICATION_ON;
	} else {
		_successMessage = SUCCESS_NOTIFICATION_OFF;
	}
}

//@author A0116179B
void Controller::markAsComplete() {

	DeleteItem *deleteItemCommand;
	try {
		deleteItemCommand = new DeleteItem(_parser->getLineOpNumber(), true, _outputFile);
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
		LOG(ERROR) << ERROR_INVALID_LINE_NUMBER << e.what();
		return;
	}
	_invoker->executeCommand(_vectorStore, deleteItemCommand, _successMessage);

	vector<Item> markedItems = deleteItemCommand->getMarkedItems();

	for (unsigned int i = 0; i < markedItems.size(); i++) {
		_outputFile->addLineToArchive(markedItems[i]);
	}

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::viewArchive(Item data, string message) {
	vector<Item> tempVector = _outputFile->getArchiveData();

	try {
		_parser->extractSearchQuery(data);
	} catch (const out_of_range& e) {
		setSuccessMessage(e.what());
		LOG(INFO) << e.what();
		return;
	}
	SearchItem *searchItemCommand;

	searchItemCommand= new SearchItem(data, message, &_otherResult, _sleepTime, false, _is12HourFormat);

	_invoker->disableUndo();
	_invoker->executeCommand(tempVector, searchItemCommand, _successMessage);
}

bool Controller::isArchiveSearch() {
	return _isArchiveSearch;
}

void Controller::restoreDefaultFileInfo() {
	if(_outputFile->restoreFileInfo()) {
		_successMessage = SUCCESS_RESTORE_FILE_DEFAULTS;
	} else {
		_successMessage =  ERROR_FILE_RESTORE_FAILED;
	}
}

Controller::~Controller(void) {
}
