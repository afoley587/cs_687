#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class MapManager {
public:
	MapManager() :max_buffer_size{ 10 }, filebuffer{} {};
	void map(std::string filename, std::string line);
	void mexport(std::string key, std::vector<std::string> buffer);
	void tokenize(std::string in, std::vector<std::string>& out);

private:
	int max_buffer_size; /* Max Number Of Words In A Buffer Vector */
	std::unordered_map<std::string, std::vector<std::string>> filebuffer;
};