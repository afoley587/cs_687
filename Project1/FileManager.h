#pragma once

#include <iostream>
#include <string>
#include <vector>

class FileManager {
public:
	void read_file(std::string filename, std::vector<std::string>& data);
	int write_file(std::string filename, std::vector<std::string> const data);
	void read_directory(std::string directory);
};