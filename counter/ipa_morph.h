#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include "util/split.h"
#include "morph.h"

namespace pmi_toolkit {
/**
 * The items in this class are based on IPA dictionary.
 *
 * @see https://osdn.jp/projects/ipadic/
 */
class IPAMorph : public Morph {
    typedef Morph inherited;
private:
    static constexpr const char* STR_POS_NOUN = "名詞";
    static constexpr const char* STR_POS_VERB = "動詞";
    static constexpr const char* STR_POS_ADJECTIVE = "形容詞";
    static constexpr const char* STR_POS_AUXILIARY_VERB = "助動詞";
    /** sub category **/
    static constexpr const char* STR_POS_SUB_ADJ_VERB = "形容動詞語幹";
public:
    void init(const std::string& morph, const std::vector<std::string>& infos)
        throw (std::runtime_error);
    IPAMorph(const std::string& infos);
    IPAMorph(const std::string& morph, const std::string& infos);
    IPAMorph(const std::string& morph, const std::vector<std::string>& infos);
    ~IPAMorph() {};
public:
    static std::shared_ptr<Morph> createAdjectiveMorph(
            const std::shared_ptr<Morph> noun,
            const std::shared_ptr<Morph> aux_verb);
    bool inline isAdjectiveVerb() { return m_sub_poss[0] == STR_POS_SUB_ADJ_VERB; }
    virtual inline std::shared_ptr<Morph> negative() {
        return std::make_shared<IPAMorph>("ない",
                "助動詞,*,*,*,特殊・ナイ,基本形,ない,ナイ,ナイ");
    }
    virtual inline bool isNegative() { return *this == *negative(); }
    virtual POSTag POSFrom(const std::string& str);
};
} // namespace pmi_toolkit
