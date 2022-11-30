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
	ThreadSafeMap& operator = (const ThreadSafeMap& sourceMap) 
	{
		data = sourceMap.data;
		return *this;
	}

	void insert(T key, U val, bool extend = false) {
		std::lock_guard<std::mutex> lock(write_mut);
		if (has(key)) {
			if (extend) {
				data[key].insert(data[key].end(), val.begin(), val.end());
			}
			else {
				data[key] = val;
			}
		}
		else {
			data.insert(std::pair<T, U>(key, val));
		}
		cv.notify_one();
	}



	bool has(T key) {
		return data.find(key) != data.end();
	}

	void clear(T key) {
		std::lock_guard<std::mutex> lock(write_mut);
		data[key] = U{};
	}

	//void merge(std::map<t, u> map) 
	//{
	//
	//}

	U get(T key) {
		std::shared_lock<std::shared_mutex> lock(read_mut);
		// cv.wait(lock, [this, key] { return this->data.find(key) != this->data.end(); });
		// if (!has(key)) { return U{}; };

		U val = data[key];

		lock.unlock();
		return val;
	}

	std::map<T, U> getData() 
	{
		std::shared_lock<std::shared_mutex> lock(read_mut);
		//makes a copy for thread safe purposes
		std::map<T, U> map(data);
		return map;
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
	std::map<T, U> data;
	std::condition_variable cv;
};

