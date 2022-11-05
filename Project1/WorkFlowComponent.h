#pragma once

#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "ReduceManager.h"
#include "ProgramSettingsStruct.h"
#include <SortManager.h>

class WorkFlowComponent {
private:
	FileManager fileManager;
	MapManager mapManager;
	SortManager sortManager;
	ReduceManager reduceManager;
	ProgramSettings programSettings;
	std::string intermediateFile = "\\temp.txt";
	std::string resultsFile = "\\results.txt";
	std::string successFile = "\\SUCCESS.txt";

public:
	WorkFlowComponent(ProgramSettings programSettings, FileManager fileMgr);
	WorkFlowComponent() {};

	void StartWorkFlow();
};