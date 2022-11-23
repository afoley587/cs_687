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

#include "../MapFunctor/MapFunctor.h"
#include "../SortFunctor/SortFunctor.h"
#include "../ReduceFunctor/ReduceFunctor.h"

using namespace std::chrono_literals;

// void MapDispatch(std::vector<std::string> infiles, std::string tempDir, FileManager fm, MapManager* mm);

WorkFlowComponent::WorkFlowComponent(ProgramSettings ps, FileManager fileMgr) {
	programSettings = ps;
	fileManager = fileMgr;
	std::cout << "[WF COMP] - In " << ps.TempDirectory + intermediateFile << std::endl;
	// mapManagers.clear();
	sortManager = SortManager{fileManager, ps.TempDirectory };
	reduceManager = ReduceManager{ fileManager, ps.OutputDirectory + resultsFile, ps.OutputDirectory + successFile };
	std::cout << "[WF COMP] - leaving ctor" << std::endl;
}

void WorkFlowComponent::StartWorkFlow() {
	//Read Input Text From File

	//Start Map Function

    std::vector<std::string> input_files;
	std::vector<std::vector<std::string>> batches;
	std::vector<std::thread> map_threads;
	std::map<std::string, std::vector<std::string>> sortThreadMetaMap;
    std::vector<std::string> reduce_tempFiles;

	//tp.Init(programSettings.NumMappers);

	//std::cout << "[WF COMP] - Reading Input Directory " << std::endl;

	//fileManager.read_directory(programSettings.InputDirectory, input_files);

	//std::cout << "[WF COMP] - Running Map Utility " << std::endl;

	//const int subVectorSize = static_cast<int>(ceil(static_cast<double>(input_files.size()) / static_cast<double>(programSettings.NumMappers)));

	//for (int i = 0; i < programSettings.NumMappers; ++i) {
	//	std::vector<std::string> batch;
	//	batch.resize(input_files.size() - i * subVectorSize);
	//	auto start_iter = std::next(input_files.begin(), i * subVectorSize);
	//	auto end_iter = input_files.end();
	//	if (i * subVectorSize + subVectorSize < input_files.size()) {
	//		end_iter = std::next(input_files.begin(), i * subVectorSize + subVectorSize);
	//		batch.resize(subVectorSize);
	//	}
	//	std::copy(start_iter, end_iter, batch.begin());
	//	batches.push_back(batch);
	//}

	//std::cout << "[WF COMP] - Vector Batched " << std::endl;

	//// tp.AddJob(MapDispatch, batches[i], programSettings.TempDirectory, fileManager, mapManagers[i]);
	//for (int i = 0; i < programSettings.NumMappers; i++) {
	//	std::cout << "[WF COMP] - Dispatching Thread With " << batches[i].size() << " files." << std::endl;
	//	tp.AddJob(MapFunctor(fileManager, mapManagers[i], batches[i], programSettings.TempDirectory));
	//	// map_threads.push_back(std::thread(MapDispatch, batches[i], programSettings.TempDirectory, fileManager, mapManagers[i]));
	//}


	//tp.Flush();

	std::cout << "[WF COMP] - Map Finished. Sorting." << std::endl;

	tp.Init(programSettings.NumSorters);
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


	//for (int i = 0; i < programSettings.NumMappers; i++) {
	//	std::cout << "[WF COMP] - Dispatching Thread With " << batches[i].size() << " files." << std::endl;
	//	tp.AddJob(MapFunctor(fileManager, mapManagers[i], batches[i], programSettings.TempDirectory));
	//	// map_threads.push_back(std::thread(MapDispatch, batches[i], programSettings.TempDirectory, fileManager, mapManagers[i]));
	//}



	//TODO Create Sort Threads
	
	int threadTempIndex = 0;
	std::vector<std::string> threadKeys;
	for (auto const& imap : sortThreadMetaMap) {
		threadKeys.push_back(imap.first);
	}

	std::vector<std::promise<std::map<std::string, std::vector<int>>>> sortPromises;
	std::vector< std::map<std::string, std::vector<int>>> sortMaps;
	for (int i = 0; i < programSettings.NumSorters; i++) {
		std::cout << "[WF COMP] - Dispatching Thread for Sorting." << std::endl;
		std::map<std::string, std::vector<int>> sortMap;
		std::promise< std::map<std::string, std::vector<int>>> promise = std::promise<std::map<std::string, std::vector<int>>> ();
		//auto sortFunc = SortFunctor(fileManager, sortManagers[i], sortThreadMetaMap[threadKey], std::ref(promise));
		std::string threadKey = threadKeys[i];
		auto sortFunc = SortFunctor(fileManager, sortManagers[i], sortThreadMetaMap[threadKey], std::ref(sortMap));
		sortMaps.push_back(sortMap);
		tp.AddJob(sortFunc);
		//sortPromises.push_back(promise);
	}

	//while (tp.IsExecuting) 
	//{
	//	std::this_thread::sleep_for(2s);
	//}
	//tp.WaitUntilCompleted();
	tp.Flush();

	//TODO Start adding sorting to threadpool
	std::map<std::string, std::vector<int>> sortedMapResults;
	for (auto &sortMap : sortMaps) {
		sortedMapResults.insert(sortMap.begin(), sortMap.end());
	}


	std::string testString = "KennyTestString";



	////Start Sort From Map Output File Read

	////Start Reduce From Sort Results
	//// Call Reduce 

	//std::cout << "[WF COMP] - Sort Finished. Reducing." << std::endl;
	//for (auto keyValuePair : sortedMapResults)
	//{
	//	reduceManager.reduce(keyValuePair.first, keyValuePair.second);
	//}
	//std::cout << "[WF COMP] - Reduce Finished. Creating Success File." << std::endl;
	//fileManager.touch_file(programSettings.OutputDirectory + successFile);
}

/*
void MapDispatch(std::vector<std::string> infiles, std::string tempDir, FileManager fm, MapManager* mm) {
	std::vector<std::string> buff;
	std::stringstream new_tf;
	std::stringstream threadid;
	threadid << std::this_thread::get_id();
	new_tf << tempDir << "\\M" << threadid.str();
	mm->setTempFile(new_tf.str());

	for (auto f : infiles) {
		std::cout << "[WF COMP] - Thread " << threadid.str() << " Reading " << f << std::endl;
		fm.read_file(f, buff);

		for (int i = 0; i < buff.size(); i++) {
			bool isLast = (i == buff.size() - 1);
			mm->map(buff[i], isLast);
		}
		std::cout << "[WF COMP] - Thread " << threadid.str() << " Finished " << f << std::endl;
	}
}
*/