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

	threadPool.Init(programSettings->NumSorters);

	fileManager.read_directory(programSettings->TempDirectory, reduce_tempFiles);

	int fileIndex = 0;
	for (std::string reduce_temp_file_path : reduce_tempFiles)
	{
		std::string tempFile = fileManager.get_filename_from_path(reduce_temp_file_path);
		std::size_t found = tempFile.find('_');
		std::string threadKey = tempFile.substr(0, found);
		std::cout << "[WF COMP] - File Name is." << threadKey << std::endl;

		if (sortThreadMetaMap.count(threadKey) > 0)
		{
			sortThreadMetaMap[threadKey].push_back(reduce_temp_file_path);
		}
		else
		{
			std::vector<std::string> tempFilesVector{ reduce_temp_file_path };
			sortThreadMetaMap.insert({ threadKey, tempFilesVector });
		}
		fileIndex++;
	}

	int threadTempIndex = 0;
	std::vector<std::string> threadKeys;
	for (auto const& imap : sortThreadMetaMap) {
		threadKeys.push_back(imap.first);
	}

	ThreadSafeMap<std::string, std::vector<int>> sortMap = ThreadSafeMap<std::string, std::vector<int>>();

	for (int i = 0; i < programSettings->NumSorters; i++) {
		std::cout << "[WF COMP] - Dispatching Thread for Sorting." << std::endl;
		std::string threadKey = threadKeys[i];
		auto sortFunc = SortFunctor(fileManager, sortManagers[i], sortThreadMetaMap[threadKey], &sortMap);
		threadPool.AddJob(sortFunc);
	}

	threadPool.Flush();

	return sortMap;
}

void SortOrchestrator::GenerateSortManagers() 
{

}



