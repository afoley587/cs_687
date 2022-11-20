#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "../FileManagerDll/FileManager.h"
#include "../MapDll/MapManager.h"

class MapFunctor {
private:
	FileManager fm;
	MapManager* mm;
	std::vector<std::string> infiles;
	std::string tempDir;
public:
	MapFunctor(FileManager _fm, MapManager* _mm, std::vector<std::string> _infiles, std::string _tempDir) :
		fm(_fm),
		mm{ _mm },
		infiles{ _infiles },
		tempDir{ _tempDir } {};

	void operator()() {
		std::vector<std::string> buff;
		std::stringstream new_tf;
		std::stringstream threadid;
		threadid << std::this_thread::get_id();
		new_tf << tempDir << "\\M" << threadid.str();
		mm->setTempFile(new_tf.str());

		for (auto f : infiles) {
			std::cout << "[MAP FUNCT] - Thread " << threadid.str() << " Reading " << f << std::endl;
			fm.read_file(f, buff);

			for (int i = 0; i < buff.size(); i++) {
				bool isLast = (i == buff.size() - 1);
				mm->map(buff[i], isLast);
			}
			std::cout << "[MAP FUNCT] - Thread " << threadid.str() << " Finished " << f << std::endl;
		}
	}
};