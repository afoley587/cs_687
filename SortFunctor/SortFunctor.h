#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <thread>


#include "../FileManagerDll/FileManager.h"
#include "../ReduceDll/SortManager.h"

class SortFunctor {
private:
	FileManager fm;
	SortManager* sm;
	std::vector<std::string> infiles;
	ThreadSafeMap<std::string, std::vector<int>> * sortMap;
public:

	SortFunctor(FileManager _fm, SortManager* _rm, std::vector<std::string> _infiles, ThreadSafeMap<std::string, std::vector<int>> * _sortMap) :
		fm(_fm),
		sm{ _rm },
		infiles{ _infiles } {};

	void operator()() {
		std::vector<std::string> buff;
		std::stringstream new_tf;
		std::stringstream threadid;
		threadid << std::this_thread::get_id();

		for (auto inputFile : infiles) {
			try
			{
				std::cout << "[SORT FUNCT] - Thread " << threadid.str() << " Reading " << inputFile << std::endl;

				auto tempMap = sm->SortInput(inputFile);
				std::cout << "[SORT FUNCT] - Thread " << threadid.str() << " Finished " << inputFile << std::endl;

				for (auto& kvp : tempMap) 
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
			catch (std::exception exception)
			{
				auto str = exception;
			}
		}
	}
};