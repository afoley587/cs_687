#pragma once
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <map>


template <typename T, typename U>
class ThreadSafeMap
{
public:
	ThreadSafeMap() {};
	ThreadSafeMap& operator = (ThreadSafeMap&) = delete;

	void insert(T key, U val) {
		std::lock_guard<std::mutex> lock(write_mut);
		if (has(key)) {
			data[key] = val;
		}
		else {
			data.insert(std::pair<T, U>(key, val));
		}

		
		// cv.notify_one();
	}

	bool has(T key) {
		return data.find(key) != data.end();
	}

	void clear(T key) {
		std::lock_guard<std::mutex> lock(write_mut);
		data[key] = U{};
	}

	U get(T key) {
		std::shared_lock<std::shared_mutex> lock(read_mut);
		// cv.wait(lock, [this, key] { return this->data.find(key) != this->data.end(); });
		// if (!has(key)) { return U{}; };

		U val = data[key];

		lock.unlock();
		return val;
	}

	ThreadSafeMap(const ThreadSafeMap& other)
	{

		data = other.data;
	}

	//ThreadSafeMap(ThreadSafeMap&& other)
	//{
	//}

private:
	std::mutex write_mut;
	std::shared_mutex read_mut;
	std::unordered_map<T, U> data;
	std::condition_variable cv;
};

