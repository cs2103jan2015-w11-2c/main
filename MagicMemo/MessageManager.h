#pragma once

#include <vector>
#include <sstream>
#include <string>
#include "Controller.h"
#include "msclr\marshal_cppstd.h"

using namespace std;
using namespace System;


ref class MessageManager {
private:
	Controller* magicMemo;
	String^ _userInput;
	String^ _successMessage;
	String^ _todayTaskBoxMessage;
	String^ _allTaskBoxMessage;
	String^ _inputBoxMessage;

public:
	MessageManager(void);

	Void generateMessageOutputs(String^);

	String^ getSuccessMessage();

	String^ getTodayTaskBoxMessage();

	String^ getAllTaskBoxMessage();

	String^ getInputBoxMessage();

	// convert from std::string to System::String^
	String^ convertToSystemString(string);
	
	// convert from System::String^ to std::string
	string convertToStdString(String^);
};

