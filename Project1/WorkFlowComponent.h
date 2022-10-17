#pragma once

#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "Reduce.h"

class WorkFlowComponent {
private:
	FileManager fileManager;
	MapManager mapManager;
	SortManager sortManager;
	Reduce reduceManager;

public:
	WorkFlowComponent(FileManager fileMgr) :
		fileManager{ fileMgr },
		mapManager{ MapManager() },
		sortManager{ SortManager() },
		reduceManager{ Reduce(fileManager)}
	{};

	void StartWorkFlow();
};