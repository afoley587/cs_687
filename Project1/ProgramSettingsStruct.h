#pragma once

struct ProgramSettings {
	std::string WorkingDirectory;
	std::string InputDirectory;
	std::string TempDirectory;
	std::string OutputDirectory;
	std::string MapDllPath;
	std::string ReduceDllPath;
	int NumMappers{ 5 };
	int NumReducers{ 5 };
	int NumSorters{ 5 };
	int NumBuckets{ 5 };

	ProgramSettings() {};

	ProgramSettings(std::string wd, std::string id, std::string td, std::string  od, std::string md, std::string rd) {
		WorkingDirectory = wd;
		InputDirectory = id;
		TempDirectory = td;
		OutputDirectory = od;
		MapDllPath = md;
		ReduceDllPath = rd;
	};

	ProgramSettings(const ProgramSettings& ps) :
		WorkingDirectory{ ps.WorkingDirectory },
		InputDirectory{ ps.InputDirectory },
		TempDirectory{ ps.TempDirectory },
		OutputDirectory{ ps.OutputDirectory },
		MapDllPath{ ps.MapDllPath },
		ReduceDllPath{ ps.ReduceDllPath } 
	{
		std::cout << "[PROG SET] - COPY CTOR" << std::endl;
	};

	ProgramSettings(ProgramSettings&& ps) :
		WorkingDirectory{ ps.WorkingDirectory },
		InputDirectory{ ps.InputDirectory },
		TempDirectory{ ps.TempDirectory },
		OutputDirectory{ ps.OutputDirectory },
		MapDllPath{ ps.MapDllPath },
		ReduceDllPath{ ps.ReduceDllPath } 
	{
		std::cout << "[PROG SET] - MOVE CTOR" << std::endl;
	};

	ProgramSettings& operator=(const ProgramSettings& ps) {
		std::cout << "[PROG SET] - COPY" << std::endl;
		WorkingDirectory = ps.WorkingDirectory;
		InputDirectory = ps.InputDirectory;
		TempDirectory = ps.TempDirectory;
		OutputDirectory = ps.OutputDirectory;
		MapDllPath = ps.MapDllPath;
		ReduceDllPath = ps.ReduceDllPath;
		std::cout << "[PROG SET] - THEIRS " << ps.InputDirectory << std::endl;
		std::cout << "[PROG SET] - MINE " << InputDirectory << std::endl;
		return *this;
	}

	ProgramSettings& operator=(ProgramSettings&& ps) {
		std::cout << "[PROG SET] - MOVE" << std::endl;
		WorkingDirectory = ps.WorkingDirectory;
		InputDirectory = ps.InputDirectory;
		TempDirectory = ps.TempDirectory;
		OutputDirectory = ps.OutputDirectory;
		MapDllPath = ps.MapDllPath;
		ReduceDllPath = ps.ReduceDllPath;
		std::cout << "[PROG SET] - THEIRS " << ps.InputDirectory << std::endl;
		std::cout << "[PROG SET] - MINE " << InputDirectory << std::endl;
		
		return *this;
	}

	//const std::string sortInputFile = "tmpFile.txt";
	//const std::string resultsFile = "resultsFile.txt";
	//const std::string finalSuccessFile = "finalSuccessFile.txt";
};
