#pragma once
#include <vector>
#include <memory>
#include "morph.h"

namespace pmi_toolkit {
class Phrase {
private:
    int m_id;
    bool m_is_negative;
    Morph::MorphType m_morph_type;
    std::vector<std::shared_ptr<Morph>> m_morphs;
private:
    std::shared_ptr<Morph> createMorph(const std::string& infos);
public:
    Phrase() : m_is_negative(false), m_morph_type(Morph::MorphType::IPADIC) {}

    Phrase(const std::vector<std::shared_ptr<Morph>>& morphs)
        : m_is_negative(false), m_morph_type(Morph::MorphType::IPADIC), m_morphs(morphs) {}

    void setMorhpType(const Morph::MorphType& type) { m_morph_type = type; }

    void add(std::shared_ptr<Morph> morph);
    void add(const std::string& infos);
    bool find(const std::string& lemma, Morph::POSTag pos) const;
    bool isNegative() const { return m_is_negative; }
    void clear();

    void setId(int id) { m_id = id; }
    const int& id() const { return m_id; }

    const std::vector<std::shared_ptr<Morph>>& morphs() const { return m_morphs; }
    std::string phrase() const;
    bool operator==(const Phrase& phrase) const;
    bool operator!=(const Phrase& phrase) const { return !(*this == phrase); }
};
} // namespace pmi_toolkit
