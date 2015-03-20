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
	vector<HIGHLIGHT>* _timeHighlight;
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

	Void calculateIndexes();

	String^ toString();

	String^ getSuccessMessage();

	String^ getTodayTaskBoxMessage();

	String^ getAllTaskBoxMessage();

	String^ getInputBoxMessage();

	vector<HIGHLIGHT>* getNumberHighlight();

	vector<HIGHLIGHT>* getTimeHighlight();

	vector<HIGHLIGHT>* getDateHighlight();

	vector<HIGHLIGHT>* getEventHighlight();

	Void clearIndexVectors();

	// convert from std::string to System::String^
	String^ convertToSystemString(string);
	
	// convert from System::String^ to std::string
	string convertToStdString(String^);
};

