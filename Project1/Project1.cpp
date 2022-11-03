// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1


#include <iostream>
#include <windows.h>

#include "ExecutiveComponent.h"

#include "MapManager.h"

typedef MapManager* (*funcCreateMapManager)();

int main(int argc, char* argv[])
{

    HINSTANCE mapDLL;
    HINSTANCE fileDLL;

    const wchar_t* mapDLLLibName = L"MapDLL";
    const wchar_t* fileDLLLibName = L"FileManagerDLL";

    mapDLL = LoadLibraryEx(mapDLLLibName, NULL, NULL);
    fileDLL = LoadLibraryEx(fileDLLLibName, NULL, NULL);

    if (mapDLL == NULL) {
        std::cout << "Couldnt loadd map" << std::endl;
    }
    if (mapDLL != NULL) {
        std::cout << "load map" << std::endl;
    }

    if (fileDLL == NULL) {
        std::cout << "Couldnt loadd fileDLL" << std::endl;
    }
    if (fileDLL != NULL) {
        std::cout << "load fileDLL" << std::endl;
    }

    funcCreateMapManager pfnCreateMapManager;
    std::cout << "t1" << std::endl;
    pfnCreateMapManager = (funcCreateMapManager)GetProcAddress(mapDLL, "CreateMapManager");
    std::cout << "t2" << std::endl;
    MapManager *mgr = pfnCreateMapManager();
    std::cout << "t3" << std::endl;

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

