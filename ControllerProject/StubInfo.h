#pragma once
#include <iostream>
#include <vector>
#include <map>

#include <chrono>

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdlib.h>

#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

enum StubStatus { none, waiting, mapping, sorting, dead, sending };

class StubInfo {
public:
	StubInfo();
	StubInfo(SOCKET incomingSocket);

	SOCKET socket;
	long lastHeartBeat = 0;
	StubStatus status = waiting;
};
