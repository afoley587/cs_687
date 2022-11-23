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
	//std::promise <std::map<std::string, std::vector<int>>>& promise;
	
public:
	static ThreadSafeMap<std::string, std::vector<int>> sortMap;
	SortFunctor(FileManager _fm, SortManager* _rm, std::vector<std::string> _infiles) :
		fm(_fm),
		sm{ _rm },
		infiles{ _infiles } {};

	//SortFunctor() {};
	//SortFunctor(FileManager _fm, SortManager* _sm, std::vector<std::string> _infiles, std::promise<std::map<std::string, std::vector<int>>> & _promise) 
	//{
	//	fm = _fm;
	//	sm = _sm;
	//	infiles = _infiles;
	//	promise = _promise;
	//}

	void operator()() {
		std::vector<std::string> buff;
		std::stringstream new_tf;
		std::stringstream threadid;
		threadid << std::this_thread::get_id();
		//std::map<std::string, std::vector<int>> resultMap = sortMap;

		for (auto inputFile : infiles) {
			try
			{
				std::cout << "[SORT FUNCT] - Thread " << threadid.str() << " Reading " << inputFile << std::endl;


				//TODO Add arg to take in a file path
				auto tempMap = sm->SortInput(inputFile);



				//std::cout << "[SORT FUNCT] - Thread " << threadid.str() << " Reading " << f << std::endl;
				//fm.read_file(f, buff);

				//for (int i = 0; i < buff.size(); i++) {
				//	bool isLast = (i == buff.size() - 1);
				//	rm->map(buff[i], isLast);
				//}
				std::cout << "[SORT FUNCT] - Thread " << threadid.str() << " Finished " << inputFile << std::endl;
				//buff.clear();

				//for (auto kvp : tempMap) 
				//{
				//	//while (it != m_DriverList.end())
				//	//{
				//	//	if (it->second == this)
				//	//	{
				//	//		m_DriverList.erase(it);
				//	//		it = m_DriverList.begin();
				//	//	}
				//	//	else
				//	//	{
				//	//		++it;
				//	//	}
				//	//}
				//	sortMap.insert(kvp);
				//}


				//for (auto kvp : tempMap) 
				//{
				//	if (sortMap.contains(kvp.first)) 
				//	{
				//		sortMap[kvp.first].insert(sortMap[kvp.first].end(), kvp.second.begin(), kvp.second.end());
				//	}
				//	else 
				//	{
				//		sortMap[kvp.first] = kvp.second;
				//	}
				//}

				for (auto& kvp : tempMap) 
				{
					
					sortMap.insert(kvp.first, kvp.second, true);
				}

				std::cout << &sortMap << std::endl;
				//sortMap.insert(tempMap.begin(), tempMap.end());
			}
			catch (std::exception exception)
			{
				auto str = exception;
			}
		}
		std::string testString = "Kenny";
		//promise.set_value(resultMap);
	}
};