// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1


#include <iostream>
#include <regex>


#include "FileManager.h"
#include "MapManager.h"


#define TEST_FM 1
// #define TEST_MM 1
// #define TEST_REDUCE 1
// #define TEST_SORT 1

bool prompt_for_dir(FileManager fm, std::string dirname);

int main(int argc, char* argv[])
{
    std::string indir, tmpdir, outdir;

    if (argc < 4) {
        std::cerr << "Please supply args" << std::endl;
        return 1;
    }

    indir = argv[1];
    tmpdir = argv[2];
    outdir = argv[3];

#ifdef TEST_FM
    std::vector<std::string> test_fm_data;
    std::vector<std::string> test_fm_files;
    FileManager fm;

    fm.read_directory(indir, test_fm_files);

    if (!prompt_for_dir(fm, tmpdir) || !prompt_for_dir(fm, outdir)) {
        std::cerr << "Unable to create some directory. Please see above." << std::endl;
        return 1;
    }

    std::regex indir_re{ indir };

    for (auto f : test_fm_files) {
        std::string tmpfile = f;;
        std::cout << f << std::endl;
        fm.read_file(f, test_fm_data);
        tmpfile.replace(0, indir.length() - 1, tmpdir);
        std::cout << tmpfile << std::endl;
        fm.write_file(tmpfile, test_fm_data);
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

#ifdef  TEST_SORT
    std::string filename{ "" };
    std::vector<std::string> input {"a", "a", "is", "the", "is"};
    std::unordered_map<std::string, std::vector<int>> output; // (( "a", [1,1,1,1] ... )

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