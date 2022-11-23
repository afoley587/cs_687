#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <thread>


#include "../FileManagerDll/FileManager.h"
#include "../ReduceDll/ReduceManager.h"

class ReduceFunctor {
private:
	FileManager fm;
	ReduceManager* rm;
	std::vector<std::string> infiles;

public:

	ReduceFunctor(FileManager _fm, ReduceManager* _rm, std::vector<std::string> _infiles) :
		fm(_fm),
		rm{ _rm },
		infiles{ _infiles } {};

	void operator()() {
		std::vector<std::string> buff;
		std::stringstream new_tf;
		std::stringstream threadid;
		threadid << std::this_thread::get_id();

		for (auto f : infiles) {
			//std::cout << "[SORT FUNCT] - Thread " << threadid.str() << " Reading " << f << std::endl;
			//fm.read_file(f, buff);

			//for (int i = 0; i < buff.size(); i++) {
			//	bool isLast = (i == buff.size() - 1);
			//	rm->map(buff[i], isLast);
			//}
			//std::cout << "[MAP FUNCT] - Thread " << threadid.str() << " Finished " << f << std::endl;
			//buff.clear();
		}
	}
};