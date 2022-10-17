#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "Reduce.h"

void WorkFlowComponent::StartWorkFlow() {
	//Read Input Text From File

	//Start Map Function

	//Read Map Functions Output File
	std::vector<std::string> mapOutputData;
	fileManager.read_file("C:\\Users\\kenne\\OneDrive\\Masters_Comp_Sci\\CSE_687_OOD\\Project1\\ExampleFiles\\TestSortInput.txt", mapOutputData);

	//Start Sort From Map Output File Read
	std::map<std::string, std::vector<int>> sortedMapResults = sortManager.SortInput(mapOutputData);

	//Start Reduce From Sort Results
	// Call Reduce 
	

	std::cout << "\n" << "\n" << "Results From Reduce Class and Reduce Function: ";
	for (auto kvp : sortedMapResults)
	{
		reduceManager.reduce(kvp.first, kvp.second);
	}

	//Output Results

	//Write 'SUCCESS' to OutFile
}