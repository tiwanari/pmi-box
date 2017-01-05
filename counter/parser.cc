#include <stdexcept>
#include <iostream>
#include "parser.h"
#include "morph.h"
#include "juman_morph.h"
#include "ipa_morph.h"
#include "util/split.h"
#include "util/type.h"

namespace pmi_toolkit {
Parser::Parser(const Morph::MorphType type)
{
    m_cur_phrase.setMorhpType(type); // use the same morph type (e.g., IPA)

    m_num_of_lines = 0;
    m_cur_sentence = "";
}

Parser::Type Parser::sentenceType(
    const std::vector<std::string>& splitted_line) const
{
    // blank or something like that
    if (splitted_line.size() == 0) {
        return Type::UNKNOWN;
    }

    const std::string& first_word = splitted_line[0];
    if (first_word == COMMENT_PREFIX) {
        return Type::UNKNOWN;
    }
    else if (first_word == EOS_PREFIX) {
        return Type::EOS;
    }
    else if (first_word == DIRECTIVE_PREFIX) {
        if (!util::is<int>(splitted_line[1])) {
            return Type::PLAIN;
        }
        return Type::DIRECTIVE;
    }
    return Type::PLAIN;
}

Parser::Type Parser::parseLine(const std::string& line)
{
    std::vector<std::string> splitted_line;
    util::splitStringWithWhitespaces(line, &splitted_line);

    switch (sentenceType(splitted_line)) {
        case Type::DIRECTIVE: {
            // e.g. * 0 1D
            // add current phrase into a vector and concatenate sentence
            if (splitted_line[1] != "0") {
                m_cur_phrases.emplace_back(m_cur_phrase);
                m_cur_sentence += m_cur_phrase.phrase();
            }
            m_cur_phrase.clear(); // create a new phrase

            const int id = std::atoi(splitted_line[1].c_str());
            m_cur_phrase.setId(id);

            // atoi converts 1D (string) to 1 (integer) (and D is ignored)
            const int depend = std::atoi(splitted_line[2].c_str());
            m_cur_connections.emplace_back(depend);
            return Type::DIRECTIVE;
        }

        case Type::PLAIN: {
            // e.g. 知り合い	名詞,普通名詞,*,*,知り合い,未知語
            // create morph and add it into current phrase
            try {
                if (splitted_line.size() < 2) {
                    throw std::runtime_error("Not enough params\n");
                }
                m_cur_phrase.add(line);
            } catch (const std::runtime_error& e) {
                std::stringstream ss;
                ss << e.what();
                ss << "[Error] (S-ID: " << (m_num_of_lines + 1) << "): "
                    << line << std::endl
                    << "so, skip this morph" << std::endl;
                std::cerr << ss.str() << std::endl;
            }
            return Type::PLAIN;
        }

        case Type::EOS: {
            // e.g. EOS
            // add the last word
            m_cur_phrases.emplace_back(m_cur_phrase);
            m_cur_sentence += m_cur_phrase.phrase();
            return Type::EOS;
        }
        case Type::UNKNOWN:
            break;
    }
    return Type::UNKNOWN;
}

bool Parser::next()
{
    // initialize variables for the next sentence
    m_cur_sentence = "";
    std::vector<Phrase>().swap(m_cur_phrases);
    std::vector<int>().swap(m_cur_connections);

    m_num_of_lines++; // count
    while (true) {
        // read a line from stdio
        std::string line;
        const bool is_eof = !std::getline(std::cin, line);

        // break if input reaches its end
        if (is_eof) break;

        // input will continue
        if (parseLine(line) == Type::EOS) return true;
    }
    return false; // the end of input
}

std::vector<std::shared_ptr<Morph>> Parser::morphs() const
{
    std::vector<std::shared_ptr<Morph>> morphs;
    for (const auto& phrase : m_cur_phrases)
        for (const auto& morph : phrase.morphs())
            morphs.emplace_back(morph);
    return morphs;
}
} // namespace pmi_toolkit
