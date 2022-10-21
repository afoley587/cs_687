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
	std::string finalOutputFile;
	std::string resultsFile;
public:

	Reduce() {};
	Reduce(FileManager fileMgr, std::string resultsFile, std::string successFile) :
		fileManager{ fileMgr }, 
		resultsFile{ resultsFile },
		finalOutputFile{ successFile } {};


	void setResultsFile(std::string r) { resultsFile = r; };
	std::string getResultsFile(void) { return resultsFile; };
	void setFinalOutputFile(std::string f) { finalOutputFile = f; };
	std::string getFinalOutputFile(void) { return finalOutputFile; };
	
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
};

#endif // Reduce_h