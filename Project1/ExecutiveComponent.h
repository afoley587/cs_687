#pragma once

#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "ProgramSettingsStruct.h"

class ExecutiveComponent {
public: 
	ExecutiveComponent(int argCount, char* args[]);

	void RunProgram();

private:
	WorkFlowComponent workFlowComponent;
	std::string workingDirectory;
	std::string InputFilePath;

	bool ValidateArgs(int argCount, char* args[]);
	ProgramSettings ParseArgs(int argCount, char* args[]);
};