#pragma once
#include "../MessageProtocolLibrary/MessagingClient.h"
#include "../ThreadPool/ThreadPool.h"

class Stub : public MessagingClient
{
public:
	Stub() { 
		tp.Init(1); 
	};
	~Stub() {
		tp.Flush();
	}
	void dispatch_map(std::vector<std::string> files_to_map, std::string tempdir);
	stringstream dispatch_reduce(std::vector<std::string> files_to_reduce);
private:
	ThreadPool tp;
	int _num_threads{ 1 };
};

