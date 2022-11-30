#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "FileManager.h"
#include "../ThreadSafeMap/ThreadSafeMap.h"
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
	std::map<std::string, std::vector<int>> SortInputForPaths(std::vector<std::string> filePaths);
	std::map<std::string, std::vector<int>> SortInput(std::string filePath);
	void SortInput_withMap(std::string filePath, std::unordered_map<std::string, std::vector<int>>& mapToUse);

	void setInputFile(std::string in) { sortInputDirectory = in; };
	std::string getInputFile(void) { return sortInputDirectory; };

	std::map<int, std::map< std::string, std::vector<int>>> ChunkMapForReduce(std::map<std::string, std::vector<int >> threadSafeMap, int chunkSize, int chunkIndex);

private:
	FileManager fileManager;
	std::string sortInputDirectory;

};

extern "C" SORTDLL_API SortManager * SortCreate(FileManager fm, std::string sortFile);