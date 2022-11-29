#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "ReduceManager.h"
#include "SortOrchestrator.h"
#include "ReduceOrchestrator.h"

#include <map>
#include <thread>
#include <time.h>
#include <future> 
#include <chrono>

#include "../MapFunctor/MapFunctor.h"
#include "../SortFunctor/SortFunctor.h"
#include "../ReduceFunctor/ReduceFunctor.h"
#include "../ThreadPool/ThreadPool.h"
//#include "../ThreadPool/ThreadPoolTemplate.cpp"

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

	tp.Init(programSettings.NumMappers);

	std::cout << "[WF COMP] - Reading Input Directory " << std::endl;

	fileManager.read_directory(programSettings.InputDirectory, input_files);

	std::cout << "[WF COMP] - Running Map Utility " << std::endl;

	const int subVectorSize = static_cast<int>(ceil(static_cast<double>(input_files.size()) / static_cast<double>(programSettings.NumMappers)));

	for (int i = 0; i < programSettings.NumMappers; ++i) {
		std::vector<std::string> batch;
		batch.resize(input_files.size() - i * subVectorSize);
		auto start_iter = std::next(input_files.begin(), i * subVectorSize);
		auto end_iter = input_files.end();
		if (i * subVectorSize + subVectorSize < input_files.size()) {
			end_iter = std::next(input_files.begin(), i * subVectorSize + subVectorSize);
			batch.resize(subVectorSize);
		}
		std::copy(start_iter, end_iter, batch.begin());
		batches.push_back(batch);
	}

	std::cout << "[WF COMP] - Vector Batched " << std::endl;

	for (int i = 0; i < programSettings.NumMappers; i++) {
		std::cout << "[WF COMP] - Dispatching Thread With " << batches[i].size() << " files." << std::endl;
		tp.AddJob(MapFunctor(fileManager, mapManagers[i], batches[i], programSettings.TempDirectory));
	}


	tp.Flush();

	std::cout << "[WF COMP] - Map Finished. Sorting." << std::endl;
	
	SortOrchestrator sortOrchestrator = SortOrchestrator(fileManager, programSettings, sortManagers);
	ThreadSafeMap<std::string, std::vector<int>> sortResult = sortOrchestrator.Sort();

	auto subMapVec = chunkMap(sortResult);
	
	ReduceOrchestrator reduceOrchestrator = ReduceOrchestrator(fileManager, programSettings, reduceManagers);
	
	ThreadSafeMap<std::string, std::vector<int>> reduceResult = reduceOrchestrator.Reduce(subMapVec);

	fileManager.touch_file(programSettings.OutputDirectory + "\\" + successFile);
}

std::vector<std::map<std::string, std::vector<int>>> WorkFlowComponent::chunkMap(ThreadSafeMap<std::string, std::vector<int>> sortMap)
{
	std::vector<std::map<std::string, std::vector<int>>> result;
	std::map<std::string, std::vector<int>> tempMap = sortMap.getData();
	std::map<std::string, std::vector<int>> iteratorMap;


	int mapCount = tempMap.size();
	int index = 1;
	int sliceIndex = 1;
	int bucketCount = 0;

	for ( auto element : tempMap)
	{
		iteratorMap.insert(element);
		if (index >= mapCount)
		{
			result.push_back(iteratorMap);
			break;
		}

		if ( (index % (tempMap.size() / 5)  == 0)  && (bucketCount < 4 ) )
		{
			result.push_back(iteratorMap);
			iteratorMap.clear();
			bucketCount++;
		}

		index++;
	};
	
	return result;
}
