#include "SingleThreadedMap.h"

#ifdef TEST_STM

#include <string>
#include <iostream>
#include <thread>

SingleThreadedMap<std::string, int> stmap;

void read(std::string key);
void write(std::string key, int val);

int main() {

	write("Hello", 1);
	write("World", 2);

	read("Hello");
	read("World");

	return 0;
}

void read(std::string key) {
	int test = stmap.get(key);
	std::cout << test << std::endl;
}

void write(std::string key, int val) {
	stmap.insert(key, val);
	std::cout << key << " : " << val << " inserted" << std::endl;
}
#endif