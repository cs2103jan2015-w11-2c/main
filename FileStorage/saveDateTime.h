#pragma once
#include "Parser.h"
#include "Item.cpp"

class saveDateTime
{
public:
	saveDateTime(void);
	ITEM storeDateTime();
	~saveDateTime(void);
private:
	ITEM content;
};

