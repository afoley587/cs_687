#include "ThreadPool.h"

#define TEST_TP 1
#ifdef TEST_TP

#include <iostream>
#include <string>
#include <sstream>
#endif

void ThreadPool::Init() {
	isAccepting = true;
	numTasks    = std::thread::hardware_concurrency();
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
			task();
			cv.notify_one();
		}
		if (kill) { return; };
	}
}

void ThreadPool::AddJob(const std::function<void()>& task) {
	if (!isAccepting) { return; };

	std::unique_lock<std::mutex> lock(mut);
	tasks.push(task);
	lock.unlock();
	cv.notify_one();
	std::cout << "Job Added" << std::endl;
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

void ThreadPool::Shutdown() {
	std::unique_lock<std::mutex> lock(mut);
	kill = true;
	lock.unlock();
	cv.notify_all();
	for (std::thread& t : threads) { 
		std::cout << "Joining " << std::endl;
		t.join(); 
	};
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
	tp.Shutdown();
}
#endif