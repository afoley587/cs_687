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

	inline std::string GetIntermediateFile(void) { return intermediateFile; };
	inline std::string GetResultsFile(void) { return resultsFile; };
	inline std::string GetSuccessFile(void) { return successFile; };
	inline void SetMapManager(MapManager m) { mapManager = m; };
	inline void SetReduceManager(ReduceManager m) { reduceManager = m; };
	inline void SetSortManager(SortManager m) { sortManager = m; };

	void StartWorkFlow();

	WorkFlowComponent(const WorkFlowComponent& ws) : 
		programSettings{ws.programSettings},
		fileManager{ ws.fileManager },
		mapManager{ ws.mapManager },
		sortManager{ ws.sortManager },
		reduceManager{ ws.reduceManager }
	{
		std::cout << "[WF COMP] - COPY Constructur" << std::endl;
	};

	WorkFlowComponent(WorkFlowComponent&& ws) :
		programSettings{ ws.programSettings },
		fileManager{ ws.fileManager },
		mapManager{ ws.mapManager },
		sortManager{ ws.sortManager },
		reduceManager{ ws.reduceManager } 
	{
		std::cout << "[WF COMP] - MOVE Constructur" << std::endl;
	};

	WorkFlowComponent& operator=(const WorkFlowComponent& ws) {
		programSettings = ws.programSettings;
		std::cout << "copy" << std::endl;
		return *this;
	}

	WorkFlowComponent& operator=(WorkFlowComponent&& ws) {
		std::cout << "[WF COMP] - Move Op=" << std::endl;
		std::cout << "[WF COMP] - mine premove " << this->programSettings.InputDirectory << std::endl;
		std::cout << "[WF COMP] - theirs premove " << ws.programSettings.InputDirectory << std::endl;
		programSettings = std::move(ws.programSettings);
		mapManager = std::move(ws.mapManager);
		fileManager = std::move(ws.fileManager);
		reduceManager = std::move(ws.reduceManager);
		sortManager = std::move(ws.sortManager);
		return *this;
	}
};