#pragma once
#include <iostream>
#include <vector>


using namespace std;

class Command {
public:
	Command();
	~Command();
	virtual vector<string> executeAction()=0;
	virtual string getMessage()=0;
};