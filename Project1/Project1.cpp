// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1


#include <iostream>
#include <regex>


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

bool prompt_for_dir(FileManager fm, std::string dirname);

int main(int argc, char* argv[])
{
    std::string indir, tmpdir, outdir;

    /*
    if (argc < 4) {
        std::cerr << "Please supply args" << std::endl;
        return 1;
    }
    */

    indir = "c:/Users/alexa/Source/Repos/cs_687/shakespeare"; // argv[1];
    tmpdir = "c:/Users/alexa/Source/Repos/cs_687/tmp"; // argv[2];
    outdir = "c:/Users/alexa/Source/Repos/cs_687/tmp"; // argv[3];

#ifdef  TEST_WRKFLOW

    ExecutiveComponent executiveComponent = ExecutiveComponent(argc, argv);
    executiveComponent.RunProgram();
#endif TEST_SORT
}

