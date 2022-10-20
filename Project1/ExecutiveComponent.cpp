#include <filesystem>
#include "ExecutiveComponent.h"
#include "WorkFlowComponent.h"
#include "FileManager.h"

#include <iostream>
#include <string>
#include <filesystem>

std::string GetDefaultWorkingDirectory(char currentExePath[]);

ExecutiveComponent::ExecutiveComponent(int argCount, char* args[])
	: programSettings{ ProgramSettings(ParseArgs(argCount, args)) } {};

void ExecutiveComponent::RunProgram() {
	//This is where the program starts after validation and object creation
	if (!ValidateArgs()) {
		std::cout << "\n" << "Invalid Args Provided. Supply Proper Arguements to Program.";
		return;
	}

	WorkFlowComponent workFlowComponent{ programSettings };
	workFlowComponent.StartWorkFlow();
}

bool ExecutiveComponent::ValidateArgs(void) {
	//TODO Add Validation to ensure Working Directory is set to default or one given is assigned

	FileManager fm{ programSettings.WorkingDirectory }; // this will crash if worrking dir doesnt exist

	if (!fm.directory_exists(programSettings.WorkingDirectory)) {
		return false;
	}

	if (!prompt_for_dir(fm, programSettings.ResultsFile) || !prompt_for_dir(fm, programSettings.FinalOutputFile)) {
		std::cerr << "Unable to create some directory. Please see above." << std::endl;
		return false;
	}


	return true;
}

ProgramSettings ExecutiveComponent::ParseArgs(int argCount, char* args[]) {
	//TODO Parse Args to extract Program Settings
	workingDirectory = GetDefaultWorkingDirectory(args[0]);

	ProgramSettings tempProgramSettings =
	{
		workingDirectory,
		"TestSortInput.txt",
		"resultsTextFile.txt",
		"FinalResultsFile.txt"
	};

	return tempProgramSettings;
}

std::string GetDefaultWorkingDirectory(char currentExePath[])
{
	std::string currentExePathString = currentExePath;
	// Search for the substring in string
	std::string toErase = "\\x64\\Debug\\Project1.exe";
	size_t pos = currentExePathString.find(toErase);
	if (pos != std::string::npos)
	{
		// If found then erase it from string
		&currentExePathString.erase(pos, toErase.length());
	}

	return currentExePathString + "\\" + "Program_Output";
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