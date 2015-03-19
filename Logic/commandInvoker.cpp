#include "commandInvoker.h"


CommandInvoker::CommandInvoker(void) {
}


CommandInvoker::~CommandInvoker(void) {
}

void CommandInvoker::executeCommand(vector<Item> &vectorStore, Command *command) {
	command->executeAction(vectorStore);

}