#include "MapManager.h"

#include <algorithm>
#include <iterator>
#include <functional>
#include <sstream>
#include <iostream>

void MapManager::map(std::string filename, std::string line) {
	std::vector<std::string> buffer;
	tokenize(line, buffer);
	mexport(filename, buffer);
}

void MapManager::mexport(std::string filename, std::vector<std::string> buffer) {
	if (filebuffer.find(filename) != filebuffer.end()) {
		filebuffer[filename].insert(filebuffer[filename].end(), buffer.begin(), buffer.end());
	}
	else {
		filebuffer.insert(std::pair<std::string, std::vector<std::string>>(filename, buffer));
	}

	if (filebuffer[filename].size() >= max_buffer_size) {
		std::cout << "DUMPING" << std::endl;
	}
}

void MapManager::tokenize(std::string in, std::vector<std::string>& out) {
	std::string nopunc;
	std::remove_copy_if(
		in.begin(), in.end(),
		std::back_inserter(nopunc),
		std::ptr_fun<int, int>(&std::ispunct)
	);

	std::istringstream iss{ nopunc };

	std::copy(
		std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter(out)
	);
}