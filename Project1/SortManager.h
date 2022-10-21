#pragma once

#include "FileManager.h"

#include <algorithm>
#include <iterator>
#include <functional>
#include <sstream>
#include <iostream>
#include <deque>
#include <map>
#include <stack>

class SortManager {
public:
	SortManager() {};
	SortManager(FileManager fileMgr, std::string input) :
		fileManager{ fileMgr },
		sortInputFile{ input } 
	{};
	
	std::map<std::string, std::vector<int>> SortInput(void);

	void setInputFile(std::string in) { sortInputFile = in; };
	std::string getInputFile(void) { return sortInputFile; };

private:
	FileManager fileManager;
	std::string sortInputFile;
};