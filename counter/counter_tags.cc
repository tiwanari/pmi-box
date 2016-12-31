#include "counter_tags.h"

namespace pmi_toolkit {
const std::unordered_map<std::string, CounterTags::Target> CounterTags::m_target_map = {
    {"NOUN", CounterTags::Target::NOUN},
    {"VERB", CounterTags::Target::VERB},
    {"ADJECTIVE", CounterTags::Target::ADJECTIVE},
};

const std::unordered_map<std::string, CounterTags::Stat> CounterTags::m_stat_map = {
    {"TOTAL_WORDS", CounterTags::Stat::TOTAL_WORDS},
    {"VOCABULARY", CounterTags::Stat::VOCABULARY},
    {"POS_OCCURRENCES", CounterTags::Stat::POS_OCCURRENCES},
    {"NEG_OCCURRENCES", CounterTags::Stat::NEG_OCCURRENCES},

    // the following tags will be used many times
    // -> short names
    {"OC", CounterTags::Stat::OCCURRENCES},
    {"P_COOC", CounterTags::Stat::POS_CO_OCCURRENCES},
    {"N_COOC", CounterTags::Stat::NEG_CO_OCCURRENCES},
};
} // namespace pmi_toolkit
