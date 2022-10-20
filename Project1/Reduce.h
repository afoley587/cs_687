#ifndef Reduce_h
#define Reduce_h

#include "FileManager.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>


class Reduce {
private:
	FileManager fileManager;
	std::string resultsDirectory;
public:
	std::string finalOutputFile;
	std::string resultsFile;
	Reduce() {};
	Reduce(FileManager fileMgr, std::string resultDir) :
		fileManager{ fileMgr }, 
		resultsDirectory { resultDir },
		resultsFile{ resultDir + "\\" + "resultsFile.txt" },
		finalOutputFile{ resultDir + "\\" + "finalSuccessFile.txt" } {};

	// constructor initializer, takes key and iterator
	// Reduce(string key, vector<int>::iterator it);
	
	// takes a key and iterator of ints - sums iterators and calls export function
	// ========== reduce w/ ITERATOR ==========
	void reduce(std::string key, std::vector<int>::iterator it);
	// ========== reduce w/ VECTOR ==========
	void reduce(std::string key, std::vector<int> vec);



	// writes result to output directoy - creates empty txt file names SUCCESS upon success
	void output(std::string key, int sum);


	// getter/setter for string key
	std::string getkey();
	void setkey(std::string k);

	// getter/setter for int sum
	int getsum();
	void setsum(int s);

	void WriteFinalOutput();
};

#endif // Reduce_h