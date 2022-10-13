#pragma once

#include <iostream>
#include <string>
#include <vector>

class FileManager {
public:
	bool file_exists(std::string filename);
	bool directory_exists(std::string dirname);
	void read_directory(std::string dirname, std::vector<std::string>& files);
	void read_file(std::string filename, std::vector<std::string>& data);
	int write_file(std::string filename, std::vector<std::string> const data);
	void read_directory(std::string directory);
};