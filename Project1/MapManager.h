#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include   "FileManager.h"

class MapManager {
public:
	MapManager() :max_buffer_size{ 10 }, filebuffer{}, fm{} {};
	MapManager(int b) : max_buffer_size{ b }, filebuffer{}, fm{} {};

	/* Reads a line of text, tokenizes it, and puts it into a readable format
	* Args:
	*	filename: Filename (used as key)
	*	line: Line from above file
	*	forceExport: To force an export
	* Returns
	*	...
	*/
	void map(std::string filename, std::string line, bool forceExport = false);

	/* Exports a buffer to disk
	* Args:
	*	key: Key to export
	*	buffer: buffer of tokens
	*	forceExport: To force an export
	* Returns
	*	...
	*/
	void mexport(std::string key, std::vector<std::string> buffer, bool forceExport = false);

	/*  Tokenizes a line from a strong to a vector
	* Args:
	*	in: line of text
	*	out: Vector of tokens
	* Returns
	*	...
	*/
	void tokenize(std::string in, std::vector<std::string>& out);

private:
	int max_buffer_size; /* Max Number Of Words In A Buffer Vector */
	std::unordered_map<std::string, std::vector<std::string>> filebuffer;
	FileManager fm;
};