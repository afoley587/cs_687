#include "pch.h"

#include <algorithm>
#include <iterator>
#include <functional>
#include <sstream>
#include <iostream>
#include <cwctype>

#include "MapManager.h"

MapManager::MapManager() {}

void MapManager::map(std::string line, bool forceExport) {
	std::vector<std::string> buffer;
	tokenize(line, buffer);
	mexport(buffer, forceExport);
}

void MapManager::mexport(std::vector<std::string> buffer, bool forceExport) {
	if (filebuffer.find(tempFile) != filebuffer.end()) {
		filebuffer[tempFile].insert(filebuffer[tempFile].end(), buffer.begin(), buffer.end());
	}
	else {
		fm.touch_file(tempFile);
 		filebuffer.insert(std::pair<std::string, std::vector<std::string>>(tempFile, buffer));
	}

	if (filebuffer[tempFile].size() >= max_buffer_size || forceExport) {

		std::vector<std::string> toExport;

		for (auto s : filebuffer[tempFile]) {
			toExport.push_back("(\"" + s + "\", [1]),");
		}

		
		try {
			std::cout << "[MAP MGR] - Dumping " << max_buffer_size << " to tempfile " << tempFile << std::endl;
			fm.append_file(tempFile, toExport);
		}
		catch (std::invalid_argument) {
			std::cerr << "[MAP MGR] - Could Not Empty Buffer" << std::endl;
			throw std::runtime_error("[MAP MGR] - Could Not Empty Buffer");
		}
		
		filebuffer[tempFile].clear();
	}
}

void MapManager::tokenize(std::string in, std::vector<std::string>& out) {
	std::string nopunc;

	// Remove all punct/special chars


	for (int i = 0; i < in.length(); i++) {
		if (!std::iswpunct(in[i])) {
			nopunc += in[i];
		}
		else {
			// some places where word--another
			if (in[i] == '-') {
				nopunc += " ";
			}
		}
	}
	/*
	std::remove_copy_if(
		in.begin(), in.end(),
		std::back_inserter(nopunc),
		[](unsigned char c) {return std::iswpunct(c); }
	);
	*/
	//std::string transformOutput;

	//transform(nopunc.begin(), nopunc.end(), transformOutput.begin(), std::tolower);

	std::for_each(nopunc.begin(), nopunc.end(), [](char& c) {
		c = ::tolower(c);
		});

	std::istringstream iss{ nopunc };

	// Remove Spaces
	std::copy(
		std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter(out)
	);

}

MAPDLL_API MapManager * Create(FileManager fm, int bufflen = 1024, std::string tempfile = "") {
	std::cout << "[MAP DLL] - Creating instance with " << bufflen << " and " << tempfile << std::endl;
	return new MapManager(fm, bufflen, tempfile);
}