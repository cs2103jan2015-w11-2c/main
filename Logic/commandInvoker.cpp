#include "commandInvoker.h"

CommandInvoker::CommandInvoker(void) {
	_numUndo = 0;
	_undo = new vector<Command *>;
	_redo = new vector<Command *>;
}


CommandInvoker::~CommandInvoker(void) {
}

void CommandInvoker::executeCommand(vector<Item> &vectorStore, Command *command, string &message) {
	command->executeAction(vectorStore);
	message = command->getMessage();

	_numUndo = 0;

	if(_undo->size() >= MAX_UNDO) {
		_undo->erase(_undo->begin());
	}

	_undo->push_back(command);
}

void CommandInvoker::executeCommand(FileStorage *outputFile, Command *command, string &message) {
	command->executeAction(outputFile);
	message = command->getMessage();
}

void CommandInvoker::undo(vector<Item> &vectorStore, string &message) {
	if(_undo->empty()) {
		return;
	}

	_numUndo++;

	Command *command = _undo->back();
	command->negateAction(vectorStore);
	message = _undo->back()->getMessage();
	
	if(_redo->size() >= 20) {
		_redo->erase(_redo->begin());
	}
	_redo->push_back(_undo->back());
	
	_undo->pop_back();

	message = SUCCESS_UNDO + message;
}

void CommandInvoker::redo(vector<Item> &vectorStore, string &message) {
	if(_redo->empty() || _numUndo <= 0) {
		return;
	}

	_numUndo--;

	Command *command =_redo->back();
	command->executeAction(vectorStore);
	message = _redo->back()->getMessage();
	
	if(_undo->size() >= MAX_REDO) {
		_undo->erase(_undo->begin());
	}
	_undo->push_back(_redo->back());
	
	_redo->pop_back();
}