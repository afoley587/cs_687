#pragma once

#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "ProgramSettingsStruct.h"

class ExecutiveComponent {
public: 
	ExecutiveComponent(int argCount, char* args[]);

	void RunProgram();

private:
	ProgramSettings programSettings;
	std::string workingDirectory;
	std::string InputFilePath;

	bool ValidateArgs(void);
	bool prompt_for_dir(FileManager fm, std::string dirname);
	ProgramSettings ParseArgs(int argCount, char* args[]);
};