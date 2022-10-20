#include "MapManager.h"

#include <algorithm>
#include <iterator>
#include <functional>
#include <sstream>
#include <iostream>
#include <cwctype>

void MapManager::map(std::string filename, std::string line, bool forceExport) {
	std::vector<std::string> buffer;
	tokenize(line, buffer);
	mexport(filename, buffer, forceExport);
}

void MapManager::mexport(std::string filename, std::vector<std::string> buffer, bool forceExport) {
	if (filebuffer.find(filename) != filebuffer.end()) {
		filebuffer[filename].insert(filebuffer[filename].end(), buffer.begin(), buffer.end());
	}
	else {
		fm.touch_file(filename);
		filebuffer.insert(std::pair<std::string, std::vector<std::string>>(filename, buffer));
	}

	if (filebuffer[filename].size() >= max_buffer_size || forceExport) {

		std::vector<std::string> toExport; 
		
		for (auto s : filebuffer[filename]) {
			toExport.push_back("(\"" + s + "\", [1]),");
		}

		// toExport.push_back("\nEXPORT\n");

		fm.append_file(filename, toExport);
		filebuffer[filename].clear();
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