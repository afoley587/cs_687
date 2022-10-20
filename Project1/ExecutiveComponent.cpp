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


bool ExecutiveComponent::ValidateArgs(std::vector<std::string> args, ProgramSettings &programSettings) {
	//TODO Add Validation to ensure Working Directory is set to default or one given is assigned

	std::string workingDirectory = GetDefaultWorkingDirectory(args.front());


	int argIndex = 0;
	// Ensure all files given exist with the correct file extention
	for (auto DirectoryArg : args) {
		// First Arg is used to create working directory
		if (DirectoryArg == args.front()) {
			programSettings.WorkingDirectory = workingDirectory;
		}
		// All other Args are file names
		else {
			bool useDefaultFile = false;
			std::string fullDirectoryPath = workingDirectory + "\\" + DirectoryArg;

			if (!fileManager.directory_exists(fullDirectoryPath)) {
				//TODO We may want to make the directory instead of return false
				return false;
			}

			if (argIndex == 1) 
			{
				programSettings.InputDirectory = workingDirectory + "\\" + DirectoryArg;
			}
			else if (argIndex == 2) {

				programSettings.OutputDirectory = workingDirectory + "\\" + DirectoryArg;
			}
			else if (argIndex == 3) {

				programSettings.TempDirectory = workingDirectory + "\\" + DirectoryArg;
			}
		}

		argIndex++;
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

