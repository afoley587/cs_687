#pragma once

#include "WorkFlowComponent.h"
#include "FileManager.h"

class ExecutiveComponent {
public: 
	ExecutiveComponent(int argCount, char* args[]);

private:
	FileManager fileManager;
	WorkFlowComponent workFlowComponent;
	std::string workingDirectory;
	std::string InputFilePath;

	bool ValidateArgs(int argCount, char* args[]);
	void ParseArgs(int argCount, char* args[]);
};