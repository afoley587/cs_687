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

#include <iostream>
#include <cstdlib>

#include "../MapFunctor/MapFunctor.h"
#include "../SortFunctor/SortFunctor.h"
#include "../ReduceFunctor/ReduceFunctor.h"
#include "../ThreadPool/ThreadPool.h"
#include "StopWatch.h"
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


	StopWatch stopWatch;
	stopWatch.Start();
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

	stopWatch.Stop();
	executionTimeLog[mapExecutionTimeKey] = cast_duration<double>(stopWatch.elapsed());

	std::cout << "[WF COMP] - Map Finished. Sorting." << std::endl;
	
	stopWatch.Start();
	SortOrchestrator sortOrchestrator = SortOrchestrator(fileManager, &programSettings, sortManagers);
	ThreadSafeMap<std::string, std::vector<int>> sortResult = sortOrchestrator.Sort();

	stopWatch.Stop();
	executionTimeLog[sortExecutionTimeKey] = cast_duration<double>(stopWatch.elapsed());
	

	stopWatch.Start();
	auto subMapVec = chunkMap(sortResult);
	stopWatch.Stop();
	executionTimeLog["Chunking Sort Results"] = cast_duration<double>(stopWatch.elapsed());

	stopWatch.Start();
	ReduceOrchestrator reduceOrchestrator = ReduceOrchestrator(fileManager, programSettings, reduceManagers);
	
	ThreadSafeMap<std::string, std::vector<int>> reduceResult = reduceOrchestrator.Reduce(subMapVec);

	fileManager.touch_file(programSettings.OutputDirectory + "\\" + successFile);
	stopWatch.Stop();
	executionTimeLog[reduceExecutionTimeKey] = cast_duration<double>(stopWatch.elapsed());
	
	PrintExecutionTimeResults();

	//std::cout << std::endl << std::endl << std::endl << "[WF COMP] - Validation of a count: " << sortResult.get("a").size() << std::endl;
}

void WorkFlowComponent::PrintExecutionTimeResults() 
{
	std::cout << '\n' <<  "*** PRINTING RESULTS FOR EXECUTION TIME: " << '\n';
	double totalTime = 0.0;
	for (auto element : executionTimeLog) 
	{
		double elapsedTime = element.second;
		std::cout << std::endl;
		std::cout << "\t" << element.first << " Time Elapsed : " << elapsedTime << " sec. " << '\n';
		std::cout << std::endl;
		totalTime += elapsedTime;
	}

	std::cout <<"*** TOTAL TIME ELAPSED: " << totalTime << " sec. " << '\n';
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
