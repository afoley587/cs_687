#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "FileManager.h"

#ifdef REDUCEDLL_EXPORTS
#define SORTDLL_API __declspec(dllexport)
#else
#define SORTDLL_API __declspec(dllimport)
#endif

class SORTDLL_API SortManager {
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