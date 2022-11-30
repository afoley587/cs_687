#pragma once

#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "ReduceManager.h"

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


class SortOrchestrator {
private:
	ThreadPool threadPool;
	std::vector<SortManager*> sortManagers;
	FileManager fileManager;
	ProgramSettings * programSettings;

	void GenerateSortManagers();

public:
	SortOrchestrator(FileManager _fileManager, ProgramSettings * _programSettings, std::vector<SortManager*> _sortManagers)
	{
		fileManager = _fileManager;
		programSettings = _programSettings;
		sortManagers = _sortManagers;
		//threadPool = _threadPool;
		//threadPool = ThreadPoolTemplate<void>(_threadPool);
	}

	ThreadSafeMap<std::string, std::vector<int>> Sort();
};