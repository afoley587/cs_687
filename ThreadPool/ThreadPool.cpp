#include "ThreadPool.h"
#include <iostream>
// #define TEST_TP 1
#ifdef TEST_TP
#include <string>
#include <sstream>
#endif

void ThreadPool::Init(int _numTasks) {
	isAccepting = true;
	kill = false;
	if (_numTasks < 1) {
		numTasks = std::thread::hardware_concurrency();
	}
	else {
		numTasks = _numTasks;
	}
	threads.resize(numTasks);
	for (int i = 0; i < numTasks; i++) {
		threads.at(i) = std::thread(&ThreadPool::EventLoop, this);
	}
}

void ThreadPool::EventLoop() {
	while (true) {
		std::function<void()> task;
		std::unique_lock<std::mutex> lock(mut);
		cv.wait(lock, [this] {
			return !tasks.empty() || kill;
			});

		if (!tasks.empty()) {
			task = tasks.front();
			tasks.pop();
			lock.unlock();
			cv.notify_one();
			task();
		}
		if (kill) {
			return;
		};
	}
}

void ThreadPool::AddTask(const std::function<void()>& task) {
	AddJob(task);
}

void ThreadPool::AddJob(const std::function<void()>& task) {
	if (!isAccepting) { return; };
	std::unique_lock<std::mutex> lock(mut);
	tasks.push(task);
	lock.unlock();
	cv.notify_one();
}

bool ThreadPool::HasTasks() {
	bool hasTasks;
	std::unique_lock<std::mutex> lock(mut);
	hasTasks = !tasks.empty();
	lock.unlock();
	return hasTasks;
}

void ThreadPool::StartAccepting() {
	isAccepting = true;
}

void ThreadPool::StopAccepting() {
	isAccepting = false;
}

void ThreadPool::FlushTasks() {
	StopAccepting();
	std::unique_lock<std::mutex> lock(mut);
	cv.wait(lock, [this] {
		return tasks.empty();
		});
	lock.unlock();
}

void ThreadPool::FlushThreads() {
	std::unique_lock<std::mutex> lock(mut);
	kill = true;
	lock.unlock();
	cv.notify_all();
	for (std::thread& t : threads) {
		t.join();
	};
}

void ThreadPool::Flush() {
	FlushTasks();
	FlushThreads();
}
void ThreadPool::Shutdown() {
	Flush();
	threads.clear();
}

#ifdef TEST_TP
int main() {
	ThreadPool tp;
	tp.Init();
	int tester{ 5 };
	std::cout << "Iniitialized" << std::endl;

	for (int i = 0; i < 10; i++) {
		tp.AddJob([tester](void) {
			std::stringstream tid;
			tid << std::this_thread::get_id();
			std::cout << "Hello From " << tid.str() << " tester = " << tester << std::endl;
			});
	}
	tp.FlushTasks();
	// tp.Shutdown();
}
#endif