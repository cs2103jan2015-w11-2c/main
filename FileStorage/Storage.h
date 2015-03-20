#pragma once
#ifndef STORAGE_H
#define STORAGE_H
#include "FileStorage.h"
#include "Item.h"
#include "Parser.h"

class Storage
{
private:

	bool   _isCorruptedFile;
	bool   _isCorruptedToken;

	void   tokenizeContent(std::string, std::vector<std::string>&);
	void   verifyTokenContent(const std::vector<std::string>&);
	bool   isNumber(const std::string&);
	void   storeDateTime(std::vector<Item> &content);
public:
	Storage(void);
	~Storage(void);
};
#endif
