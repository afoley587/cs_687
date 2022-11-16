// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1

#include <iostream>

#include "ExecutiveComponent.h"

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

