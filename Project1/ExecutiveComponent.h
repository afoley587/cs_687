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
	FileManager fileManager;
	ProgramSettings programSettings;

	bool ValidateArgs(std::vector<std::string> args, ProgramSettings& programSettings);
	bool ValidateFile(std::string filePath);
	ProgramSettings ParseArgs(int argCount, char* args[]);
};