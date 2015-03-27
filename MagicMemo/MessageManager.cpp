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

	isBoxExtended = false;
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

	//successMessageLabel
	_successMessage = convertToSystemString(magicMemo->getSuccessMessage());

	//allTaskBox
	clearAllTaskIndexVectors();
	calculateAllTaskIndexes();
	_allTaskBoxMessage = toString(_allTaskVector);

	//todayTaskBox
	clearTodayTaskIndexVectors();
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

Void MessageManager::colorAllTaskBox(RichTextBox^ allTaskBox) {
	colorTextInTaskBox(_allNumberHighlight, _allDateHighlight, _allTimeHighlight, _allEventHighlight, allTaskBox);
}

Void MessageManager::colorTodayTaskBox(RichTextBox^ todayTaskBox) {
	colorTextInTaskBox(_todayNumberHighlight, _todayDateHighlight, _todayTimeHighlight, _todayEventHighlight, todayTaskBox);
}

Void MessageManager::colorTextInTaskBox(vector<HIGHLIGHT>* _numberHighlight, 
								vector<HIGHLIGHT>* _dateHighlight,
								vector<HIGHLIGHT>* _timeHighlight,
								vector<HIGHLIGHT>* _eventHighlight, 
								RichTextBox^ taskBox) {

									//date
									for(unsigned int i = 0; i < _dateHighlight->size(); i++) {
										taskBox->Select(_dateHighlight->at(i).index,_dateHighlight->at(i).length);
										taskBox->SelectionColor = System::Drawing::Color::DarkBlue;
										taskBox->SelectionFont = gcnew System::Drawing::Font("Cooper", 10, FontStyle::Bold);
										taskBox->SelectionAlignment = HorizontalAlignment::Center;
									}

									//number
									for(unsigned int i = 0; i < _numberHighlight->size(); i++) {
										taskBox->Select(_numberHighlight->at(i).index, _numberHighlight->at(i).length);
										taskBox->SelectionColor = System::Drawing::Color::Black;
										taskBox->SelectionFont = gcnew System::Drawing::Font("Georgia", 11, FontStyle::Italic);
										taskBox->SelectionAlignment = HorizontalAlignment::Left;
									}

									//time
									for(unsigned int i = 0; i < _timeHighlight->size(); i++) {
										taskBox->Select(_timeHighlight->at(i).index, _timeHighlight->at(i).length);
										taskBox->SelectionColor = System::Drawing::Color::DarkGreen;
										taskBox->SelectionFont = gcnew System::Drawing::Font("Palatino Linotype", 11, FontStyle::Regular);
										taskBox->SelectionAlignment = HorizontalAlignment::Left;
									}

									//event
									for(unsigned int i = 0; i < _eventHighlight->size(); i++) {
										taskBox->Select(_eventHighlight->at(i).index, _eventHighlight->at(i).length);
										taskBox->SelectionColor = System::Drawing::Color::Black;
										taskBox->SelectionFont = gcnew System::Drawing::Font("Palatino Linotype", 11, FontStyle::Regular);
										taskBox->SelectionAlignment = HorizontalAlignment::Left;
									}
}

Void MessageManager::updateAutoCompleteSource(TextBox^ inputBox) {
	inputBox->AutoCompleteCustomSource->Clear();

	inputBox->AutoCompleteCustomSource->Add("search hello");
	inputBox->AutoCompleteCustomSource->Add("search hi");

}

Void MessageManager::toggleTaskBoxSize(RichTextBox^ allTaskBox, RichTextBox^ todayTaskBox) {
	if(isBoxExtended) {
		isBoxExtended = false;
		allTaskBox->Location = System::Drawing::Point(304, 75);
		allTaskBox->Size = System::Drawing::Size(270, 255);
		todayTaskBox->Location = System::Drawing::Point(19, 75);
		todayTaskBox->Size = System::Drawing::Size(270, 255);
	} else {
		isBoxExtended = true;
		allTaskBox->Location = System::Drawing::Point(304, 10);
		allTaskBox->Size = System::Drawing::Size(270, 325);
		todayTaskBox->Location = System::Drawing::Point(19, 10);
		todayTaskBox->Size = System::Drawing::Size(270, 325);
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

String^ MessageManager::getAllTaskBoxLabel(int& xCoord) {
	if(magicMemo->isSearch()) {
		xCoord = X_COORD_IS_SEARCH;
		return LABEL_IS_SEARCH;
	} else {
		xCoord = X_COORD_ALL_TASKS;
		return LABEL_ALL_TASKS;
	}
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
