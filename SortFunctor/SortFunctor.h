#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <thread>


#include "../FileManagerDll/FileManager.h"
#include "../ReduceDll/SortManager.h"
#include <queue>

class SortFunctor {
private:
	FileManager fm;
	SortManager* sm;
	std::vector<std::string> infiles;
	ThreadSafeMap<std::string, std::vector<int>>* sortMap;
public:

	SortFunctor(FileManager _fm, SortManager* _rm, std::vector<std::string> _infiles, ThreadSafeMap<std::string, std::vector<int>>* _sortMap) :
		fm(_fm),
		sm{ _rm },
		infiles{ _infiles },
		sortMap{ _sortMap } {};

	void operator()() {
		std::vector<std::string> buff;
		std::stringstream new_tf;
		std::stringstream threadid;
		threadid << std::this_thread::get_id();
		std::queue<std::map<std::string, std::vector<int>>> tempMaps;

		auto mapFromFile = sm->SortInputForPaths(infiles);

		for (auto& kvp : mapFromFile)
		{
			auto sourceVector = kvp.second;

			if (sortMap->has(kvp.first))
			{
				auto targetVector = sortMap->get(kvp.first);
				sourceVector.insert(sourceVector.end(), targetVector.begin(), targetVector.end());
			}

			sortMap->insert(kvp.first, sourceVector);
		}
	}
};