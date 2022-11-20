#pragma once

#include <unordered_map>

template <typename T, typename U>
class SingleThreadedMap
{
public:
	SingleThreadedMap() {};
	SingleThreadedMap& operator = (SingleThreadedMap&) = delete;

	void insert(T key, U val) {
		if (has(key)) {
			data[key] = val;
		}
		else {
			data.insert(std::pair<T, U>(key, val));
		}
	}

	bool has(T key) {
		return data.find(key) != data.end();
	}

	void clear(T key) {
		data[key] = U{};
	}

	U get(T key) {
		U val = data[key];
		return val;
	}

private:
	std::unordered_map<T, U> data;
};

