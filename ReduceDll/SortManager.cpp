#include "pch.h"

#include "SortManager.h"
#include "FileManager.h"

#include <iterator>
#include <functional>
#include <sstream>
#include <iostream>
#include <deque>
#include <map>
#include <stack>

std::map<std::string, std::vector<int>> ParseFileTextToKeyValueMap(const std::string& dataString);
std::pair<std::string, int> GetKeyValuePairFromParsedText(const std::string stringElement);
std::string AddTickToValue(std::string currentVal, std::string valueToAppend);
std::string FormatStringForReduceMethod(const std::map<std::string, std::vector<int>> map);
void ParseFileTextToKeyValueMap(const std::string& dataString, std::unordered_map<std::string, std::vector<int>>& mapToUse);

std::map<std::string, std::vector<int>> SortManager::SortInput(std::string filePath) {
    std::cout << "[SORT MAN] - Sorting Input From " << filePath << std::endl;

    //Read Map Functions Output File
    std::vector<std::string> dataToBeSorted;
    fileManager.read_file(filePath, dataToBeSorted);

    std::ostringstream fileDataString;
    const char* const delim = " ";

    std::copy(dataToBeSorted.begin(), dataToBeSorted.end(), std::ostream_iterator<std::string>(fileDataString, delim));

    // Hold Results in Memory/Map of Key ValuePairs while aggregating results
    auto testMapParseOutput = ParseFileTextToKeyValueMap(fileDataString.str());

    return testMapParseOutput;
}

std::map<std::string, std::vector<int>> SortManager::SortInputForPaths(std::vector<std::string> filePaths) {

    //Read Map Functions Output File
    std::vector<std::string> dataToBeSorted;
    for (auto file : filePaths)
    {
        std::cout << "[SORT MAN] - Sorting Input From " << file << std::endl;
        fileManager.read_file(file, dataToBeSorted);
    }

    std::ostringstream fileDataString;
    const char* const delim = " ";

    std::copy(dataToBeSorted.begin(), dataToBeSorted.end(), std::ostream_iterator<std::string>(fileDataString, delim));

    // Hold Results in Memory/Map of Key ValuePairs while aggregating results
    auto testMapParseOutput = ParseFileTextToKeyValueMap(fileDataString.str());

    return testMapParseOutput;
}

void SortManager::SortInput_withMap(std::string filePath, std::unordered_map<std::string, std::vector<int>>& mapToUse) 
{
    std::cout << "[SORT MAN] - Sorting Input From " << filePath << std::endl;

    //Read Map Functions Output File
    std::vector<std::string> dataToBeSorted;
    fileManager.read_file(filePath, dataToBeSorted);

    std::ostringstream fileDataString;
    const char* const delim = " ";

    std::copy(dataToBeSorted.begin(), dataToBeSorted.end(), std::ostream_iterator<std::string>(fileDataString, delim));
    ParseFileTextToKeyValueMap(fileDataString.str(), mapToUse);
    
    return;
}


std::map< int, std::map<std::string, std::vector<int>>> SortManager::ChunkMapForReduce(std::map<std::string, std::vector<int>> threadSafeMap, const int numChunks, int chunkIndex)
{
    std::map<std::string, std::vector<int>> resultArray[5];
    std::map<int ,std::map<std::string, std::vector<int>>> result;
    int refMapSize = threadSafeMap.size();

    std::map<std::string, std::vector<int>> tempMap;

    int loopIndex = 1;
    int arrayIndex = 0;
    for (auto kvp : threadSafeMap)
    {
        tempMap[kvp.first] = kvp.second;
        
        if (loopIndex % (refMapSize / numChunks) == 0)
        {
            std::map<std::string, std::vector<int>> mapContainer(tempMap);
            result[arrayIndex] = tempMap;
            //resultArray[arrayIndex] = std::move(mapContainer);
            tempMap = std::map<std::string, std::vector<int>>();
            arrayIndex++;
        }

        loopIndex++;
    }

    return result;
}

std::map<std::string, std::vector<int>> ParseFileTextToKeyValueMap(const std::string& dataString) {
    std::map<std::int16_t, std::deque<std::string>> map;
    std::map<std::string, std::vector<int>> mapResult;
    std::stack<std::string::const_iterator> stack;

    for (auto it = dataString.begin(); it != dataString.end();) {
        if (*it == '(') {
            stack.push(it++);
        }
        else if (*it == ')') {
            auto start = stack.top();
            stack.pop();

            std::string stringToGetKeyValuePair = std::string{ start, ++it };

            auto kvp = GetKeyValuePairFromParsedText(stringToGetKeyValuePair);

            (mapResult.count(kvp.first) == 0) ?
                mapResult[kvp.first].push_back(kvp.second) :
                mapResult[kvp.first].push_back(kvp.second);
        }
        else {
            it++;
        }
    }

    return mapResult;
}

void ParseFileTextToKeyValueMap(const std::string& dataString, std::unordered_map<std::string, std::vector<int>>& mapToUse) {

    std::stack<std::string::const_iterator> stack;

    std::pair<std::string, int> keyValuePairResult;
    std::string::const_iterator startingIterator;
    std::string stringData;
    bool areCapturingString = false;

    for (auto it = dataString.begin(); it != dataString.end();) {
        if (*it == '(') {
            stack.push(it++);
        }
        else if (*it == ')') {
            auto start = stack.top();
            stack.pop();

            std::string stringToGetKeyValuePair = std::string{ start, ++it };

            auto kvp = GetKeyValuePairFromParsedText(stringToGetKeyValuePair);


            if (*it == '\\') {
                continue;
            }

            if (*it == '"') {
                if (!areCapturingString) {
                    areCapturingString = true;
                    continue;
                }
                else {
                    // To ensure we get closing double quote
                    keyValuePairResult.first = stringData;
                    stringData.clear();
                    areCapturingString = false;
                }
            }
            else if (*it == '[') {
                areCapturingString = true;
                continue;
            }
            else if (*it == ']') {
                // To ensure we get closing brackets
                keyValuePairResult.second = stoi(stringData);

                stringData.clear();
                areCapturingString = false;
            }

            // Append string with char if we are capturing characters for either Key or Value
            if (areCapturingString) {
                stringData.append(1, *it);
            }

            mapToUse[keyValuePairResult.first].push_back(keyValuePairResult.second);
        }
        else {
            it++;
        }
    }


}

std::pair<std::string, int> GetKeyValuePairFromParsedText(std::string stringElement) {
    std::pair<std::string, int> keyValuePairResult;
    std::string::const_iterator startingIterator;
    std::string stringData;
    bool areCapturingString = false;

    for (auto currentIterator = stringElement.begin(); currentIterator != stringElement.end(); currentIterator++) {
        char characterToAppend = *currentIterator;

        if (*currentIterator == '\\') {
            continue;
        }

        if (*currentIterator == '"') {
            if (!areCapturingString) {
                areCapturingString = true;
                continue;
            }
            else {
                // To ensure we get closing double quote
                keyValuePairResult.first = stringData;
                stringData.clear();
                areCapturingString = false;
            }
        }
        else if (*currentIterator == '[') {
            areCapturingString = true;
            continue;
        }
        else if (*currentIterator == ']') {
            // To ensure we get closing brackets
            keyValuePairResult.second = stoi(stringData);

            stringData.clear();
            areCapturingString = false;
        }

        // Append string with char if we are capturing characters for either Key or Value
        if (areCapturingString) {
            stringData.append(1, characterToAppend);
        }
    }

    return keyValuePairResult;
}

std::string AddTickToValue(std::string currentVal, std::string valueToAppend) {
    bool areCapturingString = false;
    currentVal.erase(remove(currentVal.begin(), currentVal.end(), '"'), currentVal.end());

    valueToAppend.erase(remove(valueToAppend.begin(), valueToAppend.end(), '"'), valueToAppend.end());

    return currentVal + ", " + valueToAppend;
}

std::string FormatStringForReduceMethod(std::map<std::string, std::string> map) {
    std::string stringResult;

    for (auto keyValuePair : map)
    {
        std::string keyString = keyValuePair.first;
        std::ostringstream formattedKvpElement;
        formattedKvpElement << "(" << "\"" << keyString << "\"" << ", [" << keyValuePair.second << "]), ";
        stringResult += formattedKvpElement.str();
    }

    return stringResult;
}

SORTDLL_API SortManager* SortCreate(FileManager fm, std::string sortFile) {
    return new SortManager(fm, sortFile);
}