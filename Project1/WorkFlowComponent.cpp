#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "ReduceManager.h"

#include <map>
#include <thread>
#include <time.h>

#define NUM_MAPS 6
#define NUM_BUCKETS 3

void MapDispatch(std::vector<std::string> infiles, std::string tempDir, FileManager fm, MapManager mm);

WorkFlowComponent::WorkFlowComponent(ProgramSettings ps, FileManager fileMgr) {
	programSettings = ps;
	fileManager = fileMgr;
	std::cout << "[WF COMP] - In " << ps.TempDirectory + intermediateFile << std::endl;
	// mapManagers.clear();
	sortManager = SortManager{fileManager, ps.TempDirectory + intermediateFile};
	reduceManager = ReduceManager{ fileManager, ps.OutputDirectory + resultsFile, ps.OutputDirectory + successFile };
	std::cout << "[WF COMP] - leaving ctor" << std::endl;
}

void WorkFlowComponent::StartWorkFlow() {
	//Read Input Text From File

	//Start Map Function

    std::vector<std::string> input_files;
	std::vector<std::vector<std::string>> batches;
	std::vector<std::thread> map_threads;

	std::cout << "[WF COMP] - Reading Input Directory " << std::endl;

	fileManager.read_directory(programSettings.InputDirectory, input_files);

	std::cout << "[WF COMP] - Running Map Utility " << std::endl;

	const int subVectorSize = static_cast<int>(ceil(static_cast<double>(input_files.size()) / static_cast<double>(NUM_MAPS)));

	for (int i = 0; i < NUM_MAPS; ++i) {
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

	for (int i = 0; i < NUM_MAPS; i++) {
		std::cout << "[WF COMP] - Dispatching Thread With " << batches[i].size() << " files." << std::endl;
		map_threads.push_back(std::thread(MapDispatch, batches[i], programSettings.TempDirectory, fileManager, mapManagers[i]));
	}

	for (std::thread& t : map_threads) { t.join(); };

	std::cout << "[WF COMP] - Map Finished. Sorting." << std::endl;

	//Start Sort From Map Output File Read
	std::map<std::string, std::vector<int>> sortedMapResults = sortManager.SortInput();

	//Start Reduce From Sort Results
	// Call Reduce 

	std::cout << "[WF COMP] - Sort Finished. Reducing." << std::endl;
	for (auto keyValuePair : sortedMapResults)
	{
		reduceManager.reduce(keyValuePair.first, keyValuePair.second);
	}
	std::cout << "[WF COMP] - Reduce Finished. Creating Success File." << std::endl;
	fileManager.touch_file(programSettings.OutputDirectory + successFile);
}

void MapDispatch(std::vector<std::string> infiles, std::string tempDir, FileManager fm, MapManager mm) {
	std::vector<std::string> buff;
	std::stringstream new_tf;
	new_tf << tempDir << "\\M" << std::this_thread::get_id();
	mm.setTempFile(new_tf.str());

	for (auto f : infiles) {
		std::cout << "Reading " << f << std::endl;
		fm.read_file(f, buff);

		for (int i = 0; i < buff.size(); i++) {
			bool isLast = (i == buff.size() - 1);
			mm.map(buff[i], isLast);
		}
	}
}