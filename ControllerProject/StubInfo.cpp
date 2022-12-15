#include "StubInfo.h"

StubInfo::StubInfo() 
{

}


StubInfo::StubInfo(SOCKET incomingSocket)
{
	socket = incomingSocket;
	lastHeartBeat = 0;
}