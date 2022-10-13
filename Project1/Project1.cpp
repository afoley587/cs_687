// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "FileManager.h"
#include "MapManager.h"

// #define TEST_FM 1
// #define TEST_MM 1
// #define TEST_REDUCE 1
// #define TEST_SORT 1

int main()
{

#ifdef TEST_FM
    std::vector<std::string> data;
    FileManager fm;
    fm.read_file("c:/Users/alexa/OneDrive/Desktop/test.txt", data);
    fm.write_file("c:/Users/alexa/OneDrive/Desktop/test2.txt", data);
    // fm.read_directory("c:/Users/alexa/OneDrive/Desktop/");
#endif // TEST_FM

#ifdef TEST_MM
    MapManager mm;
    std::vector<std::string> tokens;
    mm.tokenize("This, is a string; yea", tokens);

    for (auto s : tokens) {
        std::cout << s << std::endl;
    }

    mm.map("c:/Users/alexa/OneDrive/Desktop/test2.txt", "SOME EXTREMELTY LONG AND WILD LINE JKLJS KLJJJKL DJA SLDK JLKDJ  LKSA JDLSA JDASD");

#endif // TEST_MM

#ifdef  TEST_SORT
    std::string filename{ "" };
    std::vector<std::string> input {"a", "a", "is", "the", "is"};
    std::unordered_map<std::string, std::vector<int>> output; // (( "a", [1,1,1,1] ... )

#endif TEST_SORT
}