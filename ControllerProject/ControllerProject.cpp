// ControllerProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "StubInfo.h"

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <future>
#include <stack>

#include "StubLedger.h"

#include <stdlib.h>

#include "../FileManagerDll/FileManager.h"


#pragma comment (lib, "ws2_32.lib")

//#include <future>         // std::promise, std::future

using namespace std;

enum WorkFlowState { initializing, waiting_map, ready_sort, waiting_sort, ready_reduce, done };

void ListenOnPort(int portno);
void multipleConnectionCode(int socketNum);
void LogBuffer(char buffer[], SOCKET socket);
void EchoMessage(char buffer[], SOCKET socket);
vector<vector<string>> BatchFiles(string inputdir, int batchSize);
vector<vector<string>> ChunkFiles(vector<string> files, int numberOfChunks);

void parseMessageResultsFromSort(string sortedMessage);
std::vector<std::string> split(std::string input, char delimiter = ';');
void outputMapToFile(std::map<std::string, int> results);

map<int, stringstream> continueReadingClients;

StubLedger stubLedger;
WorkFlowState DetermineWorkFlowState();

WorkFlowState currentWorkFlowState = initializing;
WorkFlowState lastState = currentWorkFlowState;

std::time_t startTime;
vector<string> sortedMessagesFromClient;
map<string, int> sortedResults;

int main()
{
	startTime = std::time(nullptr);
	multipleConnectionCode(54540);
}

void multipleConnectionCode(int socketNum)
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(socketNum);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	fd_set master;
	FD_ZERO(&master);

	FD_SET(listening, &master);

	bool running = true;
	string stub_msg = "_m";
	string msg_delim{ ";" };

	vector<vector<string>> files_to_process;
	int last_batch_processed = 0;
	int total_batches = files_to_process.size();
	stringstream files_to_process_joined;

	cout << "Using Port - " << socketNum << " - " << listening << endl;
	cout << "Listening for Clients on Socket - " << listening << endl;

	while (running)
	{
		//reducing = last_batch_processed >= total_batches;
		fd_set fdcopy = master;

		// See who's talking to us
		int socketCount = select(0, &fdcopy, nullptr, nullptr, nullptr);

		// Loop through all the current connections / potential connect
		for (int i = 0; i < socketCount; i++)
		{
			// Makes things easy for us doing this assignment
			SOCKET sock = fdcopy.fd_array[i];

			// Is it an inbound communication?
			if (sock == listening)
			{
				// Accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &master);

				// Send a welcome message to the connected client
				string welcomeMsg = "Welcome to the Map Reduce Server!\r\n";
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
				Sleep(500);

				//TODO Add SocketInfo to SocketLedger
				stubLedger.AddStub(client);


				// We've finished doing all of the map input files
				// and can swich to reduce
			}
			else // It's an inbound message
			{
				char buf[70000];
				ZeroMemory(buf, 70000);

				// Receive message
				int bytesIn = recv(sock, buf, 70000, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					stubLedger.RemoveStub(sock);
					closesocket(sock);
					FD_CLR(sock, &master);
					cout << "Closing Connection with SOCKET: " << sock << endl;
				}
				else
				{
					// Check to see if it's a command. \quit kills the server
					if (buf[0] == '\\')
					{
						// Is the command quit? 
						string cmd = string(buf, bytesIn);
						if (cmd == "\\quit")
						{
							//running = false;
							break;
						}

						// Unknown command
						continue;
					}

					string cmd = string(buf, bytesIn);
					//TODO Check for HeartBeat Message
					if (cmd.find("HB:") != std::string::npos || cmd.find("DONE_M:") != std::string::npos || cmd.find("DONE_S:") != std::string::npos || continueReadingClients.find(sock) != continueReadingClients.end())
					{
						
						string data;

						if (cmd.find("DONE_M:") != std::string::npos)
						{
							stubLedger.SetStubStatus(sock, dead);
							if (stubLedger.GetStubInfoByStatus(mapping).size() < 1) 
							{
								currentWorkFlowState = ready_sort;
							}
						}

						if (cmd.find("DONE_S:") != std::string::npos || continueReadingClients.find(sock) != continueReadingClients.end())
						{
							//If Char is not last element signifier keep reading the s
							auto lastChar = cmd.back();
							data = cmd.substr(7);

							if (lastChar != '\0') 
							{
								//Keep reading from this client
								continueReadingClients[sock] << data;
							}
							else 
							{
								continueReadingClients[sock] << data;

								stubLedger.SetStubStatus(sock, dead);
								//Ingest Sort Message

								parseMessageResultsFromSort(continueReadingClients[sock].str());

								if (stubLedger.GetStubInfoByStatus(sorting).size() < 1)
								{
									currentWorkFlowState = ready_reduce;
								}
							}


						}

						if (cmd.find("HB:") != std::string::npos)
						{
							stubLedger.SetStubStatus(sock, waiting);
						}
					}
				}

				stubLedger.MarkDeadStubs();
				LogBuffer(buf, sock);
			}
		}

		//TODO Move some logic here to handle workflow of map and reduce 

		//Give Server 5 seconds to establish Connection with all stubs before sending commands
		time_t dateTimeMillis = time(nullptr);
		if (dateTimeMillis - startTime > 15)
		{

			if (currentWorkFlowState == initializing)
			{
				stringstream files_to_process_joined;
				int last_batch_processed = 0;
				vector<StubInfo> waitingStubs = stubLedger.GetStubInfoByStatus(waiting);
				int numberOfWaitingProcesses = waitingStubs.size();
				FileManager fm;
				vector<string> input_files;
				fm.read_directory("C:\\Users\\kenne\\OneDrive\\Masters_Comp_Sci\\CSE_687_OOD\\Project1\\cs_687\\shakespeare", input_files);
				files_to_process = ChunkFiles(input_files,
					numberOfWaitingProcesses/*programSettings..., numMappers...*/);

				for (auto fileBatch : files_to_process) {
					copy(
						files_to_process[last_batch_processed].begin(),
						files_to_process[last_batch_processed].end(),
						ostream_iterator<string>(files_to_process_joined, msg_delim.c_str())
					);

					stub_msg = "_m;" + files_to_process_joined.str();

					//Send a start Reduce Message to each waiting stub
					send(waitingStubs[last_batch_processed].socket, stub_msg.c_str(), stub_msg.size() + 1, 0);
					stubLedger.SetStubStatus(waitingStubs[last_batch_processed].socket, mapping);
					last_batch_processed++;
				}

				currentWorkFlowState = waiting_map;
				////inject stubLedger with Promise
				//stubLedger.WaitForStubsToBeComplete(AllMapCompletePromise, reducing);
				//currentWorkFlowState = map_state;
			}

			//Set a wait here for all reduce to be done 
			if (currentWorkFlowState == ready_sort) {
				//If All Reduce messages are done Start doing Reduce

				vector<StubInfo> waitingStubs = stubLedger.GetStubInfoByStatus(waiting);
				int numberOfWaitingProcesses = waitingStubs.size();

				//split up the number of batches for each Waiting Stub
				if (numberOfWaitingProcesses >= 1)
				{


					FileManager fm;
					vector<string> input_files;
					fm.read_directory("C:\\Users\\kenne\\OneDrive\\Masters_Comp_Sci\\CSE_687_OOD\\Project1\\cs_687\\temp", input_files);
					auto BatchOfFilesToReduce = ChunkFiles(input_files,
						numberOfWaitingProcesses/*programSettings..., numMappers...*/);

					/*auto BatchOfFilesToReduce = BatchFiles("C:\\Users\\kenne\\OneDrive\\Masters_Comp_Sci\\CSE_687_OOD\\Project1\\cs_687\\temp", numberOfWaitingProcesses);*/

					stringstream files_to_reduce_joined;
					//vector<StubInfo> waitingStubs = stubLedger.GetStubInfoByStatus(waiting);
					int reduceBatchIndex = 0;


					//TODO Split files by delimiter 
					for (auto fileBatch : BatchOfFilesToReduce) {
						copy(
							BatchOfFilesToReduce[reduceBatchIndex].begin(),
							BatchOfFilesToReduce[reduceBatchIndex].end(),
							ostream_iterator<string>(files_to_reduce_joined, msg_delim.c_str())
						);

						stub_msg = "_r;" + files_to_reduce_joined.str();


						send(waitingStubs[reduceBatchIndex].socket, stub_msg.c_str(), stub_msg.size() + 1, 0);
						stubLedger.SetStubStatus(waitingStubs[reduceBatchIndex].socket, sorting);
						reduceBatchIndex++;
					}

					currentWorkFlowState = waiting_sort;
				}
			}

			if (currentWorkFlowState == ready_reduce)
			{
				outputMapToFile(sortedResults);
				currentWorkFlowState = done;
			}

		}
	}

	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(listening, &master);
	closesocket(listening);

	// Message to let users know what's happening.
	string msg = "Server is shutting down. Goodbye\r\n";

	while (master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = master.fd_array[0];

		// Send the goodbye message
		send(sock, msg.c_str(), msg.size() + 1, 0);

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();

	system("pause");
}

//WorkFlowState DetermineWorkFlowState()
//{
//	WorkFlowState result = currentWorkFlowState;
//	for (const auto& kvp : stubLedger.stubs)
//	{
//		if (kvp.second.status == mapping || kvp.second.status == reducing)
//		{
//			return waiting_on_stubs;
//		}
//	}
//
//	if (currentWorkFlowState == waiting_on_stubs)
//	{
//		switch (lastState) {
//		case initializing:
//			result = sort_state;
//			break;
//		case sort_state:
//			result = reduce_state;
//			break;
//		}
//	}
//
//	lastState = result;
//	return result;
//}

void EchoMessage(char buffer[], SOCKET socket)
{
	ostringstream ss;
	ss << "[ECHO FROM SERVER] " << "SOCKET #" << socket << ": " << buffer << "\r\n";
	string strOut = ss.str();
	cout << strOut;
	send(socket, strOut.c_str(), strOut.size() + 1, 0);
}

void LogBuffer(char buffer[], SOCKET socket)
{
	ostringstream ss;
	ss << "SOCKET #" << socket << ": " << buffer << "\r\n";
	string strOut = ss.str();
	cout << strOut;
}

vector<vector<string>> BatchFiles(string inputdir, int batchSize) {
	FileManager fm;
	vector<string> input_files;
	fm.read_directory(inputdir, input_files);
	vector<vector<string>> batches;
	for (int i = 0; i < batchSize
		; ++i) {
		vector<string> batch;
		batch.resize(batchSize);
		auto start_iter = next(input_files.begin(), i * batchSize);
		auto end_iter = input_files.end();
		if (i * batchSize + batchSize < input_files.size()) {
			end_iter = next(input_files.begin(), i * batchSize + batchSize);
			batch.resize(batchSize);
		}
		copy(start_iter, end_iter, batch.begin());
		batches.push_back(batch);
	}
	return batches;
}

void parseMessageResultsFromSort(string sortedMessage)
{

	//TODO Parse KVP Pairs
	vector<string> pasedKVPElement = split(sortedMessage, '|');


	//TODO split Keys From Pairs 
	for (auto kvpString : pasedKVPElement)
	{
		vector<string> parsedKeyAndValue = split(kvpString, '_');

		if (parsedKeyAndValue.size() > 1) {
			string key = parsedKeyAndValue[0];
			int value = stoi(parsedKeyAndValue[1]);

			//Insert into collected SortMap
			//if sortedResults has key 
			if (!(sortedResults.count(key) > 0))
			{
				sortedResults[key] = value;
			}
			else
			{
				int currentVal = sortedResults[key];
				sortedResults[key] = currentVal + value;
			}
		}
	}


}

std::vector<std::string> split(std::string input, char delimiter) {
	std::vector<std::string> ret;
	std::string tok;
	std::stringstream ss(input);


	while (getline(ss, tok, delimiter)) {
		ret.push_back(tok);
	}

	return ret;
}


void outputMapToFile(std::map<std::string, int> results) {
	FileManager fm;
	std::ostringstream formattedKvpElement;

	for (auto element : results)
	{
		formattedKvpElement << "KEY: " << element.first << " , Value: " << element.second << std::endl;
	}

	fm.append_file("C:\\Users\\kenne\\OneDrive\\Masters_Comp_Sci\\CSE_687_OOD\\Project1\\cs_687\\out\\results.txt", std::vector<std::string> { formattedKvpElement.str()});
}

vector<vector<string>> ChunkFiles(vector<string> files, int numberOfChunks)
{
	vector<vector<std::string>> result;
	vector<string> iteratorVector;


	int mapCount = files.size();
	int index = 1;
	int sliceIndex = 1;
	int bucketCount = 0;

	for (auto element : files)
	{
		iteratorVector.push_back(element);
		if (index >= mapCount)
		{
			result.push_back(iteratorVector);
			break;
		}

		if ((index % (files.size() / numberOfChunks) == 0) && (bucketCount < (numberOfChunks - 1)))
		{
			result.push_back(iteratorVector);
			iteratorVector.clear();
			bucketCount++;
		}

		index++;
	};

	return result;
}