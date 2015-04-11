#include "commandInvoker.h"

//@author A0111951N

CommandInvoker::CommandInvoker(void) {
	_numRedo = 0;
	_enableUndoFlag = true;
	_undo = new stack<Command *>;
	_redo = new stack<Command *>;
}


CommandInvoker::~CommandInvoker(void) {
}

void CommandInvoker::executeCommand(vector<Item> &vectorStore, Command *command, string &message) {
	command->executeAction(vectorStore);
	message = command->getMessage();

	if(_enableUndoFlag) {
		_numRedo = 0;
		while(!_redo->empty()) {
			_redo->pop();
		}
		_undo->push(command);
	}
	_enableUndoFlag = true;

}

void CommandInvoker::executeCommand(FileStorage *outputFile, Command *command, string &message) {
	command->executeAction(outputFile);
	message = command->getMessage();
}

void CommandInvoker::disableUndo() {
	_enableUndoFlag = false;
}

void CommandInvoker::undo(vector<Item> &vectorStore, string &message) {
	if(_undo->empty()) {
		return;
	}

	_numRedo++;

	Command *command = _undo->top();
	command->negateAction(vectorStore);
	message = _undo->top()->getMessage();
	
	_redo->push(_undo->top());
	
	_undo->pop();

	message = SUCCESS_UNDO + message;
}

void CommandInvoker::redo(vector<Item> &vectorStore, string &message) {
	if(_redo->empty() || _numRedo <= 0) {
		return;
	}

	_numRedo--;

	Command *command =_redo->top();
	command->executeAction(vectorStore);
	message = _redo->top()->getMessage();
	
	_undo->push(_redo->top());
	
	_redo->pop();
}