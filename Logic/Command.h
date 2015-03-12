#pragma once
#include <iostream>
#include <vector>
#include "Item.cpp"


using namespace std;

class Command {
public:
	Command();
	~Command();
	virtual vector<ITEM> executeAction();
	virtual string getMessage();
};