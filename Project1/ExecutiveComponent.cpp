#include <filesystem>
#include "ExecutiveComponent.h"
#include "WorkFlowComponent.h"
#include "FileManager.h"

#include <iostream>
#include <string>
#include <filesystem>

std::string GetDefaultWorkingDirectory(std::string& currentExePath);

ExecutiveComponent::ExecutiveComponent(int argCount, char* args[]) : 
	fileManager(fileManager), workFlowComponent(WorkFlowComponent(fileManager)) {
	

	if (!ValidateArgs(argCount, args)) {
		std::cout << "\n" << "Invalid Args Provided. Supply Proper Arguements to Program.";
		return;
	}

	//TODO Get working directory and input file from args and inject into FileManager
	ParseArgs(argCount, args);
	fileManager = FileManager("Test");

	std::string currentExePath = args[0];
	workingDirectory = GetDefaultWorkingDirectory(currentExePath);
	std::cout << "\n" << "Printing out Current Working Directory: ";
	std::cout << "\n" << workingDirectory;

	/*std::string currentExePath = args[0];
	std::cout << "\n" << args[0];
	std::string substringToRemoveFromPath = "\\x64\\Debug\\Project1.exe";
	eraseSubStr(currentExePath, substringToRemoveFromPath);
	std::cout << "\n" << currentExePath;*/
	//std::string workingDirectory = std::filesystem::current_path();
	//std::cout << "\n" << std::filesystem::current;
	//Inject File Manager into WorkFlow Component

}

bool ExecutiveComponent::ValidateArgs(int argCount, char* args[]) {
	//TODO Add Validation to ensure Working Directory is set to default or one given is assigned


	return true;
}

void ExecutiveComponent::ParseArgs(int argCount, char* args[]) {

}

std::string GetDefaultWorkingDirectory(std::string& currentExePath)
{
	// Search for the substring in string
	std:string toErase = "\\x64\\Debug\\Project1.exe";
	size_t pos = currentExePath.find(toErase);
	if (pos != std::string::npos)
	{
		// If found then erase it from string
		currentExePath.erase(pos, toErase.length());
	}
	return currentExePath;
}

