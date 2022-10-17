#pragma once

#include <iostream>
#include <string>
#include <vector>

class FileManager {
public:
	FileManager(std::string workingDir) :workingDirectory{ workingDir } {};

	/* Checks if the filename given exists
	* Args:
	*	filename: Filename to check
	* Returns
	*	true if exists, false otherwise
	*/
	bool file_exists(std::string filename);

	/* Checks if the directory given exists
	* Args:
	*	dirname: Directory to check
	* Returns
	*	true if exists, false otherwise
	*/
	bool directory_exists(std::string dirname);

	/* Iterates a directory and finds all files
	* Args:
	*	dirname: Directory to check
	*	files: Buffer to place files in
	* Returns
	*	...
	*/
	void read_directory(std::string dirname, std::vector<std::string>& files);

	/* Reads a file and places its contents in a buffer
	* Args:
	*	filename: Directory to check
	*	data: Buffer to place raw data in
	* Returns
	*	...
	*/
	void read_file(std::string filename, std::vector<std::string>& data);

	/* Writes a file from the data given in the buffer contents
	* Args:
	*	filename: Filename to write
	*	data: data to write
	* Returns
	*	...
	*/
	int write_file(std::string filename, std::vector<std::string> const data);
	void read_directory(std::string directory);

	void test_output(std::string textToOutput);

private:
	std::string workingDirectory;
};