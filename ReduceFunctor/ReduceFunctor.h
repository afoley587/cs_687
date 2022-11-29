#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <thread>


#include "../FileManagerDll/FileManager.h"
#include "../ReduceDll/ReduceManager.h"

class ReduceFunctor {
private:
	ReduceManager* rm;
	ThreadSafeMap<std::string, std::vector<int>>* reduceMap;
	std::map<std::string, std::vector<int>> sortedMap;

public:

	ReduceFunctor(ReduceManager* _rm, ThreadSafeMap<std::string, std::vector<int>>* _reduceMap, std::map<std::string, std::vector<int>> _sortedMap) :
		rm{ _rm },
		reduceMap{_reduceMap},
		sortedMap{ _sortedMap }
	{};

	void operator()() {
		std::vector<std::string> buff;
		std::stringstream new_tf;
		std::stringstream threadid;

		threadid << std::this_thread::get_id();

		for (auto element : sortedMap)
		{
			auto reduceElement = rm->reduceForMap(element.second);
			std::vector<int> reduceElementVec{ reduceElement };
			reduceMap->insert(element.first, reduceElementVec, false);
		}
		
		return;
	}
};