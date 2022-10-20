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
	WorkFlowComponent(ProgramSettings programSettings, FileManager fileMgr) :
		fileManager{ fileMgr },
		mapManager{ MapManager(fileManager, 1024, programSettings.InputDirectory) },
		sortManager{ SortManager(fileManager, programSettings.TempDirectory) },
		reduceManager{ Reduce(fileManager, programSettings.OutputDirectory)}
	{
	};

	WorkFlowComponent();

	void StartWorkFlow();
};