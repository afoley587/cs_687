// ControllerProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include "../FileManagerDll/FileManager.h"

#pragma comment (lib, "ws2_32.lib")

using namespace std;
void ListenOnPort(int portno);
void multipleConnectionCode(int socketNum);
void LogBuffer(char buffer[], SOCKET socket);
void EchoMessage(char buffer[], SOCKET socket);
vector<vector<string>> BatchFiles(string inputdir, int batchSize );

int main()
{
	multipleConnectionCode(54000);
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

	vector<vector<string>> files_to_process = BatchFiles("C:\\Users\\alexa\\Source\\Repos\\project-2\\shakespeare", 3/*programSettings..., numMappers...*/);
	int last_batch_processed = 0;
	int total_batches = files_to_process.size();
	stringstream files_to_process_joined;

	while (running)
	{

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
				if (last_batch_processed >= total_batches) {
					stub_msg = "_r";
				}
				else {
					copy(
						files_to_process[last_batch_processed].begin(),
						files_to_process[last_batch_processed].end(),
						ostream_iterator<string>(files_to_process_joined, msg_delim.c_str())
					);

					stub_msg = "_m;" + files_to_process_joined.str();
				}


				// Accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &master);

				// Send a welcome message to the connected client
				string welcomeMsg = "Welcome to the Map Reduce Server!\r\n";
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
				Sleep(2000);
				// string startMsg = "_r";

				send(client, stub_msg.c_str(), stub_msg.size() + 1, 0);

				last_batch_processed++;
				// We've finished doing all of the map input files
				// and can swich to reduce
			}
			else // It's an inbound message
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
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
							running = false;
							break;
						}

						// Unknown command
						continue;
					}
				}

				LogBuffer(buf, sock);
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