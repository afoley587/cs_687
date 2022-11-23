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
	SortManager(FileManager fileMgr, std::string inputDirectory) :
		fileManager{ fileMgr },
		sortInputDirectory{ inputDirectory }
	{};

	std::map<std::string, std::vector<int>> SortInputFiles(std::vector<std::string> inputFiles);

	std::map<std::string, std::vector<int>> SortInput(std::string filePath);

	void setInputFile(std::string in) { sortInputDirectory = in; };
	std::string getInputFile(void) { return sortInputDirectory; };

private:
	FileManager fileManager;
	std::string sortInputDirectory;

};

extern "C" SORTDLL_API SortManager * SortCreate(FileManager fm, std::string sortFile);