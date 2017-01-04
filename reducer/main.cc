#include <cstdlib>
#include <sstream>
#include <future>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include "reducer.h"
#include "util/trim.h"

void readLinesFromAFile(
    const std::string& input_filename,
    std::vector<std::string>* lines)
{
    std::ifstream input_file;
    input_file.open(input_filename);
    if (!input_file) {
        std::cerr << "Can't open file: " << input_filename << std::endl;
        input_file.close();
        exit(1);
    }
    std::string line;
    while (std::getline(input_file, line)) {
        if (line.empty()) continue;
        lines->emplace_back(util::trim(line));
    }
    input_file.close();
}

void reduce(
    const std::string& input_file_list,
    const std::string& output_file)
{
    // input file is a list of counted result files
    std::vector<std::string> counted_result_files;
    readLinesFromAFile(input_file_list, &counted_result_files);

    // show target files
    std::cerr << "=== reduce target file(s) ===" << std::endl;
    for (const auto& file : counted_result_files)
        std::cerr << file << std::endl;
    std::cerr << "-> " << output_file << std::endl;

    // reducing hints
    pmi_toolkit::Reducer reducer(counted_result_files);
    std::cout << "reducing..." << std::endl;
    reducer.reduce(output_file);
    std::cout << "reduced!" << std::endl;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ja_JP.UTF-8");

    // std::cout << "SVM" << std::endl;
    if (argc <= 2) {
        std::cout
            << "Usage: "
            << argv[0] << " input_list_file output_file" << std::endl
            << "see README.md for more information." << std::endl;
        return 1;
    }

    const std::string kInputFileList = argv[1];
    const std::string kOutputFile = argv[2];

    reduce(kInputFileList, kOutputFile);

    return 0;
}
