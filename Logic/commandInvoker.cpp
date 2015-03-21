#include "commandInvoker.h"


CommandInvoker::CommandInvoker(void) {
	_undo = new stack<Command>;
	_redo = new stack<Command>;
}


CommandInvoker::~CommandInvoker(void) {
}

void CommandInvoker::executeCommand(vector<Item> &vectorStore, Command *command, string &message) {
	command->executeAction(vectorStore);
	message = command->getMessage();

	_redo->push(*command);
	_undo->push(*command);
}

void CommandInvoker::executeCommand(FileStorage *outputFile, Command *command, string &message) {
	command->executeAction(outputFile);
	message = command->getMessage();
}