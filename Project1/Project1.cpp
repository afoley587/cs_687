// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1


#include <iostream>

#include "FileManager.h"
#include "MapManager.h"
#include "SortManager.h"
#include "Reduce.h"
#include "WorkFlowComponent.h"
#include "ExecutiveComponent.h"


// #define TEST_FM 1
// #define TEST_MM 1
// #define TEST_REDUCE 1
 #define TEST_WRKFLOW 1

int main(int argc, char* argv[])
{

#ifdef TEST_FM
    std::vector<std::string> data;
    std::vector<std::string> files;
    FileManager fm;
    fm.read_file("c:/Users/alexa/OneDrive/Desktop/test.txt", data);
    fm.write_file("c:/Users/alexa/OneDrive/Desktop/test2.txt", data);
    fm.read_directory("c:/Users/alexa/OneDrive/Desktop", files);

    for (auto f : files) {
        std::cout << f << std::endl;
    }
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

#ifdef  TEST_WRKFLOW

    ExecutiveComponent executiveComponent = ExecutiveComponent(argc, argv);
    executiveComponent.RunProgram();
#endif TEST_SORT
}