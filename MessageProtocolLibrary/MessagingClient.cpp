#include "pch.h"

#include "MessagingClient.h"
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <windows.h>
#include <thread>
#include <sstream>


#pragma comment(lib, "ws2_32.lib")

using namespace std;

bool MessagingClient::SendString(const std::string& str, byte terminator)
{
	size_t bytesSent, bytesRemaining = str.size();
	const char* pBuf = &(*str.begin());
	while (bytesRemaining > 0)
	{
		bytesSent = ::send(sendingSocket, pBuf, bytesRemaining, 0);
		if (bytesSent == INVALID_SOCKET || bytesSent == 0)
			return false;
		bytesRemaining -= bytesSent;
		pBuf += bytesSent;
	}
	::send(sendingSocket, &terminator, 1, 0);
	return true;
}

bool MessagingClient::InitializeWinSock() 
{
	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return false;
	}
}

bool MessagingClient::ConnectToServer(std::promise<Serialized>& executionPromise)
{
	string ipAddress = MESSAGING_IP_ADDRESS;			// IP Address of the server
	int port = MESSAGING_PORT;						// Listening port # on the server

	// Initialize WinSock
	InitializeWinSock();

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return NULL;
	}

	// Fill in a hint structure

	//sockaddr_in hint;
	//hint.sin_family = AF_INET;
	//hint.sin_port = htons(54000);
	//hint.sin_addr.S_un.S_addr = INADDR_ANY;
	string ipString = "localhost";
	const char* pTemp = ipString.c_str();
	auto sPort = htons(54000);
	//auto iResult = getaddrinfo(pTemp, sPort.c_str(), &hints, &result);
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(MESSAGING_PORT);
	inet_pton(AF_INET, MESSAGING_IP_ADDRESS.c_str(), &hint.sin_addr);
	int numTries = 0;
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));

	//if (connResult == SOCKET_ERROR) {

	//	// Connect to server
	//	do {
	//		connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	//		numTries++;
	//		Sleep(2000);

	//	} while (numTries < 40 || (connResult != SOCKET_ERROR));
	//}


	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return false;
	}

	sendingSocket = sock;
	// Do-while loop to send and receive data
	char buf[4096];
	string userInput;


	bool sendHeartbeats = true;
	std::thread t1(
		[&]()
		{
			while (sendHeartbeats) {
				std::this_thread::sleep_for(std::chrono::seconds(5));
				SendHeartBeat(sendingSocket);
			}
		} // 50 messages 100 millisec apart
	);

	do
	{
		//process incoming messages
		Serialized msg;
		char readBuf[4096];
		ZeroMemory(buf, 4096);
		int bytesRecieved = recv(sendingSocket, readBuf, 4096, 0);
		if (bytesRecieved > 0)
		{
			ostringstream ss;
			ss << "MessageFromServer -" << readBuf << "\r\n";
			string strOut = ss.str();
			cout << strOut;

			if (readBuf[0] == '_')
			{
				startEnum commandEnum = static_cast<startEnum>(readBuf[1]);
				msg.action = commandEnum;
				msg.data = "";

				for (int i = 3; i < sizeof(readBuf) / sizeof(char); i++) {
					if (readBuf[i] == '\0') {
						break;
					}
					msg.data += readBuf[i];
				}
					

				executionPromise.set_value(msg);
				sendHeartbeats = false;
			}
		}

	} while (sendHeartbeats);

	t1.join();

	return true;
}
bool MessagingClient::ShutDownConnection() 
{
	// Gracefully close down everything
	closesocket(sendingSocket);
	WSACleanup();
	return true;
}


void MessagingClient::SendHeartBeat(SOCKET socket) {
	string heartBeatText = "HB:";
	int sendResult = send(socket, heartBeatText.c_str(), heartBeatText.size() + 1, 0);
}