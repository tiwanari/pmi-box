#include <fstream>
#include <algorithm>
#include "counter.h"

namespace pmi_toolkit {
void Counter::reset()
{
    m_total_words = 0;

    m_total_pos_occurrences = 0; // adjective or (not antonym)
    m_total_neg_occurrences = 0; // antonym or (not adjective)

    WORD_COUNT().swap(m_targets_occurrences);

    WORD_COUNT().swap(m_targets_pos_co_occurrences);
    WORD_COUNT().swap(m_targets_neg_co_occurrences);
}

Morph::POSTag Counter::convertTargetToTag(const CounterTags::Target& target) const
{
    switch (target) {
        case CounterTags::Target::NOUN:
            return Morph::POSTag::NOUN;
        case CounterTags::Target::VERB:
            return Morph::POSTag::VERB;
        case CounterTags::Target::ADJECTIVE:
            return Morph::POSTag::ADJECTIVE;
    }
    return Morph::POSTag::NOUN;
}

void Counter::count(Parser& parser)
{
    reset();

    try {
        while (parser.next()) {
            switch (m_count_type) {
                case CountType::COOC:
                    countCooccurrences(parser);
                    break;
                case CountType::DEP_ON:
                case CountType::DEP_FROM:
                case CountType::DEP:
                    countDependencies(parser);
                    break;
            }

            if (parser.lines() % 1000 == 0) {
                std::cerr << "\r" << "read " << parser.lines() << " lines";
            }
        }
        std::cerr << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "counting error!" << std::endl << std::flush;
        throw e;
    }
}

void Counter::countStats(const Phrase& phrase)
{
    m_total_words += phrase.morphs().size(); // count the number of words in a phrase
    // insert words into a set
    for (const auto& morph : phrase.morphs()) {
        if (morph->isUnknown()) m_vocabulary.insert(morph->morph());
        else                    m_vocabulary.insert(morph->lemma());
    }
}

bool Counter::searchTarget(
    const Phrase& phrase,
    std::set<std::string>* found_targets)
{
    bool did_found = false;
    for (const auto& morph : phrase.morphs()) {
        if (morph->POS() != m_tag) continue;

        // found something!
        did_found = true;

        // adjectives sometimes are followed by the negative word
        if (m_tag == Morph::POSTag::ADJECTIVE && phrase.isNegative()) {
            // append the negative word (Japanese grammar)
            // NOTE: this way may miss some words
            // e.g., "綺麗ではない" -> "綺麗でない" ("は" will be ignored)
            const std::string t = morph->morph() + morph->negative()->morph();
            found_targets->emplace(t);
        }
        else {
            // unknown words don't have their lemma
            if (morph->isUnknown()) found_targets->emplace(morph->morph());
            else                    found_targets->emplace(morph->lemma());
        }
    }
    return did_found;
}

void Counter::countCooccurrences(const Parser& parser)
{
    bool pos_flag = false;
    bool neg_flag = false;

    std::set<std::string> found_targets;
    for (const auto& phrase : parser.phrases()) {

        // count statistics
        countStats(phrase);

        // -- search a word with the target tag
        searchTarget(phrase, &found_targets);

        // -- search adjective/antonym
        const bool did_found_adjective =
            phrase.find(m_adjective, Morph::POSTag::ADJECTIVE);
        const bool did_found_antonym =
            phrase.find(m_antonym, Morph::POSTag::ADJECTIVE);

        // not found
        if (!did_found_adjective && !did_found_antonym) continue;

        // positive
        if ((did_found_adjective && !phrase.isNegative())
                || (did_found_antonym && phrase.isNegative())) {
            m_total_pos_occurrences++;
            pos_flag = true;
        }
        // negative
        else {
            m_total_neg_occurrences++;
            neg_flag = true;
        }
    }
    for (const auto& target : found_targets) {
        m_targets_occurrences[target]++;
        if (pos_flag) m_targets_pos_co_occurrences[target]++;
        if (neg_flag) m_targets_neg_co_occurrences[target]++;
    }
}

void Counter::countDependedTarget(const Parser& parser, const int src, const bool is_pos)
{
    const int dst = parser.connections()[src];
    if (dst == -1) return;

    std::set<std::string> found_targets;
    searchTarget(parser.phrases()[dst], &found_targets);

    for (const auto& target : found_targets) {
        if (is_pos) m_targets_pos_co_occurrences[target]++;
        else        m_targets_neg_co_occurrences[target]++;
    }
}

void Counter::countDependingTarget(const Parser& parser, const int dst, const bool is_pos)
{
    const auto& connections = parser.connections();
    const auto& phrases = parser.phrases();

    std::set<std::string> found_targets;
    for (int i = 0; i < connections.size(); ++i) {
        const auto& con = connections[i];
        if (con == dst) searchTarget(phrases[i], &found_targets);
    }

    for (const auto& target : found_targets) {
        if (is_pos) m_targets_pos_co_occurrences[target]++;
        else        m_targets_neg_co_occurrences[target]++;
    }
}


void Counter::countEachDependency(
    const Parser& parser,
    const int phrase_id,
    const bool is_pos)
{
    // m_count_type is a bit field
    if (m_count_type & CountType::DEP_ON)
        countDependingTarget(parser, phrase_id, is_pos);

    if (m_count_type & CountType::DEP_FROM)
        countDependedTarget(parser, phrase_id, is_pos);
}

void Counter::countDependencies(const Parser& parser)
{
    std::set<std::string> found_targets;
    for (const auto& phrase : parser.phrases()) {

        // count statistics
        countStats(phrase);

        // -- search a word with the target tag
        searchTarget(phrase, &found_targets);

        // -- search adjective/antonym
        const bool did_found_adjective =
            phrase.find(m_adjective, Morph::POSTag::ADJECTIVE);
        const bool did_found_antonym =
            phrase.find(m_antonym, Morph::POSTag::ADJECTIVE);

        // not found
        if (!did_found_adjective && !did_found_antonym) continue;

        // positive
        if ((did_found_adjective && !phrase.isNegative())
                || (did_found_antonym && phrase.isNegative())) {
            m_total_pos_occurrences++;
            countEachDependency(parser, phrase.id(), true);
        }
        // negative
        else {
            m_total_neg_occurrences++;
            countEachDependency(parser, phrase.id(), false);
        }
    }

    for (const auto& target : found_targets) {
        m_targets_occurrences[target]++;
    }
}

void Counter::save(const std::string& output_filename) const
{
    std::ofstream output_file;
    output_file.open(output_filename);

    _write(output_file);

    output_file.flush();
    output_file.close();
}

#define WRITE_STAT_TO_FILE(stat, stat_val) \
     output_file \
        << CounterTags::statString(CounterTags::Stat::stat) \
        << "," \
        << stat_val << std::endl << std::flush;

#define COUNT_AT(m, k) (m.find(k) == m.end() ? 0 : m.at(k))

void Counter::_write(std::ostream& output_file) const
{
    // the format of a file is something like this ("#| " will not appear)
    // @see counter_tags.h
    //
    //  1| ADJ, adjective
    //  2| ANT, antonym
    //  3| TAG, tag
    //  4| TOTAL_WORDS, total_words
    //  5| VOCABULARY, vocab
    //  6| POS_OCCURRENCES, pos_occurrences
    //  7| NEG_OCCURRENCES, neg_occurrences
    //  8| OC, target0, 100
    //  9| P_COOC, target0, 10
    // 10| N_COOC, target0, 10
    // 11| OC, target1, 200
    // 12| ...

    /*  1 */ output_file << "ADJ," << m_adjective << std::endl;
    /*  2 */ output_file << "ANT," << m_antonym << std::endl;
    /*  3 */ output_file << "TAG," << CounterTags::targetString(m_target) << std::endl;

    /*  4 */ WRITE_STAT_TO_FILE(TOTAL_WORDS, m_total_words);
    /*  5 */ WRITE_STAT_TO_FILE(VOCABULARY, m_vocabulary.size());
    /*  6 */ WRITE_STAT_TO_FILE(POS_OCCURRENCES, m_total_pos_occurrences);
    /*  7 */ WRITE_STAT_TO_FILE(NEG_OCCURRENCES, m_total_neg_occurrences);
    for (const auto& pair : m_targets_occurrences) {
        // pair = (target_str, count)
        /*  8 */ WRITE_STAT_TO_FILE(OCCURRENCES, pair.first << "," << pair.second);
        /*  9 */ WRITE_STAT_TO_FILE(POS_CO_OCCURRENCES,
                    pair.first << "," << COUNT_AT(m_targets_pos_co_occurrences, pair.first));
        /* 10 */ WRITE_STAT_TO_FILE(NEG_CO_OCCURRENCES,
                    pair.first << "," << COUNT_AT(m_targets_neg_co_occurrences, pair.first));
    }
}
} // namespace pmi_toolkit
