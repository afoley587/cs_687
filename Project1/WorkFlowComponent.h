#pragma once

#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "Reduce.h"
#include "ProgramSettingsStruct.h"

class WorkFlowComponent {
private:
	FileManager fileManager;
	MapManager mapManager;
	SortManager sortManager;
	Reduce reduceManager;
	ProgramSettings programSettings;

public:
	/*
	 * Set Sane Default For each file. Putting in here
	 * So we can easily see what they all are and their
	 * ctors dont hide that and allow for mismatching file
	 * names
	*/
	WorkFlowComponent(ProgramSettings programSettings, FileManager fileMgr) :
		programSettings{programSettings},
		fileManager{ fileMgr },
		mapManager{ MapManager(fileManager, 1024, programSettings.TempDirectory + "\\temp.txt")},
		sortManager{ SortManager(fileManager, programSettings.TempDirectory + "\\temp.txt")},
		reduceManager{ Reduce(fileManager, programSettings.OutputDirectory + "\\results.txt", programSettings.OutputDirectory + "\\SUCCESS.txt")}
	{};

	WorkFlowComponent() {};

	void StartWorkFlow();
};