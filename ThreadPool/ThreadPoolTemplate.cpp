#include "ThreadPoolTemplate.h"
#include <iostream>
// #define TEST_TP 1
#ifdef TEST_TP
#include <string>
#include <sstream>
#endif

template<class T>
void ThreadPoolTemplate<T>::Init(int _numTasks) {
	isAccepting = true;

	if (_numTasks < 1) {
		numTasks = std::thread::hardware_concurrency();
	}
	else {
		numTasks = _numTasks;
	}
	threads.resize(numTasks);
	for (int i = 0; i < numTasks; i++) {
		threads.at(i) = std::thread(&ThreadPoolTemplate<T>::EventLoop, this);
	}

	kill = false;
}

template<class T>
void ThreadPoolTemplate<T>::EventLoop() {
	while (true) {
		std::function<void()> task;
		std::unique_lock<std::mutex> lock(mut);
		cv.wait(lock, [this] {
			return !tasks.empty() || kill;
			});

		if (!tasks.empty()) {
			task = std::move(tasks.front());
			tasks.pop();
			lock.unlock();
			task();
			cv.notify_one();
		}
		if (kill) {
			return;
		};
	}
}

template<class T>
void ThreadPoolTemplate<T>::ExecuteFunc(const std::function<void()>& task)
{
	std::promise<bool> promise = std::promise<bool>();
	std::future<bool> future = promise.get_future();
	futures.push_back(std::move(future));
	task();
	promise.set_value(true);
}

template<class T>
void ThreadPoolTemplate<T>::AddJob(const std::function<T()>& task) {
	if (!isAccepting) { return; };
	std::unique_lock<std::mutex> lock(mut);
	tasks.push(task);
	lock.unlock();
	cv.notify_one();
}

template<class T>
bool ThreadPoolTemplate<T>::HasTasks() {
	bool hasTasks;
	std::unique_lock<std::mutex> lock(mut);
	hasTasks = !tasks.empty();
	lock.unlock();
	return hasTasks;
}

template<class T>
void ThreadPoolTemplate<T>::WaitUntilCompleted() {
	for (std::thread& t : threads) {
		t.join();
	};

	std::unique_lock<std::mutex> lock(mut);

	for (int i = 0; i < futures.size(); i++)
	{
		std::future<bool> future = std::move(futures.at(i));

		future.get();
	}
}

template<class T>
void ThreadPoolTemplate<T>::StartAccepting() {
	isAccepting = true;
}

template<class T>
void ThreadPoolTemplate<T>::StopAccepting() {
	isAccepting = false;
}

template<class T>
void ThreadPoolTemplate<T>::FlushTasks() {
	StopAccepting();
	std::unique_lock<std::mutex> lock(mut);
	cv.wait(lock, [this] {
		return tasks.empty();
		});
	lock.unlock();
}

template<class T>
void ThreadPoolTemplate<T>::FlushThreads() {
	std::unique_lock<std::mutex> lock(mut);
	kill = true;
	lock.unlock();
	cv.notify_all();
	for (std::thread& t : threads) {
		t.join();
	};
}

template<class T>
void ThreadPoolTemplate<T>::Flush() {
	FlushTasks();
	FlushThreads();
}

template<class T>
void ThreadPoolTemplate<T>::Shutdown() {
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