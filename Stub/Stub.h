#pragma once
#include "../MessageProtocolLibrary/MessagingClient.h"
#include "../ThreadPool/ThreadPool.h"

class Stub : public MessagingClient
{
public:
	Stub() { 
		tp.Init(_num_threads); 
	};
	void dispatch_map();
private:
	void dispatch_reduce();
	ThreadPool tp;
	int _num_threads{ 5 };
};

