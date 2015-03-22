#include "Command.h"

Command::Command() {
}

Command::~Command() {
}

void Command::executeAction(FileStorage *outputFile) {
}

void Command::executeAction(vector<Item>& vectorStore) {
}

void Command::negateAction(vector<Item> &vectorStore) {
}

string Command::getMessage() {
	return "";
}