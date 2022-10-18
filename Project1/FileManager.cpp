#include "FileManager.h"

#include <fstream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1
#include <experimental/filesystem>


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
		std::cerr << "Ffile Does Not Exists!" << std::endl;
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

void FileManager::test_output(std::string textToOutput) {
	std::cout << "\n" << textToOutput;
}


/*
void FileManager::read_directory(std::string directory) {

	for (const auto& file : std::filesystem::directory_iterator(directory)) {
		std::cout << file.path() << std::endl;
	}
}
*/
