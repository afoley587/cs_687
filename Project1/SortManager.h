#pragma once
#include <algorithm>
#include <iterator>
#include <functional>
#include <sstream>
#include <iostream>
#include <deque>
#include <map>
#include <stack>

class SortManager {
public:
	std::map<std::string, std::vector<int>> SortInput(std::vector<std::string> dataToBeSorted);
};