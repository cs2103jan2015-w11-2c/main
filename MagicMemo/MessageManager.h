#pragma once

#include <vector>
#include <sstream>
#include <string>
#include "Controller.h"
#include "msclr\marshal_cppstd.h"

using namespace std;
using namespace System;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Drawing;


struct HIGHLIGHT {
	int index;
	int length;
};

ref class MessageManager {
private:
	static String^ LABEL_IS_SEARCH = "Search Results";
	static String^ LABEL_ALL_TASKS = "Upcoming Tasks";

	Controller* magicMemo;

	vector<RESULT>* _allTaskVector;
	vector<RESULT>* _todayTaskVector;

	vector<HIGHLIGHT>* _allNumberHighlight;
	vector<HIGHLIGHT>* _allDateHighlight;
	vector<HIGHLIGHT>* _allTimeHighlight;
	vector<HIGHLIGHT>* _allEventHighlight;
	vector<HIGHLIGHT>* _allCompletedHighlight;

	vector<HIGHLIGHT>* _todayNumberHighlight;
	vector<HIGHLIGHT>* _todayDateHighlight;
	vector<HIGHLIGHT>* _todayTimeHighlight;
	vector<HIGHLIGHT>* _todayEventHighlight;
	vector<HIGHLIGHT>* _todayCompletedHighlight;


	String^ _userInput;
	String^ _successMessage;
	String^ _todayTaskBoxMessage;
	String^ _allTaskBoxMessage;
	String^ _inputBoxMessage;

public:
	MessageManager(void);

	Void generateMessageOutputs(String^);

	Void calculateAllTaskIndexes();

	Void calculateTodayTaskIndexes();

	Void colorAllTaskBox(RichTextBox^ taskBox);

	Void colorTodayTaskBox(RichTextBox^ taskBox);

	Void colorTextInTaskBox(vector<HIGHLIGHT>* _numberHighlight, 
		vector<HIGHLIGHT>* _dateHighlight,
		vector<HIGHLIGHT>* _timeHighlight,
		vector<HIGHLIGHT>* _eventHighlight, 
		RichTextBox^ taskBox);

	//Auto-complete collection
	Void updateAutoCompleteSource(TextBox^ inputBox);

	String^ toString(vector<RESULT>*);

	String^ getSuccessMessage();

	String^ getTodayTaskBoxMessage();

	String^ getAllTaskBoxMessage();

	String^ getInputBoxMessage();

	String^ getAllTaskBoxLabel();

	Void clearAllTaskIndexVectors();

	Void clearTodayTaskIndexVectors();

	// convert from std::string to System::String^
	String^ convertToSystemString(string);

	// convert from System::String^ to std::string
	string convertToStdString(String^);
};

