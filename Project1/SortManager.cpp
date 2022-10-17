#include "SortManager.h"
#include "FileManager.h"

#include <algorithm>
#include <iterator>
#include <functional>
#include <sstream>
#include <iostream>
#include <deque>
#include <map>
#include <stack>

std::map<std::string, std::string> ParseFileTextToKeyValueMap(const std::string& dataString);
std::map<std::string, std::string> AggregateParsedValues(const std::deque<std::string> parsedValues);
std::pair<std::string, std::string> GetKeyValuePairFromParsedText(const std::string stringElement);
std::string AddTickToValue(std::string currentVal, std::string valueToAppend);
std::string FormatStringForReduceMethod(const std::map<std::string, std::string> map);


void SortManager::SortInput(void) {
	// Read From Test SortInputFile
	std::vector<std::string> data;
	FileManager fileManager; 
	fileManager.read_file("C:\\Users\\kenne\\OneDrive\\Masters_Comp_Sci\\CSE_687_OOD\\Project1\\ExampleFiles\\TestSortInput.txt", data);
    std::string fileDataString;

    std::cout << "\n" << "\n" << "Input From Reading File Expected for Input of Sorting Methods: ";
	std::for_each(data.begin(), data.end(), [&](std::string dataString)
		{
			std::cout << dataString;
            fileDataString = dataString;
		});

    // Hold Results in Memory/Map of Key ValuePairs while aggregating results
    auto testMapParseOutput = ParseFileTextToKeyValueMap(fileDataString);
    
    std::cout << "\n" << "\n" << "Results From Map Generated From Data String: ";
    for (auto kvp : testMapParseOutput) 
    {
        std::cout << "\n" << "Key: " << kvp.first << "\n" << "Value: " << kvp.second;
    }

    // Format Output of Sort Results in Map to an output string
    std::string formatStringForReduce = FormatStringForReduceMethod(testMapParseOutput);

	// Print out all results
    std::cout << "\n" << "\n" << "Results From Formatting Map Data for Reduce Input String: ";
    std::cout << "\n" << formatStringForReduce << "\n";
}

std::map<std::string, std::string> ParseFileTextToKeyValueMap(const std::string& dataString) {
    std::map<std::int16_t, std::deque<std::string>> map;
    std::map<std::string, std::string> mapResult;
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
                mapResult[kvp.first] = kvp.second : 
                mapResult[kvp.first] = AddTickToValue(mapResult[kvp.first], kvp.second);
        }
        else {
            it++;
        }
    }

    return mapResult;
}

std::pair<std::string, std::string> GetKeyValuePairFromParsedText(const std::string stringElement) {
    std::pair<std::string, std::string> keyValuePairResult;
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
            keyValuePairResult.second = stringData;
            
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

std::string FormatStringForReduceMethod(const std::map<std::string, std::string> map) {
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
