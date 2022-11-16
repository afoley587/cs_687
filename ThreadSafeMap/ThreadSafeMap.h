#pragma once
#include <mutex>
#include <unordered_map>

template <typename T, typename U>
class ThreadSafeMap
{
public:
	ThreadSafeMap() {};
	ThreadSafeMap& operator = (ThreadSafeMap&) = delete;

	void insert(T key, U val) {
		std::lock_guard<std::mutex> lock(mut);
		data.insert(std::pair<T, U>(key, val));
		cv.notify_one();
	}

	U get(T key) {
		std::unique_lock<std::mutex> lock(mut);
		cv.wait(lock, [this, key] { return this->data.find(key) != this->data.end(); });
		// std::unordered_map<T, U>::iterator found = data.find(key);
		U val = data[key];
		lock.unlock();
		return val;
	}

private:
	std::mutex mut;
	std::unordered_map<T, U> data;
	std::condition_variable cv;
};

