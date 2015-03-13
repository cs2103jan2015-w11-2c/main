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
	String^ _userInput;
	String^ _successMessage;
	String^ _mainOutputBoxMessage;
	String^ _inputBoxMessage;

public:
	MessageManager(void);

	MessageManager(String^);

	Void generateMessageOutputs();

	String^ getSuccessMessage();

	String^ getMainOutputBoxMessage();

	String^ getInputBoxMessage();

	// convert from std::string to System::String^
	String^ convertToSystemString(string);
	
	// convert from System::String^ to std::string
	string convertToStdString(String^);
};

