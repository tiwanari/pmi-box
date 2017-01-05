#include <cstdlib>
#include <sstream>
#include <future>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include "reducer.h"
#include "util/trim.h"
#include "util/cmdline.h"

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

const std::string ARG_IN  = "input_list_file";
const std::string ARG_OUT = "output_file";

void parseArguments(cmdline::parser& p, int argc, char** argv)
{
    p.add<std::string>(ARG_IN,  'i', "a file shows a list of inputs", true);
    p.add<std::string>(ARG_OUT, 'o', "a file for output", true);
    p.parse_check(argc, argv);
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ja_JP.UTF-8");

    cmdline::parser p;
    parseArguments(p, argc, argv);

    // === read arguments ===
    const std::string input_list_file   = p.get<std::string>(ARG_IN);
    const std::string output_file       = p.get<std::string>(ARG_OUT);
    // === /read arguments ===

    reduce(input_list_file, output_file);

    return 0;
}
