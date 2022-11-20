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

	std::vector<std::string> tsmbuff;
	// std::unique_lock<std::mutex> lock(mut);
	std::string bucketTempFile{ tempFile + "_R" + std::to_string(lastBucket)};
	lastBucket = (lastBucket + 1) % numBuckets;
	// lock.unlock();

	if (tsm.has(bucketTempFile)) {
		tsmbuff = tsm.get(bucketTempFile);
		tsmbuff.insert(tsmbuff.end(), buffer.begin(), buffer.end());
		tsm.insert(bucketTempFile, tsmbuff);
	}
	else {
		tsmbuff = buffer;
		fm.touch_file(bucketTempFile);
 		tsm.insert(bucketTempFile, tsmbuff);
	}

	if (tsmbuff.size() >= max_buffer_size || forceExport) {

		std::vector<std::string> toExport;

		for (auto s : tsmbuff) {
			toExport.push_back("(\"" + s + "\", [1]),");
		}
		
		try {
			// std::cout << "[MAP MGR] - Dumping " << max_buffer_size << " to tempfile " << bucketTempFile << std::endl;
			fm.append_file(bucketTempFile, toExport);
		}
		catch (std::invalid_argument) {
			std::cerr << "[MAP MGR] - Could Not Empty Buffer" << std::endl;
			throw std::runtime_error("[MAP MGR] - Could Not Empty Buffer");
		}
		
		tsmbuff.clear();
		tsm.insert(bucketTempFile, tsmbuff);
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

MAPDLL_API MapManager * MapCreate(FileManager fm, int bufflen = 1024, std::string tempfile = "") {
	std::cout << "[MAP DLL] - Creating instance with " << bufflen << " and " << tempfile << std::endl;
	return new MapManager(fm, bufflen, tempfile);
}