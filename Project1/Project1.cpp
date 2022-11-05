// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1


#include <iostream>
#include <regex>

#include "WorkFlowComponent.h"
#include "ExecutiveComponent.h"

// #define TEST_FM 1
// #define TEST_MM 1
// #define TEST_REDUCE 1
 #define TEST_WRKFLOW 1

bool prompt_for_dir(FileManager fm, std::string dirname);

int main(int argc, char* argv[])
{
    try{
        ExecutiveComponent executiveComponent = ExecutiveComponent(argc, argv);
        executiveComponent.RunProgram();
    }
    catch (std::invalid_argument) {
        return EXIT_FAILURE;
    }
    catch (std::runtime_error) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

