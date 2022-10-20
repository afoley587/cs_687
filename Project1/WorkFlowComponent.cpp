#include "WorkFlowComponent.h"
#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "Reduce.h"


WorkFlowComponent::WorkFlowComponent() {
}

void WorkFlowComponent::StartWorkFlow() {
	//Read Input Text From File

	//Start Map Function

	//Start Sort From Map Output File Read
	std::map<std::string, std::vector<int>> sortedMapResults = sortManager.SortInput();

	//Start Reduce From Sort Results
	// Call Reduce 
	
	for (auto keyValuePair : sortedMapResults)
	{
		reduceManager.reduce(keyValuePair.first, keyValuePair.second);
	}

	//Output Results
	

	//Write Final 'SUCCESS' to Final output file
	reduceManager.WriteFinalOutput();
}