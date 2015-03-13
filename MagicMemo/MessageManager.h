#pragma once

#include <vector>
#include <sstream>
#include <string>
#include "msclr\marshal_cppstd.h"

using namespace std;
using namespace System;


ref class MessageManager {
public:
	MessageManager(void);

	// convert from System::String^ to std::string
	string convertToStdString(String^);

	// convert from std::string to System::String^
	String^ convertToSystemString(string);

};

