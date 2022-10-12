#include "FileManager.h"

#include <fstream>
#include <filesystem>

void FileManager::read_file(std::string filename, std::vector<std::string>& data) {
	
	std::ifstream ifile(filename);

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
	}

	for (auto s : data) {
		ofile << s;
	}

	ofile.close();

	return data.size();

}

/*
void FileManager::read_directory(std::string directory) {

	for (const auto& file : std::filesystem::directory_iterator(directory)) {
		std::cout << file.path() << std::endl;
	}
}
*/
