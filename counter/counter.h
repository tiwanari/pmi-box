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
public:
    enum CountType : unsigned int {
        COOC        = 0x00,
        DEP_ON      = 0x01, // count (1) target depending on adj/ant
        DEP_FROM    = 0x10, // count (2) target depended from adj/ant
        DEP         = 0x11, // count both of (1) and (2)
    };
private:
    std::string m_adjective;
    std::string m_antonym;
    CounterTags::Target m_target;
    Morph::POSTag m_tag;

    CountType m_count_type;

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
    Morph::POSTag convertTargetToTag(const CounterTags::Target& target) const;

    bool searchTarget(const Phrase& phrase, std::set<std::string>* found_targets);
    void countStats(const Phrase& phrase);
    void countCooccurrences(const Parser& parser);
    // == dep ===
    void countDependencies(const Parser& parser);
    void countEachDependency(const Parser& parser, const int phrase_id, const bool is_pos);
    void countDependedTarget(const Parser& parser, const int src, const bool is_pos);
    void countDependingTarget(const Parser& parser, const int dst, const bool is_pos);
    // == /dep ===

    void _write(std::ostream& output_file) const;

public:
    Counter(const std::string& adjective,
            const std::string& antonym,
            const CounterTags::Target& target)
        : m_adjective(adjective), m_antonym(antonym), m_target(target), m_count_type(CountType::COOC)
            { m_tag = convertTargetToTag(target); }
    void setCountType(const CountType& countType) { m_count_type = countType; }
    void count(Parser& parser);
    void output() const { _write(std::cout); }
    void save(const std::string& output_filename) const;
};
} // namespace pmi_toolkit
