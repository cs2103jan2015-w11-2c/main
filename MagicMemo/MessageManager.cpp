#include "MessageManager.h"
//@author A0111951N
MessageManager::MessageManager(void) {
	magicMemo = new Controller();
	_allTaskVector = new vector<RESULT>;
	_todayTaskVector = new vector<RESULT>;

	_allNumberHighlight = new vector<HIGHLIGHT>;
	_allDateHighlight = new vector<HIGHLIGHT>;
	_allEndDateHighlight = new vector<HIGHLIGHT>;
	_allTimeHighlight = new vector<HIGHLIGHT>;
	_allEventHighlight = new vector<HIGHLIGHT>;
	_allCompletedHighlight = new vector<HIGHLIGHT>;

	_todayNumberHighlight = new vector<HIGHLIGHT>;
	_todayDateHighlight = new vector<HIGHLIGHT>;
	_todayEndDateHighlight = new vector<HIGHLIGHT>;
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
	calculateAllTaskIndexes();
	_allTaskBoxMessage = toString(_allTaskVector);

	//todayTaskBox
	calculateTodayTaskIndexes();
	_todayTaskBoxMessage = toString(_todayTaskVector);

	//Textbox size
	isBoxExtended = magicMemo->isWide();
}

Void MessageManager::calculateAllTaskIndexes() {
	clearAllTaskIndexVectors();
	string prevDate = "";
	int indexCount = 0;
	for(unsigned int i = 0; i < _allTaskVector->size(); i++) {
		HIGHLIGHT temp;
		if(_allTaskVector->at(i).isExpired) {
			temp.special = "expired";
		} else if(_allTaskVector->at(i).isClash) {
			temp.special = "clash";
		} else {
			temp.special = "";
		}

		if(_allTaskVector->at(i).date != prevDate) {
			temp.index = indexCount;
			temp.length = _allTaskVector->at(i).date.length();
			_allDateHighlight->push_back(temp);
			indexCount = indexCount + temp.length + 1;
			prevDate = _allTaskVector->at(i).date;
		}

		int numberPos = indexCount;
		temp.index = indexCount;
		temp.length = _allTaskVector->at(i).lineNumber.length();
		_allNumberHighlight->push_back(temp);

		temp.index = temp.index + temp.length + 1;
		temp.length = _allTaskVector->at(i).time.length();
		_allTimeHighlight->push_back(temp);

		if(temp.length > 0) {
			temp.index++;
		}

		temp.index = temp.index + temp.length;
		temp.length = _allTaskVector->at(i).endDate.length();
		_allEndDateHighlight->push_back(temp);

		if(temp.length > 0) {
			temp.index++;
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

		if(temp.length > 0) {
			temp.index++;
		}

		temp.index = temp.index + temp.length;
		temp.length = _todayTaskVector->at(i).endDate.length();
		_todayEndDateHighlight->push_back(temp);

		if(temp.length > 0) {
			temp.index++;
		}

		temp.index = temp.index + temp.length;
		temp.length = _todayTaskVector->at(i).event.length();
		_todayEventHighlight->push_back(temp);

		indexCount = temp.index + temp.length + 1;
	}
}

Void MessageManager::colorAllTaskBox(RichTextBox^ allTaskBox) {
	colorTextInTaskBox(_allNumberHighlight, _allDateHighlight, _allTimeHighlight, _allEndDateHighlight, _allEventHighlight, allTaskBox);
}

Void MessageManager::colorTodayTaskBox(RichTextBox^ todayTaskBox) {
	colorTextInTaskBox(_todayNumberHighlight, _todayDateHighlight, _todayTimeHighlight, _todayEndDateHighlight, _todayEventHighlight, todayTaskBox);
}

Void MessageManager::colorTextInTaskBox(
	vector<HIGHLIGHT>* _numberHighlight, 
	vector<HIGHLIGHT>* _dateHighlight,
	vector<HIGHLIGHT>* _timeHighlight,
	vector<HIGHLIGHT>* _endDateHighlight,
	vector<HIGHLIGHT>* _eventHighlight, 
	RichTextBox^ taskBox) {
		taskBox->SelectAll();
		taskBox->SelectionHangingIndent = 20;

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
			if(_timeHighlight->at(i).special == "expired") {
				taskBox->SelectionColor = System::Drawing::Color::Red;
			} else if(_timeHighlight->at(i).special == "clash") {
				taskBox->SelectionColor = System::Drawing::Color::DarkOrange;
			} else {
				taskBox->SelectionColor = System::Drawing::Color::DarkGreen;
			}
			taskBox->SelectionFont = gcnew System::Drawing::Font("Palatino Linotype", 10, FontStyle::Regular);
			taskBox->SelectionAlignment = HorizontalAlignment::Left;
		}

		//end date
		for(unsigned int i = 0; i < _endDateHighlight->size(); i++) {
			taskBox->Select(_endDateHighlight->at(i).index, _endDateHighlight->at(i).length);
			if(_timeHighlight->at(i).special == "clash") {
				taskBox->SelectionColor = System::Drawing::Color::Orange;
			} else if(_timeHighlight->at(i).special == "expired") {
				taskBox->SelectionColor = System::Drawing::Color::Crimson;
			} else {
				taskBox->SelectionColor = System::Drawing::Color::Brown;
			}
			taskBox->SelectionFont = gcnew System::Drawing::Font("Palatino Linotype", 10, FontStyle::Regular);
			taskBox->SelectionAlignment = HorizontalAlignment::Left;
		}

		//end date
		for(unsigned int i = 0; i < _endDateHighlight->size(); i++) {
			taskBox->Select(_endDateHighlight->at(i).index, _endDateHighlight->at(i).length);
			taskBox->SelectionColor = System::Drawing::Color::DarkGreen;
			taskBox->SelectionFont = gcnew System::Drawing::Font("Palatino Linotype", 10, FontStyle::Regular);
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
	vector<string> autoComplete = magicMemo->getInputBank();

	for(unsigned int i = 0; i < autoComplete.size(); i++) {
		String^ temp = "search " + convertToSystemString(autoComplete[i]);
		inputBox->AutoCompleteCustomSource->Add(temp);
	}

}

Void MessageManager::toggleTaskBoxSize(RichTextBox^ allTaskBox, RichTextBox^ todayTaskBox, PictureBox^ pictureBox) {
	if(isBoxExtended) {
		isBoxExtended = false;
		allTaskBox->Location = System::Drawing::Point(315, 75);
		allTaskBox->Size = System::Drawing::Size(260, 255);
		todayTaskBox->Location = System::Drawing::Point(16, 75);
		todayTaskBox->Size = System::Drawing::Size(260, 255);

		pictureBox->Location = System::Drawing::Point(13, 67);
		pictureBox->Size = System::Drawing::Size(565, 265);
		pictureBox->Image = System::Drawing::Image::FromFile("resources//notebookShort.png");
	} else {
		isBoxExtended = true;
		allTaskBox->Location = System::Drawing::Point(315, 22);
		allTaskBox->Size = System::Drawing::Size(260, 310);
		todayTaskBox->Location = System::Drawing::Point(16, 22);
		todayTaskBox->Size = System::Drawing::Size(260, 310);

		pictureBox->Location = System::Drawing::Point(13, 12);
		pictureBox->Size = System::Drawing::Size(565, 323);
		pictureBox->Image = System::Drawing::Image::FromFile("resources//notebookTall.png");
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

		if(taskVector->at(i).time != "") {
			oss << taskVector->at(i).time << " ";
		}

		if(taskVector->at(i).endDate != "") {
			oss << taskVector->at(i).endDate << " ";
		}

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

String^ MessageManager::getAllTaskBoxLabel() {
	if(magicMemo->isSearch()) {
		return LABEL_IS_SEARCH;
	} else if(magicMemo->isHelp()) {
		return LABEL_IS_HELP;
	} else {
		return LABEL_ALL_TASKS;
	}
}

Void MessageManager::clearAllTaskIndexVectors() {
	_allNumberHighlight->clear();
	_allDateHighlight->clear();
	_allEndDateHighlight->clear();
	_allTimeHighlight->clear();
	_allEventHighlight->clear();
	_allCompletedHighlight->clear();
}

Void MessageManager::clearTodayTaskIndexVectors() {
	_todayNumberHighlight->clear();
	_todayDateHighlight->clear();
	_todayEndDateHighlight->clear();
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
