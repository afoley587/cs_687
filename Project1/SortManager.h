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
	SortManager(FileManager fileMgr, std::string sortInputFile) :
		fileManager{ fileMgr }, sortInputFilePath{ sortInputFile } {};
	
	std::map<std::string, std::vector<int>> SortInput();

private:
	FileManager fileManager;
	std::string sortInputFilePath;
};