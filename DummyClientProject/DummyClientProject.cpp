// DummyClientProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include "..\MessageProtocolLibrary\MessagingClient.h"
#pragma comment(lib, "ws2_32.lib")


//#include <boost/array.hpp>
//#include <boost/asio.hpp>
//using boost::asio::ip::tcp;
using namespace std;

int main()
{
	MessagingClient messageClient = MessagingClient();
	std::promise<startEnum> commandEnumPromise;
	auto commandEnumFuture = commandEnumPromise.get_future();
	messageClient.ConnectToServer(commandEnumPromise);
	startEnum commandFromServer = commandEnumFuture.get();

	//Entry Point for STUB Logic to start Reduce and Map
	switch (commandFromServer) 
	{

		case startEnum::Reduce:
			break;
			//Start Reduce Operations
		case startEnum::Map:
			break;
			//Start Map Operations
	}

	//When Done With Process Send Completion Message to Server

	string testString = "Done with Process";
	
	messageClient.SendString(testString);
	messageClient.ShutDownConnection();

}