#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>
#include "counter_tags.h"
#include "parser.h"
#include "morph.h"

namespace pmi_toolkit {
class Counter {
    typedef long long COUNT;
    typedef std::unordered_map<std::string, COUNT> WORD_COUNT;
private:
    std::string m_adjective;
    std::string m_antonym;
    CounterTags::Target m_target;
    Morph::POS_TAG m_tag;

    COUNT m_total_words; // # all words
    std::set<std::string> m_vocabulary; // # of vocabulary

    COUNT m_total_pos_occurrences; // adjective or (not antonym)
    COUNT m_total_neg_occurrences; // antonym or (not adjective)

    // occurrence(word) where 'word' has the target tag (m_tag)
    WORD_COUNT m_targets_occurrences;

    // co-occurrence(word, pos/neg)
    WORD_COUNT m_targets_pos_co_occurrences;
    WORD_COUNT m_targets_neg_co_occurrences;

private:
    void reset();
    Morph::POS_TAG convertTargetToTag(const CounterTags::Target& target) const;

    bool searchTarget(const Phrase& phrase, std::set<std::string>* found_targets);
    void _count(const Parser& parser);

    void _write(std::ostream& output_file) const;
public:
    Counter(const std::string& adjective,
            const std::string& antonym,
            const CounterTags::Target& target)
        : m_adjective(adjective), m_antonym(antonym), m_target(target) { m_tag = convertTargetToTag(target); }
    void count(Parser& parser);
    void output() const { _write(std::cout); }
    void save(const std::string& output_filename) const;
};
} // namespace pmi_toolkit
