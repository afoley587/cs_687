#pragma once
#include <functional>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>

class ThreadPool
{
public:
	void Init(int _numTasks = 0);
	void AddJob(const std::function<void()>& task);
	void StopAccepting();
	void StartAccepting();
	void Flush();
	void Shutdown();
	bool HasTasks();
	ThreadPool& operator = (ThreadPool&) = delete;

private:
	void EventLoop();
	void FlushTasks();
	void FlushThreads();
	bool kill;
	bool isAccepting;
	unsigned int numTasks;
	std::mutex mut;
	std::condition_variable cv;
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;
};

