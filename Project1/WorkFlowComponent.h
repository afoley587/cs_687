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
	std::string resultsFile;
	std::string finalOutputFile;
	
public:
	
	WorkFlowComponent(ProgramSettings programSettings) :
		workingDirectory{programSettings.WorkingDirectory},
		resultsFile{programSettings.ResultsFile},
		finalOutputFile{programSettings.FinalOutputFile},
		fileManager{ FileManager(programSettings.WorkingDirectory)},
		mapManager{ MapManager(fileManager, 1024) },
		sortManager{ SortManager(fileManager, programSettings.SortInputFile) },
		reduceManager{ Reduce(fileManager, programSettings.ResultsFile, programSettings.FinalOutputFile)}
	{};

	void StartWorkFlow();
};