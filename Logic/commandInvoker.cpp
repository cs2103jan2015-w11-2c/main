#include "commandInvoker.h"


CommandInvoker::CommandInvoker(void) {
}


CommandInvoker::~CommandInvoker(void) {
}

void CommandInvoker::executeCommand(vector<Item> &vectorStore, Command *command, string &message) {
	command->executeAction(vectorStore);
	message = command->getMessage();
}

void CommandInvoker::executeCommand(FileStorage *outputFile, Command *command, string &message) {
	command->executeAction(outputFile);
	message = command->getMessage();
}