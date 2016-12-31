#pragma once
#include <vector>
#include <memory>
#include "morph.h"

namespace pmi_toolkit {
class Phrase {
private:
    int m_depend;
    bool m_is_negative;
    Morph::MORPH_TYPE m_morph_type;
    std::vector<std::shared_ptr<Morph>> m_morphs;
private:
    std::shared_ptr<Morph> createMorph(const std::string& infos);
public:
    Phrase() : m_is_negative(false), m_morph_type(Morph::MORPH_TYPE::IPADIC) {}

    Phrase(const std::vector<std::shared_ptr<Morph>>& morphs)
        : m_is_negative(false), m_morph_type(Morph::MORPH_TYPE::IPADIC), m_morphs(morphs) {}

    void setMorhpType(const Morph::MORPH_TYPE& type) { m_morph_type = type; }

    void add(std::shared_ptr<Morph> morph);
    void add(const std::string& infos);
    bool find(const std::string& lemma, Morph::POS_TAG pos) const;
    bool isNegative() const { return m_is_negative; }
    void clear();

    void setDepend(int d) { m_depend = d; }
    const int& depend() const { return m_depend; }

    const std::vector<std::shared_ptr<Morph>>& morphs() const { return m_morphs; }
    std::string phrase() const;
    bool operator==(const Phrase& phrase) const;
    bool operator!=(const Phrase& phrase) const { return !(*this == phrase); }
};
} // namespace pmi_toolkit
