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

#ifdef TEST_MM
    std::vector<std::string> test_fm_data;
    std::vector<std::string> test_fm_files;
    FileManager fm;
    MapManager mm {1024};

    if (!prompt_for_dir(fm, tmpdir) || !prompt_for_dir(fm, outdir)) {
        std::cerr << "Unable to create some directory. Please see above." << std::endl;
        return 1;
    }

    fm.read_directory(indir, test_fm_files);

    for (auto f : test_fm_files) {
        std::string tmpfile = f;
        tmpfile.replace(0, indir.length(), tmpdir);
        std::cout << "Processinng " << f << " into " << tmpfile << std::endl;

        fm.read_file(f, test_fm_data);
        for (int i = 0; i < test_fm_data.size(); i++) {
            bool isLast = (i == test_fm_data.size() - 1);
            mm.map(tmpfile, test_fm_data[i], isLast);
        }
        test_fm_data.clear();
    }
#endif // TEST_MM

#ifdef  TEST_WRKFLOW

    ExecutiveComponent executiveComponent = ExecutiveComponent(argc, argv);
    executiveComponent.RunProgram();
#endif TEST_SORT
}

bool prompt_for_dir(FileManager fm, std::string dirname) {
    if (!fm.directory_exists(dirname)) {
        char yes_no;
        std::cout << dirname + " does not exists. Would you like to create it (y/n)?" << std::endl;
        std::cin >> yes_no;

        if (yes_no == 'y' || yes_no == 'Y') {
            return fm.mkdir(dirname);
        }
        else {
            std::cout << "Please create directory and run again!" << std::endl;
            return false;
        }
    }
    return true;

}