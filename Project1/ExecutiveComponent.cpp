#include <filesystem>
#include "ExecutiveComponent.h"
#include "WorkFlowComponent.h"
#include "FileManager.h"

#include <iostream>
#include <string>
#include <filesystem>

std::vector<std::string> ConvertArgsToStringVector(char* a[], int size);
std::string GetDefaultWorkingDirectory(std::string workDirectoryString);

ExecutiveComponent::ExecutiveComponent(int argCount, char* args[]) {
	programSettings = ParseArgs(argCount, args);
	fileManager = FileManager();
	workFlowComponent = WorkFlowComponent(programSettings, fileManager);
}


void ExecutiveComponent::RunProgram() {
	//This is where the program starts after validation and object creation
	workFlowComponent.StartWorkFlow();
}

ProgramSettings ExecutiveComponent::ParseArgs(int argCount, char* args[]) {
	//convert character array to string vector
	std::vector<std::string> argVector = ConvertArgsToStringVector(args, argCount);
	
	//TODO Parse Args to extract Program Settings
	ProgramSettings programSettings;

	if (!ValidateArgs(argVector, programSettings)) {
		std::cout << "\n" << "Invalid Args Provided. Supply Proper Arguements to Program.";
		throw std::invalid_argument("Invalid Arguements Provided!");
	}

	return programSettings;
}


bool ExecutiveComponent::ValidateArgs(void) {
	//TODO Add Validation to ensure Working Directory is set to default or one given is assigned

	FileManager fm{ programSettings.WorkingDirectory }; // this will crash if working dir doesnt exist

	/*
	if (!fm.directory_exists(programSettings.WorkingDirectory)) {
		std::cerr << "Input directory doesnt exist! Please provide it!" << std::endl;
		return false;
	}*/

	if (!prompt_for_dir(fm, programSettings.WorkingDirectory + "\\" + programSettings.ResultsFile) || !prompt_for_dir(fm, programSettings.WorkingDirectory + "\\" + programSettings.FinalOutputFile)) {
		std::cerr << "Unable to create some directory. Please see above." << std::endl;
		return false;
	}


	return true;
}

bool ExecutiveComponent::ValidateFile(std::string filePath) {
	//Does File Exist
	if (!fileManager.file_exists(filePath)) {
		return false;
	}


	//Does File Have the correct Extenstion
	return fileManager.validate_file_extension(filePath, ".txt");
}


std::string GetDefaultWorkingDirectory(std::string workDirectoryString)
{
	// Search for the substring in string
	// TODO Remove this when we are done testing this using Visual Studio
	std::string toErase = "\\x64\\Debug\\Project1.exe";
	size_t pos = workDirectoryString.find(toErase);
	if (pos != std::string::npos)
	{
		// If found then erase it from string
		&workDirectoryString.erase(pos, toErase.length());
	}

	return workDirectoryString;
}

std::vector<std::string> ConvertArgsToStringVector(char* a[], int size)
{
	std::vector<std::string> argVector;

	for (int i = 0; i < size; i++) {
		argVector.push_back(a[i]);
	}

	return argVector;
}


bool ExecutiveComponent::prompt_for_dir(FileManager fm, std::string dirname) {
	if (!fm.directory_exists(dirname)) {
		char yes_no;
		std::cout << dirname + " does not exists. Would you like to create it (y/n)?" << std::endl;
		std::cin >> yes_no;

		if (yes_no == 'y' || yes_no == 'Y') {
			return fm.mkdir(dirname);
		}
		else {
			std::cout << "Please create directory and run again!" << std::endl;
			return false;
		}
	}
	return true;

}