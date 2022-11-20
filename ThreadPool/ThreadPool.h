#pragma once
#include <functional>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>

class ThreadPool
{
public:
	void Init();
	void AddJob(const std::function<void()>& task);
	void StopAccepting();
	void StartAccepting();
	void FlushTasks();
	void FlushThreads();
	void Shutdown();
	bool HasTasks();
	ThreadPool& operator = (ThreadPool&) = delete;

private:
	void EventLoop();
	bool kill;
	bool isAccepting;
	int numTasks;
	std::mutex mut;
	std::condition_variable cv;
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;
};

