#include "Reduce.h"
#include "FileManager.h"

#include <sstream>

 //========== CONSTRUCTOR INITIALIZER ==========

 //========== reduce w/ ITERATOR ========== 
void Reduce::reduce(std::string key, std::vector<int>::iterator it) {
	for (int x = 0; x < 10; x++) {
		std::cout << *it << "\n";
		it++;
	}
}


// ========== reduce w/ VECTOR ==========
void Reduce::reduce(std::string key, std::vector<int> vec) {
	int sum = 0;

	// loop iterates and sums through std::vector
	for (int valElement : vec) {
		sum += valElement;
	}

	// calls output method
	output(key, sum);
}

void Reduce::output(std::string key, int sum) {
	std::ostringstream formattedKvpElement;
	formattedKvpElement << "\n" << key << ", " << sum << std::endl;

	fileManager.test_output(formattedKvpElement.str());
}
 

void Reduce::WriteFinalOutput() 
{
	fileManager.write_file(finalOutputFile, std::vector<std::string> { "Success!"});
}