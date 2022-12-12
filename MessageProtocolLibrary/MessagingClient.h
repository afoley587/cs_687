#pragma once

#include <future>
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include<windows.h>
#include "Models.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#ifdef MESSAGING_API_EXPORTS
#define MESSAGING_API __declspec(dllexport)
#else
#define MESSAGING_API __declspec(dllimport)
#endif

class MESSAGING_API MessagingClient {
	private:
		bool taskComplete = false;
		const int MESSAGING_PORT = 54000;
		const string MESSAGING_IP_ADDRESS = "127.0.0.1";
		void SendHeartBeat(SOCKET socket);
		bool InitializeWinSock();
		//SOCKET CreateSocket(sockaddr_in &hint);
		SOCKET sendingSocket;
	public:
		using byte = char;
		bool SendString(const std::string& str, byte terminator = '\0');
		MessagingClient() {};
		~MessagingClient() { ShutDownConnection(); };
		bool ConnectToServer(std::promise<Serialized>& executionPromise);
		bool ShutDownConnection();

};