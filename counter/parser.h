#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include "phrase.h"
#include "morph.h"

namespace pmi_toolkit {
class Parser {
private:
    static constexpr const char* COMMENT_PREFIX = "#";
    static constexpr const char* DIRECTIVE_PREFIX = "*";
    static constexpr const char* EOS_PREFIX = "EOS";
    enum class Type : unsigned char {
        PLAIN,
        DIRECTIVE,
        EOS, // end of sentence
        UNKNOWN,
    };
private:
    long m_num_of_lines;

    std::string m_cur_sentence;
    Phrase m_cur_phrase;

    std::vector<Phrase> m_cur_phrases;
    std::vector<int> m_cur_connections;
private:
    void init(const Morph::MorphType type);
    Parser::Type sentenceType(
        const std::vector<std::string>& splitted_line) const;
    Parser::Type parseLine(const std::string& line);
public:
    Parser(const Morph::MorphType type);
    bool next();
    const long& lines() const { return m_num_of_lines; }
    const std::string& raw() const { return m_cur_sentence; }
    const std::vector<Phrase>& phrases() const { return m_cur_phrases; }
    std::vector<std::shared_ptr<Morph>> morphs() const;
    const std::vector<int>& connections() const { return m_cur_connections; }
};
} // namespace pmi_toolkit
