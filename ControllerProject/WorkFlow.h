#pragma once
#include <vector>
#include <vector>
#include <string>
using namespace std;

enum WorkFlowState { initializing, map_state, reduce_state };

class Workflow 
{
public:
	Workflow() {};

	vector<vector<string>> FilesToReduce(string tempDirectory) 
	{
		
	};
private:
};