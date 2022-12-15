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
#include "../ReduceFunctor/ReduceFunctor.h"
#include "../SortFunctor/SortFunctor.h"
#include "../MapDll/MapManager.h"
#include "../ReduceDll/SortManager.h"
#include "../FileManagerDll/FileManager.h"

// Global so cleanup can call shutdown
Stub stub{};

std::vector<std::string> split(std::string input, char delimiter);
void cleanup(int signum);

void Stub::dispatch_map(std::vector<std::string> files_to_map, std::string tempdir = "C:\\Users\\kenne\\OneDrive\\Masters_Comp_Sci\\CSE_687_OOD\\Project1\\cs_687\\temp") {
	std::cout << "[STUB] - Dispatching Map" << std::endl;
	// replace with other stuff
	MapManager* mm = new MapManager{};
 	tp.AddTask(MapFunctor(FileManager{}, mm, files_to_map, tempdir));
	tp.Flush();
}

stringstream Stub::dispatch_reduce(std::vector<std::string> files_to_reduce) {
	tp.Init(1);
	std::cout << "[STUB] - Dispatching Reduce" << std::endl;
	//Todo Start Sort then Reduce
	SortManager* sm = new SortManager{};
	ThreadSafeMap<std::string, std::vector<int>> sortMap = ThreadSafeMap<std::string, std::vector<int>>();
	tp.AddTask(SortFunctor(FileManager{}, sm, files_to_reduce, &sortMap));
	//TODO Wait for Tasks
	tp.Flush();
	stringstream sorted_keys_values;
	string msg_delim = "|";

	for (const auto& kvp : sortMap.getData()) 
	{
		stringstream kvpStringStream;
		kvpStringStream << kvp.first << "_" << kvp.second.size();
		//std::cout << kvpStringStream.str() << std::endl;
		sorted_keys_values << kvpStringStream.str() << msg_delim.c_str();
	}

	return sorted_keys_values;
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

		std::vector<std::string> files = split(commandFromServer.data, ';');

		std::cout << "DEBUG" << std::endl;
		for (auto t : files) {
			std::cout << t << std::endl;
		}

		stringstream sortedValues;
		switch (commandFromServer.action)
		{
			
			//Start Map Operations
			case startEnum::Map:
				stub.dispatch_map(files);
				stub.SendString("DONE_M:");
				break;
			
			//Start Reduce Operations
			case startEnum::Reduce:
				sortedValues = stub.dispatch_reduce(files);
				stub.SendString("DONE_S:" + sortedValues.str());
				break;
		}

		stub.ShutDownConnection();
	}
}

void SendMessageOfCompletion() 
{
	stub.SendString("Done:");
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