#include <iostream>
#include <string>
#include "morph.h"
#include "counter.h"
#include "util/cmdline.h" // https://github.com/tanakh/cmdline

using namespace pmi_toolkit;

Morph::MORPH_TYPE getMorphType(const std::string& morph)
{
    if (morph == "IPA")     return Morph::MORPH_TYPE::IPADIC;
    if (morph == "JUMAN")   return Morph::MORPH_TYPE::JUMAN;
    return Morph::MORPH_TYPE::IPADIC;
}

const std::string ARG_ADJ = "adjective";
const std::string ARG_ANT = "antonym";
const std::string ARG_TAG = "target_pos";
const std::string ARG_MOR = "morph";

void parseArguments(cmdline::parser& p, int argc, char** argv)
{
    p.add<std::string>(ARG_ADJ, 'a', "adjective", true);
    p.add<std::string>(ARG_ANT, 'n', "antonym", true);
    p.add<std::string>(ARG_TAG, 'p', "POS [NOUN | VERB | ADJECTIVE]", true, "",
                        cmdline::oneof<std::string>("NOUN", "VERB", "ADJECTIVE"));
    p.add<std::string>(ARG_MOR, 'm', "morph type [IPA | JUMAN]", false, "IPA",
                        cmdline::oneof<std::string>("IPA", "JUMAN"));
    p.parse_check(argc, argv);
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "ja_JP.UTF-8");

    cmdline::parser p;
    parseArguments(p, argc, argv);

    // === read arguments ===
    const std::string adjective     = p.get<std::string>(ARG_ADJ);
    const std::string antonym       = p.get<std::string>(ARG_ANT);
    const std::string target_pos    = p.get<std::string>(ARG_TAG);
    const std::string morph         = p.get<std::string>(ARG_MOR);
    // === /read arguments ===


    // initialize a counter
    // target_pos = NOUN or VERB or ADJECTIVE
    Counter counter(adjective, antonym,
                                CounterTags::target(target_pos));

    Parser parser(getMorphType(morph));
    counter.count(parser);

    counter.output();
    return 0;
}
