// Stub.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <signal.h>
#include <iostream>
// #include <windows.h>

#include "Stub.h"
// #include "../utils/utils.h"
#include "../MapFunctor/MapFunctor.h"
#include "../MapDll/MapManager.h"
#include "../FileManagerDll/FileManager.h"

// Global so cleanup can call shutdown
Stub stub{};

void cleanup(int signum);

void Stub::dispatch_map() {
	std::cout << "[STUB] - Dispatching Map" << std::endl;
	// replace with other stuff
	std::vector<std::string> tempfiles{ "C:\\Users\\alexa\\Source\\Repos\\project-2\\shakespeare\\TamingOfTheShrew.txt" };
	std::string tempdir = "C:\\Users\\alexa\\Source\\Repos\\project-2\\tmp";
	MapManager* mm = new MapManager{};
 	tp.AddTask(MapFunctor(FileManager{}, mm, tempfiles, tempdir));
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
		std::promise<startEnum> commandEnumPromise;
		auto commandEnumFuture = commandEnumPromise.get_future();
		stub.ConnectToServer(commandEnumPromise);
		startEnum commandFromServer = commandEnumFuture.get();
		std::cout << "[STUB] - Received From Controller" << std::endl;

		switch (commandFromServer)
		{

		case startEnum::Reduce:
			stub.dispatch_reduce();
			break;
			//Start Reduce Operations
		case startEnum::Map:
			stub.dispatch_map();
			break;
			//Start Map Operations
		}

		string testString = "Done with Process";

		stub.SendString(testString);
	}
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