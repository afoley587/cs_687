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

	std::string workingDirectory;
	std::string resultsFile; // change to dir
	std::string finalOutputFile; // chaange to dir
	
public:
	
	WorkFlowComponent(ProgramSettings programSettings) :
		workingDirectory{programSettings.WorkingDirectory},
		resultsFile{ programSettings.WorkingDirectory + "\\" + programSettings.ResultsFile},
		fileManager{ FileManager(programSettings.WorkingDirectory)},
		mapManager{ MapManager(fileManager, 1024) },
		sortManager{ SortManager(fileManager, programSettings.SortInputFile) },
		reduceManager{ Reduce(fileManager, programSettings.WorkingDirectory + "\\" + programSettings.FinalOutputFile)}
	{};

	void StartWorkFlow();
};