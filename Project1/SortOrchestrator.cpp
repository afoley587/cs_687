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

#include "../SortFunctor/SortFunctor.h"
#include "SortOrchestrator.h"
#include "../ThreadPool/ThreadPoolTemplate.h"
#include "../ThreadPool/ThreadPoolTemplate.cpp"

ThreadSafeMap<std::string, std::vector<int>> SortOrchestrator::Sort()
{
	std::map<std::string, std::vector<std::string>> sortThreadMetaMap;

	std::vector<std::string> reduce_tempFiles;

	threadPool.Init(programSettings.NumSorters);
	//TODO
	//Read Map Temp Files
	//	- Get Number of Threads
	//	- Get Files For Each Thread
	fileManager.getFiles_directory(programSettings.TempDirectory, reduce_tempFiles);

	for (std::string reduce_temp_file : reduce_tempFiles)
	{
		std::string tempFile = reduce_temp_file;
		std::size_t found = tempFile.find('_');
		std::string threadKey = tempFile.substr(0, found);
		std::cout << "[WF COMP] - File Name is." << threadKey << std::endl;

		if (sortThreadMetaMap.count(threadKey) > 0)
		{
			sortThreadMetaMap[threadKey].push_back(tempFile);
		}
		else
		{
			std::vector<std::string> tempFilesVector{ tempFile };
			sortThreadMetaMap.insert({ threadKey, tempFilesVector });
		}
	}

	//TODO Create Sort Threads

	int threadTempIndex = 0;
	std::vector<std::string> threadKeys;
	for (auto const& imap : sortThreadMetaMap) {
		threadKeys.push_back(imap.first);
	}

	ThreadSafeMap<std::string, std::vector<int>> sortMap = ThreadSafeMap<std::string, std::vector<int>>();

	std::vector<std::promise<std::map<std::string, std::vector<int>>>> sortPromises;
	for (int i = 0; i < programSettings.NumSorters; i++) {
		std::cout << "[WF COMP] - Dispatching Thread for Sorting." << std::endl;
		std::string threadKey = threadKeys[i];
		auto sortFunc = SortFunctor(fileManager, sortManagers[i], sortThreadMetaMap[threadKey], &sortMap);
		threadPool.AddJob(sortFunc);
	}

	threadPool.Flush();

	return std::move(sortMap);
}

void SortOrchestrator::GenerateSortManagers() 
{

}



