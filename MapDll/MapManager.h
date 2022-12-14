#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "../FileManagerDll/FileManager.h"
#include "../ThreadSafeMap/ThreadSafeMap.h"
#include "../SingleThreadMap/SingleThreadedMap.h"

#ifdef MAPDLL_EXPORTS
#define MAPDLL_API __declspec(dllexport)
#else
#define MAPDLL_API __declspec(dllimport)
#endif

class MAPDLL_API MapManager {
public:
	MapManager();
	MapManager(FileManager fileMgr) :max_buffer_size{ 1024 }, filebuffer{}, fm{ fileMgr }, tempFile{} {};
	MapManager(FileManager fileMgr, int b, std::string temp) :
		max_buffer_size{ b },
		filebuffer{},
		fm{ fileMgr },
		tempFile{ temp }
	{};

	void mapFromVector(std::vector<std::string> lines, bool forceExport);

	inline void setTempFile(std::string t) { tempFile = t; };
	inline std::string getTempFile(void) { return tempFile; };

	/* Reads a line of text, tokenizes it, and puts it into a readable format
	* Args:
	*	filename: Filename (used as key)
	*	line: Line from above file
	*	forceExport: To force an export
	* Returns
	*	...
	*/
	void map(std::string line, bool forceExport = false);

	/* Exports a buffer to disk
	* Args:
	*	key: Key to export
	*	buffer: buffer of tokens
	*	forceExport: To force an export
	* Returns
	*	...
	*/
	void mexport(std::vector<std::string> buffer, bool forceExport = false);

	/*  Tokenizes a line from a strong to a vector
	* Args:
	*	in: line of text
	*	out: Vector of tokens
	* Returns
	*	...
	*/
	void tokenize(std::string in, std::vector<std::string>& out);

private:
	std::string tempFile;
	int numBuckets{ 5 };
	int lastBucket{ 0 };
	std::mutex mut;
	int max_buffer_size; /* Max Number Of Words In A Buffer Vector */
	std::unordered_map<std::string, std::vector<std::string>> filebuffer;
	//Test the below to see if race conds actually exist
	ThreadSafeMap<std::string, std::vector<std::string>> tsm;
	// SingleThreadedMap<std::string, std::vector<std::string>> tsm;
	FileManager fm;
};

extern "C" MAPDLL_API MapManager * MapCreate(FileManager fm, int bufflen, std::string tempfile);