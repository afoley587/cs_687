#include "ReduceOrchestrator.h"
#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "ReduceManager.h"

#include <map>
#include <thread>
#include <time.h>
#include <future>
#include <chrono>

#include "../ReduceFunctor/ReduceFunctor.h"
#include "../ThreadPool/ThreadPoolTemplate.h"
#include "../ThreadPool/ThreadPoolTemplate.cpp"

ThreadSafeMap<std::string, std::vector<int>> ReduceOrchestrator::Reduce(std::vector<std::map<std::string, std::vector<int>>> tempMap)
{
	std::vector<std::string> reduce_tempFiles;

	threadPool.Init(programSettings.NumReducers);

	ThreadSafeMap<std::string, std::vector<int>> reduceMap = ThreadSafeMap<std::string, std::vector<int>>();

	std::vector<std::promise<std::map<std::string, std::vector<int>>>> reducePromises;

	for (int i = 0; i < programSettings.NumReducers; i++) {
		std::cout << "[REDUCE ORCHESTRATOR] - Dispatching Thread for Reducing." << std::endl;
		auto reduceFunc = ReduceFunctor( reduceManagers[i], &reduceMap, tempMap[i]);
		threadPool.AddJob(reduceFunc);
	}

	threadPool.Flush();

	reduceManagers[0]->outputMapToFile(reduceMap.getData());
	//for (auto const& [key, val] : reduceMap.getData())
	//{
	//	reduceManagers[0]->output(key, val[0]);
	//}

	/*for (auto element : reduceMap.getData())
	{
		reduceManagers[0].output(reduceMap.first, reduceMap.second);
	}*/

	return std::move(reduceMap);
}




