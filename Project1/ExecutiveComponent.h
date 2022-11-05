#pragma once

#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "ProgramSettingsStruct.h"
#include <windows.h>

typedef MapManager* (*funcPtr)();

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
	// typedef MapManager* (*funcCreateMapManager)();
};

MapManager* MapFactory(HINSTANCE dll);

/*
ReduceManager* ReduceFactory(HINSTANCE dll) {
	typedef ReduceManager* (*rmFuncPtr)();
	rmFuncPtr pfnCreate;
	pfnCreate = (rmFuncPtr)GetProcAddress(dll, "RmCreate");
	ReduceManager* mgr = pfnCreate();
	return mgr;
}
*/
