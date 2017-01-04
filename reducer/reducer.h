#pragma once
#include <string>
#include <vector>
#include <map>

namespace pmi_toolkit {
class Reducer {
    typedef long long COUNT;
    #define stov(str) (std::stoll(value));
private:
    std::string m_adjective;
    std::string m_antonym;
    std::string m_tag;

    COUNT m_total_words; // # all words
    COUNT m_vocabulary; // # vocab (NOTE: ignore)
    COUNT m_total_pos_occurrences; // adjective or (not antonym)
    COUNT m_total_neg_occurrences; // antonym or (not adjective)

    std::map<std::string, long> m_kvs;
    std::vector<std::string> m_input_filenames;
private:
    bool readCount(std::ifstream& input_file);
    void _write(const std::string& output_filename);
public:
    Reducer(const std::vector<std::string>& input_filenames)
        : m_adjective(""), m_antonym(""), m_tag(""),
            m_total_words(0), m_vocabulary(0), m_total_pos_occurrences(0), m_total_neg_occurrences(0),
            m_input_filenames(input_filenames) {}
    bool reduce(const std::string& output_filename);
    const std::map<std::string, long>& result() { return m_kvs; }
};
} // namespace pmi_toolkit
