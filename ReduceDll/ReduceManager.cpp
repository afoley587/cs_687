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

void ReduceManager::output(std::string key, int sum) {
	std::ostringstream formattedKvpElement;
	formattedKvpElement << "KEY: " << key << " , Value: " << sum << std::endl;

	fileManager.append_file(resultsFile, std::vector<std::string> { formattedKvpElement.str()});
}
