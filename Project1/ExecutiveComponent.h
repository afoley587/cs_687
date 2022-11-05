#pragma once

#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "ProgramSettingsStruct.h"
#include <windows.h>

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

MapManager* MapFactory(HINSTANCE dll) {
	typedef MapManager* (*funcPtr)();
	funcPtr pfnCreate;
	pfnCreate = (funcPtr)GetProcAddress(dll, "Create");
	MapManager* mgr = pfnCreate();
	return mgr;
}