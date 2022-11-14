#pragma once

#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "ProgramSettingsStruct.h"
#include <windows.h>

typedef MapManager* (*mapManagerCreateFuncPtr)(FileManager, int, std::string);
typedef ReduceManager* (*reduceManagerCreateFuncPtr)(FileManager, std::string , std::string);
typedef SortManager* (*sortManagerCreateFuncPtr)(FileManager, std::string);

class ExecutiveComponent {
public: 
	ExecutiveComponent(int argCount, char* args[]);
	ExecutiveComponent() {};
	void RunProgram();

private:
	WorkFlowComponent workFlowComponent;
	std::string workingDirectory;
	std::string InputFilePath;
	FileManager fileManager;
	ProgramSettings programSettings;

	bool ValidateArgs(std::vector<std::string> argVector);
	bool ValidateFile(std::string filePath);
	bool prompt_for_dir(FileManager fm, std::string dirname);
	ProgramSettings ParseArgs(int argCount, char* args[]);
	void PrintHelp(void);
	HINSTANCE LoadDll(std::string path);
	MapManager* MapFactory(HINSTANCE dll);
	ReduceManager* ReduceFactory(HINSTANCE dll);
	SortManager* SortFactory(HINSTANCE dll);
};