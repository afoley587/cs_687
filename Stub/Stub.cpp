// Stub.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <signal.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include "../MessageProtocolLibrary/Models.h"

// #include <windows.h>

#include "Stub.h"
// #include "../utils/utils.h"
#include "../MapFunctor/MapFunctor.h"
#include "../MapDll/MapManager.h"
#include "../FileManagerDll/FileManager.h"

// Global so cleanup can call shutdown
Stub stub{};

std::vector<std::string> split(std::string input, char delimiter);
void cleanup(int signum);

void Stub::dispatch_map(std::vector<std::string> files_to_map, std::string tempdir = "C:\\Users\\alexa\\Source\\Repos\\project-2\\tmp") {
	std::cout << "[STUB] - Dispatching Map" << std::endl;
	// replace with other stuff
	MapManager* mm = new MapManager{};
 	tp.AddTask(MapFunctor(FileManager{}, mm, files_to_map, tempdir));
}

void Stub::dispatch_reduce() {
	std::cout << "[STUB] - Dispatching Reduce" << std::endl;
}

int main()
{
	// HINSTANCE mapdll = LoadDll("MapManager.dll");
	// HINSTANCE reddll = LoadDll ("ReduceManager.dll");

	signal(SIGINT, cleanup);
	while (true) {
		std::cout << "[STUB] - Waiting For Controller" << std::endl;
		std::promise<Serialized> commandEnumPromise;
		auto commandEnumFuture = commandEnumPromise.get_future();
		stub.ConnectToServer(commandEnumPromise);
		Serialized commandFromServer = commandEnumFuture.get();
		std::cout << "[STUB] - Received From Controller" << std::endl;

		switch (commandFromServer.action)
		{

		case startEnum::Reduce:

			stub.dispatch_reduce();
			break;
			//Start Reduce Operations
		case startEnum::Map:
			std::vector<std::string> files = split(commandFromServer.data, ';');

			std::cout << "DEBUG" << std::endl;
			for (auto t : files) {
				std::cout << t << std::endl;
			}
			stub.dispatch_map(files);
			break;
			//Start Map Operations
		}

		string testString = "Done with Process";

		stub.SendString(testString);
	}
}

std::vector<std::string> split(std::string input, char delimiter = ';') {
	std::vector<std::string> ret;
	std::string tok;
	std::stringstream ss(input);


	while (getline(ss, tok, delimiter)) {
		ret.push_back(tok);
	}

	return ret;
}

void cleanup(int signum) {
	cout << "[STUB] - Catching Exit" << endl;
	bool clean = stub.ShutDownConnection();
	if (clean) {
		cout << "[STUB] - Connection Shut" << endl;
	}
	else {
		cerr << "[STUB] - Socket not cleanly closed" << endl;
	}
	exit(EXIT_SUCCESS);
}