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
	SortManager(FileManager fileMgr, std::string tempDir) :
		fileManager{ fileMgr }, 
		tempDirectory{ tempDir },
		sortInputFile { tempDir + "\\" + "tmpFile.txt" }
	{};
	
	std::map<std::string, std::vector<int>> SortInput(std::string tmpdir);

private:
	FileManager fileManager;
	std::string tempDirectory;
	std::string sortInputFile;
};