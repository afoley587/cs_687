#include "StubLedger.h"

StubLedger::StubLedger()
{
	stubs = std::map<int, StubInfo>();
};

void StubLedger::RemoveStub(int socket)
{
	stubs.erase(socket);
};

void StubLedger::AddStub(int socket)
{
	StubInfo tempStub = StubInfo(socket);
	stubs.emplace(socket, tempStub);
};

StubInfo StubLedger::GetStub(int socket)
{
	return stubs[socket];
}

vector<StubInfo> StubLedger::GetStubInfoByStatus(StubStatus status)
{
	vector<StubInfo> results = vector<StubInfo>();
	for (auto stub : stubs)
	{
		if (stub.second.status == status)
		{
			results.push_back(stub.second);
		}
	}

	return results;
}

int StubLedger::GetNumWaitingStubs()
{
	int numWaitingStubs = 0;
	for (auto kvp : stubs)
	{
		if (kvp.second.status == waiting)
		{
			numWaitingStubs++;
		}
	}

	return numWaitingStubs;
}

void StubLedger::SetStubStatus(int socket, StubStatus status)
{
	stubs[socket].status = status;
}

void StubLedger::MarkDeadStubs()
{
	//TODO Implement
	//std::time_t dateTimeMillis = std::time(nullptr);

	//for (auto kvp : stubs)
	//{
	//	//Time since Last Heartbeat was provided by the client
	//	auto timeSinceLastHeartBeat = dateTimeMillis - kvp.second.lastHeartBeat;
	//	//If its been more than 10 seconds since last heartbeat change the status
	//	if (timeSinceLastHeartBeat > 10000)
	//	{
	//		stubs[kvp.first].status = dead;
	//	}
	//}
}

//void StubLedger::WaitForStubsToBeComplete(std::promise<bool>& AllStubsComplete, StubStatus status)
//{
//	bool waitingForStatusToComplete = true;
//	while (waitingForStatusToComplete)
//	{
//		waitingForStatusToComplete = false;
//		for (auto kvp : stubs) {
//
//			if (kvp.second.status == status)
//			{
//				waitingForStatusToComplete = true;
//				break;
//			}
//		}
//
//		Sleep(2000);
//	}
//
//	AllStubsComplete.set_value(true);
//}