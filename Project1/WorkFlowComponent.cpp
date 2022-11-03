#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "ReduceManager.h"

void WorkFlowComponent::StartWorkFlow() {
	//Read Input Text From File

	//Start Map Function

    std::vector<std::string> input_files;
    std::vector<std::string> input_file_data;

    fileManager.read_directory(programSettings.InputDirectory, input_files);

	std::string tmpfile     = programSettings.TempDirectory + "\\tempfile.txt";
	std::string successfile = programSettings.OutputDirectory + "\\SUCCESS.txt";

	mapManager.setTempFile(tmpfile);
	sortManager.setInputFile(mapManager.getTempFile());

    for (auto f : input_files) {
        fileManager.read_file(f, input_file_data);
        for (int i = 0; i < input_file_data.size(); i++) {
            bool isLast = (i == input_file_data.size() - 1);
            mapManager.map(input_file_data[i], isLast);
        }
        input_file_data.clear();
    }

	//Start Sort From Map Output File Read
	// fileManager.touch_file(tmpfile);
	std::map<std::string, std::vector<int>> sortedMapResults = sortManager.SortInput();

	//Start Reduce From Sort Results
	// Call Reduce 
	
	fileManager.touch_file(reduceManager.getResultsFile());

	for (auto keyValuePair : sortedMapResults)
	{
		reduceManager.reduce(keyValuePair.first, keyValuePair.second);
	}

	//Output Results
	

	//Write Final 'SUCCESS' to Final output file

	fileManager.touch_file(successfile);
}
