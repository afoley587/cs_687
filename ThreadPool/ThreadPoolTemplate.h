#pragma once
#include <functional>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>
#include <future>

template<typename T>
class ThreadPoolTemplate
{
public:
	ThreadPoolTemplate<T>& operator = (ThreadPoolTemplate&) = delete;
	ThreadPoolTemplate<T>(const ThreadPoolTemplate& source) { };
	ThreadPoolTemplate() {};

	void Init(int _numTasks = 0);
	void AddJob(const std::function<T()>& task);
	void StopAccepting();
	void StartAccepting();
	void Flush();
	void Shutdown();
	bool HasTasks();
	bool IsExecuting = false;
	void WaitUntilCompleted();

private:
	void ExecuteFunc(const std::function<void()>& task);
	void EventLoop();
	void FlushTasks();
	void FlushThreads();
	bool kill;
	bool isAccepting;
	unsigned int numTasks;
	std::mutex mut;
	std::condition_variable cv;
	std::vector<std::thread> threads;
	std::queue<std::function<T()>> tasks;
	int numExecuting = 0;
	std::vector<std::future<bool>> futures;


	std::atomic<int> njobs_pending;
	std::mutex main_mutex;
	std::condition_variable main_condition;
};

