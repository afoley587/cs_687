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

#include "StubInfo.h"

#pragma comment (lib, "ws2_32.lib")
using namespace std;

class StubLedger {
public:
	StubLedger();

	void RemoveStub(int socket);
	void AddStub(int socket);

	StubInfo GetStub(int socket);
	
	vector<StubInfo> GetStubInfoByStatus(StubStatus status);

	int GetNumWaitingStubs();

	void SetStubStatus(int socket, StubStatus status);

	void MarkDeadStubs();

	//void WaitForStubsToBeComplete(std::promise<bool>& AllStubsComplete, StubStatus status);

	std::map<int, StubInfo> stubs;
private:
};