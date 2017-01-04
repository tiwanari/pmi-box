#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "reducer.h"
#include "counter_tags.h"

namespace pmi_toolkit {
typedef long long COUNT;
bool readLine(std::ifstream& input_file, std::string* read_value)
{
    std::string line;

    // line = e.g. ADJ,大きい
    // reach the end of file
    if (!std::getline(input_file, line)) return false;

    if (*read_value == "") {
        *read_value = line;
         return true;
    }

    if (*read_value != line) {
        // the value is something different
        return false;
    }

    return true;
}

bool readCount(std::ifstream& input_file, COUNT* read_value)
{
    std::string line;
    // reach the end of file
    if (!std::getline(input_file, line)) return false;

    // line = e.g. OC,犬,0
    auto index = line.find_last_of(",");
    if (index == std::string::npos) {
        // NOTE: the format of this file is something wrong
        return false;
    }

    // split string into a pair of key and value
    // const std::string& key = line.substr(0, index);
    const std::string& value = line.substr(index+1);
    *read_value += stov(value);
    return true;
}

bool Reducer::readCount(std::ifstream& input_file)
{
    std::string line;
    // reach the end of file
    if (!std::getline(input_file, line)) return false;

    // line = e.g. OC,犬,0
    auto index = line.find_last_of(",");
    if (index == std::string::npos) {
        // NOTE: the format of this file is something wrong
        return false;
    }

    // split string into a pair of key and value
    const std::string& key = line.substr(0, index);
    const std::string& value = line.substr(index+1);
    m_kvs[key] += stov(value);

    return true;
}
#define WRITE_STAT_TO_FILE(stat, value) \
    output_file << CounterTags::statString(CounterTags::Stat::stat) \
        << "," << value << std::endl;

void Reducer::_write(const std::string& output_filename)
{
    std::ofstream output_file;
    output_file.open(output_filename);

    if (!output_file) {
        std::cerr << "Can't write to: " << output_filename << std::endl;
        output_file.close();
        exit(1);
    }

    output_file << m_adjective << std::endl;
    output_file << m_antonym << std::endl;
    output_file << m_tag << std::endl;
    WRITE_STAT_TO_FILE(TOTAL_WORDS, m_total_words);
    // WRITE_STAT_TO_FILE(VOCABULARY, m_vocabulary);
    WRITE_STAT_TO_FILE(VOCABULARY, -1);
    WRITE_STAT_TO_FILE(POS_OCCURRENCES, m_total_pos_occurrences);
    WRITE_STAT_TO_FILE(NEG_OCCURRENCES, m_total_neg_occurrences);

    for (const auto& pair : m_kvs) {
        // pair = (key, value)
        output_file
            << pair.first << ","
            << pair.second << std::endl;
    }

    output_file.close();
}

bool Reducer::reduce(const std::string& output_filename)
{
    bool doAllFilesExist = true;

    // count hints from all data
    for (const auto& filename : m_input_filenames) {
        struct stat buffer;
        // check path existence
        if (::stat(filename.c_str(), &buffer) != 0) {
            std::cerr << "Not exist: " << filename << std::endl;
            doAllFilesExist = false;
            continue;
        }

        std::ifstream input_file;
        input_file.open(filename);

        std::string line;
        /* 1 */ ::pmi_toolkit::readLine(input_file, &m_adjective);
        /* 2 */ ::pmi_toolkit::readLine(input_file, &m_antonym);
        /* 3 */ ::pmi_toolkit::readLine(input_file, &m_tag);
        /* 4 */ ::pmi_toolkit::readCount(input_file, &m_total_words);
        /* 5 */ ::pmi_toolkit::readCount(input_file, &m_vocabulary); // not works
        /* 6 */ ::pmi_toolkit::readCount(input_file, &m_total_pos_occurrences);
        /* 7 */ ::pmi_toolkit::readCount(input_file, &m_total_neg_occurrences);

        /* 8- */
        while (readCount(input_file)) ;

        input_file.close();
    }

    _write(output_filename);

    return doAllFilesExist;
}
} // namespace pmi_toolkit
