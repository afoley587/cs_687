#pragma once

#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "ReduceManager.h"
#include "ProgramSettingsStruct.h"

#include <map>
#include <thread>
#include <time.h>
#include <future>
#include <chrono>

#include "../MapFunctor/MapFunctor.h"
#include "../SortFunctor/SortFunctor.h"
#include "../ReduceFunctor/ReduceFunctor.h"
#include "../ThreadPool/ThreadPool.h"
//#include "../ThreadPool/ThreadPoolTemplate.h"
//#include "../ThreadPool/ThreadPoolTemplate.cpp"


class ReduceOrchestrator {
private:
	ThreadPool threadPool;
	std::vector<ReduceManager*> reduceManagers;
	FileManager fileManager;
	ProgramSettings programSettings;

	void GenerateSortManagers();

public:
	ReduceOrchestrator(FileManager _fileManager, ProgramSettings _programSettings, std::vector<ReduceManager*> _reduceManagers)
	{
		fileManager = _fileManager;
		programSettings = _programSettings;
		reduceManagers = _reduceManagers;
		//threadPool = _threadPool;
		//threadPool = ThreadPoolTemplate<void>(_threadPool);
	}

	ThreadSafeMap<std::string, std::vector<int>> Reduce(std::vector<std::map<std::string, std::vector<int>>>);
};