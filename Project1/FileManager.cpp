#include "FileManager.h"

#include <fstream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1
#include <experimental/filesystem>
#include <filesystem>
#include <string>


bool FileManager::file_exists(std::string filename) {

	return std::experimental::filesystem::exists(filename);
}

bool FileManager::directory_exists(std::string dirname) {

	return file_exists(dirname);
}

void FileManager::read_directory(std::string dirname, std::vector<std::string>& files) {

	if(!directory_exists(dirname)) {
		std::cerr << "Directory does not exist!" << std::endl;
	}
	std::experimental::filesystem::path top_dir{ dirname };

	for (auto const& dir_entry : std::experimental::filesystem::recursive_directory_iterator{ top_dir }) {
		files.push_back(dir_entry.path().string());
	}
}

void FileManager::read_file(std::string filename, std::vector<std::string>& data) {

	if (!file_exists(filename)) {
		std::cerr << "File Does Not Exists!" << std::endl;
	}

	std::ifstream ifile{ filename };

	if (!ifile) {
		std::cerr << "Could not open file!\n";
	}

	std::string line;

	while (getline(ifile, line)) {
		data.push_back(line);
	}

	ifile.close();
}

int FileManager::write_file(std::string filename, std::vector<std::string> const data) {

	std::ofstream ofile(filename);

	if (!ofile) {
		std::cerr << "Could not open file!\n";
		std::cerr << filename;
	}

	for (auto s : data) {
		ofile << s;
	}

	ofile.close();

	return data.size();
}

void FileManager::append_file(std::string filename, std::vector<std::string> const data) {
	std::fstream fileStream;
	
	fileStream.open(filename, std::ios::app);

	if (fileStream.is_open()) {
		for (auto dataline : data) {
			fileStream << "\n" << dataline << std::endl;
		}
	}

	fileStream.close();
}


void FileManager::test_output(std::string textToOutput) {
	std::cout << "\n" << textToOutput;
}

void FileManager::reset_output_files() {
	for (const auto& entry : std::filesystem::directory_iterator(workingDirectory)) {
		std::string pathString = entry.path().string();
		bool isSortInputFile = pathString.find("TestSortInput") != std::string::npos;

		if (!isSortInputFile) {
			std::fstream fileStream;
			fileStream.open(entry.path(), std::ofstream::trunc);
			fileStream.close();
		}
	}
	
	std::string testString = "Test";
}

/*
void FileManager::read_directory(std::string directory) {

	for (const auto& file : std::filesystem::directory_iterator(directory)) {
		std::cout << file.path() << std::endl;
	}
}
*/
