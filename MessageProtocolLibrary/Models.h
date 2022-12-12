#pragma once

#include <string>

using namespace std;

enum startEnum { Map = 'm', Reduce = 'r' };

struct Serialized {
	startEnum action;
	string data;
};