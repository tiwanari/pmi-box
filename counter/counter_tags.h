#pragma once
#include <string>
#include <unordered_map>

namespace pmi_toolkit {
class CounterTags {
public:
    enum class Target : unsigned int {
        NOUN,
        VERB,
        ADJECTIVE,
    };
    enum class Stat : unsigned int {
        TOTAL_WORDS,
        VOCABULARY,
        POS_OCCURRENCES, // # positive adjectives
        NEG_OCCURRENCES, // # negative adjectives
        OCCURRENCES,        // # occurrences of targets
        POS_CO_OCCURRENCES, // # co-occurrences of target and positive adjectives
        NEG_CO_OCCURRENCES, // # co-occurrences of target and negative adjectives
    };
private:
    const static std::unordered_map<std::string, CounterTags::Target> m_target_map;
    const static std::unordered_map<std::string, CounterTags::Stat> m_stat_map;
public:
    static bool isTag(const std::string& str) {
        return isTargetVal(str) || isStatVal(str);
    }
    static bool isTargetVal(const std::string& str) { return m_target_map.find(str) != m_target_map.end(); }
    static bool isStatVal(const std::string& str) { return m_stat_map.find(str) != m_stat_map.end(); }

    static const Target target(const std::string& str) { return m_target_map.at(str); }
    static const Stat stat(const std::string& str) { return m_stat_map.at(str); }

    static const std::string targetString(const Target& target) {
        for (const auto& pair : m_target_map) {
            // pair = (target_str, target)
            if (pair.second == target) return pair.first;
        }
        return "";
    }
    static const std::string statString(const Stat& stat) {
        for (const auto& pair : m_stat_map) {
            // pair = (stat_str, stat)
            if (pair.second == stat) return pair.first;
        }
        return "";
    }
};
} // namespace pmi_toolkit
