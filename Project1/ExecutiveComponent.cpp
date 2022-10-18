#include <filesystem>
#include "ExecutiveComponent.h"
#include "WorkFlowComponent.h"
#include "FileManager.h"

#include <iostream>
#include <string>
#include <filesystem>

std::string GetDefaultWorkingDirectory(char currentExePath[]);

ExecutiveComponent::ExecutiveComponent(int argCount, char* args[]) 
	: workFlowComponent{WorkFlowComponent(ParseArgs(argCount, args))} 
{
	if (!ValidateArgs(argCount, args)) {
		std::cout << "\n" << "Invalid Args Provided. Supply Proper Arguements to Program.";
		return;
	}

}

void ExecutiveComponent::RunProgram() {
	//This is where the program starts after validation and object creation
	workFlowComponent.StartWorkFlow();
}

bool ExecutiveComponent::ValidateArgs(int argCount, char* args[]) {
	//TODO Add Validation to ensure Working Directory is set to default or one given is assigned


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

