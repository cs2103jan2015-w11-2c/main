#pragma once
#include <iostream>
#include <vector>
#include "Item.cpp"


using namespace std;

class Command {
public:
	Command();
	~Command();
	virtual vector<ITEM> executeAction() = 0;
	virtual string getMessage() = 0;
};