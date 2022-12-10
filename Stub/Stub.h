#pragma once
#include "../MessageProtocolLibrary/MessagingClient.h"
#include "../ThreadPool/ThreadPool.h"

class Stub : public MessagingClient
{
public:
	Stub() { 
		tp.Init(_num_threads); 
	};
	~Stub() {
		tp.Flush();
	}
	void dispatch_map();
	void dispatch_reduce();
private:
	ThreadPool tp;
	int _num_threads{ 5 };
};

