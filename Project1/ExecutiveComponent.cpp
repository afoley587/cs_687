#include <filesystem>
#include "ExecutiveComponent.h"
#include "WorkFlowComponent.h"
#include "FileManager.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <cwctype>

#define NUM_REQUIRED_ARGS 6

std::vector<std::string> ConvertArgsToStringVector(char* a[], int size);
std::string GetDefaultWorkingDirectory(std::string workDirectoryString);

ExecutiveComponent::ExecutiveComponent(int argCount, char* args[]) {
	try{
		programSettings = ParseArgs(argCount, args);
	}
	catch (std::invalid_argument) {
		PrintHelp();
		PrintHelp();
		throw std::invalid_argument("[EXEC COMP] - Unable to verify args!");
	}
	
	fileManager = FileManager();
	workFlowComponent = WorkFlowComponent(programSettings, fileManager);
}

void ExecutiveComponent::PrintHelp() {
	//This is where the program starts after validation and object creation
	const char* help = "\nmapreduce.exe. A tool to map, reduce, and group words.\n"
		"mapreduce.exe [inputdir] [tmpdir] [outputdir] [mapDll] [reduceDll]\n"
		"All passed positional arugemnts must be strings (or string convertible). Special symbols are not allowed.\n"
		"All passed positional arguments must be unique.\n";
	std::cout << help;
}

void ExecutiveComponent::RunProgram() {
	//This is where the program starts after validation and object creation
	HINSTANCE mapDll = LoadDll(programSettings.MapDllPath);
	MapManager* _mapManager =  MapFactory(mapDll);

	std::cout << "[EXEC COMP] - Loaded Dlls" << std::endl;
	workFlowComponent.StartWorkFlow();
}

ProgramSettings ExecutiveComponent::ParseArgs(int argCount, char* args[]) {
	//convert character array to string vector
	//std::vector<std::string> argVector = ConvertArgsToStringVector(args, argCount);
	//
	////TODO Parse Args to extract Program Settings
	//

	//if (!ValidateArgs(argVector)) {
	//	throw std::invalid_argument("Invalid Arguements Provided!");
	//}

	return ProgramSettings{ "", args[1], args[2], args[3], args[4], args[5] };
}


bool ExecutiveComponent::ValidateArgs(std::vector<std::string> argVector) {
	//TODO Add Validation to ensure Working Directory is set to default or one given is assigned

	if (argVector.size() < NUM_REQUIRED_ARGS) {
		return false;
	}

	// Add a check to make sure each dir is unique
	if (!fileManager.directory_exists(argVector[1])) {
		return false;
	}

	if (!fileManager.file_exists(argVector[4]) || !fileManager.file_exists(argVector[5])) {
		return false;
	}

	if (!prompt_for_dir(fileManager, argVector[2]) || !prompt_for_dir(fileManager, argVector[3])) {
		return false;
	}

	// All dirs should be unique


	if (!fileManager.are_unique(argVector)) {
		return false;
	};

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

	for (int i = 0; i < dirname.length(); i++) {
		if (dirname[i] == '.' || dirname[i] == '/' || dirname[i] == '\\' || dirname[i] == '_' || dirname[i] == '-' || dirname[i] == ':') {
			continue;
		}
		if (std::iswpunct(dirname[i])) {
			throw std::invalid_argument("[EXEC COMP] - Special Character In Dirname");
		}
	}

	if (!fm.directory_exists(dirname)) {
		char yes_no;
		std::cout << dirname + " does not exists. Would you like to create it (y/n)?" << std::endl;
		std::cin >> yes_no;

		if (yes_no == 'y' || yes_no == 'Y') {
			return fm.mkdir(dirname);
		}
		else {
			throw std::invalid_argument("[EXEC COMP] - Please Create Directory Manually");
		}
	}
	return true;

}

HINSTANCE ExecutiveComponent::LoadDll(std::string path) {

	std::wstring widepath;
	HINSTANCE dll;

	for (int i = 0; i < path.length(); ++i) {
		widepath += wchar_t(path[i]);
	}

	const wchar_t* dllLibName = widepath.c_str();

	dll = LoadLibraryEx(dllLibName, NULL, NULL);

	if (dll == NULL) {
		throw std::runtime_error("[EXEC COMP] - Unable to load DLL");
	}

	return dll;

}
