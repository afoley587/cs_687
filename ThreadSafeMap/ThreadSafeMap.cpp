#include "ThreadSafeMap.h"

#ifdef TEST_TSM

#include <string>
#include <iostream>
#include <thread>

ThreadSafeMap<std::string, int> tsmap;

void read(std::string key);
void write(std::string key, int val);

int main() {
	
	std::vector<std::thread> readers;
	std::vector<std::thread> writers;

	std::cout << "Started threads" << std::endl;

	writers.push_back(std::thread(write, "hello", 1));
	writers.push_back(std::thread(write, "world", 2));
	for (std::thread& t : writers) { t.join(); };

	readers.push_back(std::thread(read, "hello"));
	readers.push_back(std::thread(read, "world"));
	for (std::thread& t : readers) { t.join(); };
	
	
	return 0;
}

void read(std::string key) {
	int test = tsmap.get(key);
	std::cout << test << std::endl;
}

void write(std::string key, int val) {
	tsmap.insert(key, val);
	std::cout << key << " : " << val << " inserted" << std::endl;
}
#endif