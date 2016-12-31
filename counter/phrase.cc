#include <iostream>
#include "phrase.h"
#include "ipa_morph.h"
#include "juman_morph.h"

namespace pmi_toolkit {
std::shared_ptr<Morph> Phrase::createMorph(const std::string& infos)
{
    switch (m_morph_type) {
        case Morph::MORPH_TYPE::JUMAN:
            return std::shared_ptr<Morph>(new JumanMorph(infos));
        case Morph::MORPH_TYPE::IPADIC:
            return std::shared_ptr<Morph>(new IPAMorph(infos));
        default:
            // TODO: throw an exception
            return nullptr;
    }
}

void Phrase::add(const std::string& infos)
{
    add(createMorph(infos));
}

void Phrase::add(const std::shared_ptr<Morph> morph)
{
    if (m_morphs.size() != 0 && morph->isNegative())
        m_is_negative = true;

    // if it is an IPA dic morph and the type of it is auxiliary verb,
    // it can be seen as a part of an adjective verb
    // e.g.,
    // 綺麗   	名詞,形容動詞語幹,*,*,*,*,綺麗,キレイ,キレイ
    // だ     	助動詞,*,*,*,特殊・ダ,基本形,だ,ダ,ダ           <- may be this!
    if (m_morph_type == Morph::MORPH_TYPE::IPADIC
            && morph->POS() == Morph::POS_TAG::AUXILIARY_VERB
            && m_morphs.size() > 0) {
        // check the previous morph
        std::shared_ptr<IPAMorph> prev
            = std::dynamic_pointer_cast<IPAMorph>(m_morphs.back());
        // check if 'prev' has '形容動詞語幹' as a sub POS or not
        if (prev->isAdjectiveVerb()) {
            // merge them
            std::shared_ptr<Morph> new_morph
                = IPAMorph::createAdjectiveMorph(prev, morph);
            m_morphs.pop_back(); // delete previous one
            m_morphs.emplace_back(new_morph);
            return ;
        }
    }

    m_morphs.emplace_back(morph);
}

bool Phrase::find(const std::string& lemma, Morph::POS_TAG pos) const
{
    std::string concatenated_morphs = "";
    // reverse traversal (concept can be separated into 2 or more parts)
    // e.g. 冷蔵 庫
    for (int i = m_morphs.size() - 1; i >= 0; --i) {
        const auto& morph = m_morphs[i];
        if (morph->POS() != pos) {
            concatenated_morphs = "";
            continue;
        }

        // concatenate
        if (morph->isUnknown()) concatenated_morphs.insert(0, morph->morph());
        else                    concatenated_morphs.insert(0, morph->lemma());

        // check it size is over the lemma
        if (concatenated_morphs.size() > lemma.size()) break;

        // check lemma
        if (concatenated_morphs == lemma) return true;
    }
    return false;
}

void Phrase::clear()
{
    m_is_negative = false;
    std::vector<std::shared_ptr<Morph>>().swap(m_morphs);
}

bool Phrase::operator==(const Phrase& phrase) const
{
    return this->morphs() == phrase.morphs();
}

std::string Phrase::phrase() const
{
    std::string connected_morphs = "";
    for (const auto& morph : m_morphs) {
        connected_morphs += morph->morph();
    }
    return connected_morphs;
}
} // namespace pmi_toolkit
