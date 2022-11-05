#pragma once

struct ProgramSettings {
	std::string WorkingDirectory;
	std::string InputDirectory;
	std::string TempDirectory;
	std::string OutputDirectory;
	std::string MapDllPath;
	std::string ReduceDllPath;

	//const std::string sortInputFile = "tmpFile.txt";
	//const std::string resultsFile = "resultsFile.txt";
	//const std::string finalSuccessFile = "finalSuccessFile.txt";
};
