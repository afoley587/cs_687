#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "ReduceManager.h"

#include <map>

WorkFlowComponent::WorkFlowComponent(ProgramSettings ps, FileManager fileMgr) {
	programSettings = ps;
	fileManager = fileMgr;
	std::cout << "[WF COMP] - In " << ps.TempDirectory + intermediateFile << std::endl;
	mapManager = MapManager{ fileManager, 1024, ps.TempDirectory + intermediateFile };
	sortManager = SortManager{fileManager, ps.TempDirectory + intermediateFile};
	reduceManager = ReduceManager{ fileManager, ps.OutputDirectory + resultsFile, ps.OutputDirectory + successFile };
	std::cout << "[WF COMP] - Testing " << this->mapManager.getTempFile() << std::endl;
	std::cout << "[WF COMP] - leaving ctor" << std::endl;
}

void WorkFlowComponent::StartWorkFlow() {
	//Read Input Text From File

	//Start Map Function

    std::vector<std::string> input_files;
    std::vector<std::string> input_file_data;

	std::cout << "[WF COMP] - Reading Input Directory " << std::endl;

	fileManager.read_directory(programSettings.InputDirectory, input_files);

	std::cout << "[WF COMP] - Running Map Utility " << std::endl;

    for (auto f : input_files) {
        fileManager.read_file(f, input_file_data);
        for (int i = 0; i < input_file_data.size(); i++) {
            bool isLast = (i == input_file_data.size() - 1);
            mapManager.map(input_file_data[i], isLast);
        }
        input_file_data.clear();
    }

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
