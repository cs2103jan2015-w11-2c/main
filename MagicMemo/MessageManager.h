#pragma once

#include <vector>
#include <sstream>
#include <string>
#include "Controller.h"
#include "msclr\marshal_cppstd.h"

using namespace std;
using namespace System;

struct HIGHLIGHT {
	int index;
	int length;
};

ref class MessageManager {
private:
	Controller* magicMemo;
	
	vector<RESULT>* _resultVector;

	vector<HIGHLIGHT>* _numberHighlight;
	vector<HIGHLIGHT>* _dateHighlight;
	vector<HIGHLIGHT>* _eventHighlight;
	vector<HIGHLIGHT>* _completedHighlight;
	

	String^ _userInput;
	String^ _successMessage;
	String^ _todayTaskBoxMessage;
	String^ _allTaskBoxMessage;
	String^ _inputBoxMessage;

public:
	MessageManager(void);

	Void generateMessageOutputs(String^);

	Void calculateNumberIndex();

	Void calculateDateIndex();

	Void calculateEventIndex();

	String^ getSuccessMessage();

	String^ getTodayTaskBoxMessage();

	String^ getAllTaskBoxMessage();

	String^ getInputBoxMessage();

	// convert from std::string to System::String^
	String^ convertToSystemString(string);
	
	// convert from System::String^ to std::string
	string convertToStdString(String^);
};

