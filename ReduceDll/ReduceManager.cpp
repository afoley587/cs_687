#include "pch.h"

#include "ReduceManager.h"
#include "FileManager.h"

#include <sstream>

//========== CONSTRUCTOR INITIALIZER ==========

//========== reduce w/ ITERATOR ========== 
void ReduceManager::reduce(std::string key, std::vector<int>::iterator it) {
	for (int x = 0; x < 10; x++) {
		std::cout << *it << "\n";
		it++;
	}
}


// ========== reduce w/ VECTOR ==========
void ReduceManager::reduce(std::string key, std::vector<int> vec) {
	
	int sum = 0;

	// loop iterates and sums through std::vector
	for (int valElement : vec) {
		sum += valElement;
	}

	// calls output method
	output(key, sum);
}

int ReduceManager::reduceForMap( std::vector<int> vec)
{
	int sum = 0;

	for (int valElement : vec) {
		sum += valElement;
	}
	
	return sum;
}


void ReduceManager::output(std::string key, int sum) {
	std::ostringstream formattedKvpElement;
	formattedKvpElement << "KEY: " << key << " , Value: " << sum << std::endl;

	fileManager.append_file(resultsFile, std::vector<std::string> { formattedKvpElement.str()});
}

void ReduceManager::outputMapToFile(std::map<std::string, std::vector<int>> results) {
	std::ostringstream formattedKvpElement;

	for (auto element : results) 
	{
		formattedKvpElement << "KEY: " << element.first << " , Value: " << element.second[0] << std::endl;
	}

	fileManager.append_file(resultsFile, std::vector<std::string> { formattedKvpElement.str()});
}


REDUCEDLL_API ReduceManager* ReduceCreate(FileManager fm, std::string resFile, std::string outFile) {
	return new ReduceManager(fm, resFile, outFile);
}