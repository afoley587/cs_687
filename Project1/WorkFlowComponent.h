#pragma once

#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "ReduceManager.h"
#include "ProgramSettingsStruct.h"
#include <SortManager.h>
#include "../ThreadPool/ThreadPool.h"
//#include "../ThreadPool/ThreadPoolTemplate.h"

class WorkFlowComponent {
private:
	FileManager fileManager;
	SortManager sortManager;
	ReduceManager reduceManager;
	ProgramSettings programSettings;
	ThreadPool tp;
	std::vector<std::map<std::string, std::vector<int>>> chunkMap(ThreadSafeMap<std::string, std::vector<int>> sortMap);


	std::vector<MapManager *> mapManagers;
	std::vector<ReduceManager *> reduceManagers;
	std::vector<SortManager *> sortManagers;

	std::string intermediateFile = "\\temp.txt";
	std::string resultsFile = "\\results.txt";
	std::string successFile = "\\SUCCESS.txt";
	std::map<std::string, double> executionTimeLog;
	std::string mapExecutionTimeKey = "MapTime";
	std::string sortExecutionTimeKey = "SortTime";
	std::string reduceExecutionTimeKey = "ReduceTime";

	void PrintExecutionTimeResults();

public:
	WorkFlowComponent(ProgramSettings programSettings, FileManager fileMgr);
	WorkFlowComponent() {};

	inline std::string GetIntermediateFile(void) { return intermediateFile; };
	inline std::string GetResultsFile(void) { return resultsFile; };
	inline std::string GetSuccessFile(void) { return successFile; };
	inline void AddMapManager(MapManager* m) { mapManagers.push_back(m); };
	inline void AddReduceManager(ReduceManager* m) { reduceManagers.push_back(m); };
	inline void AddSortManager(SortManager* m) { sortManagers.push_back(m); };
	inline void SetReduceManager(ReduceManager m) { reduceManager = m; };
	inline void SetSortManager(SortManager m) { sortManager = m; };

	void StartWorkFlow();

	WorkFlowComponent(const WorkFlowComponent& ws) : 
		programSettings{ws.programSettings},
		fileManager{ ws.fileManager },
		mapManagers{ ws.mapManagers },
		sortManager{ ws.sortManager },
		reduceManager{ ws.reduceManager }
	{
		std::cout << "[WF COMP] - COPY Constructur" << std::endl;
	};

	WorkFlowComponent(WorkFlowComponent&& ws) :
		programSettings{ ws.programSettings },
		fileManager{ ws.fileManager },
		mapManagers{ ws.mapManagers },
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
		mapManagers = std::move(ws.mapManagers);
		fileManager = std::move(ws.fileManager);
		reduceManager = std::move(ws.reduceManager);
		sortManager = std::move(ws.sortManager);
		return *this;
	}
};