#include <iostream>
#include "counter.h"

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "ja_JP.UTF-8");

    if (argc <= 3) {
        std::cout
            << "Usage: "
            << argv[0] << " adjective antonym target_pos" << std::endl
            << "see README.md for more information." << std::endl;
        return 1;
    }
    // === read arguments ===
    const std::string adjective = argv[1];
    const std::string antonym = argv[2];
    const std::string target_pos = argv[3];
    // === /read arguments ===


    // initialize a counter
    // target_pos = NOUN or VERB or ADJECTIVE
    pmi_toolkit::Counter counter(adjective, antonym,
                                pmi_toolkit::CounterTags::target(target_pos));

    // TODO: select the type with a command line
    pmi_toolkit::Parser parser(pmi_toolkit::Morph::MORPH_TYPE::IPADIC);
    counter.count(parser);

    counter.output();
    return 0;
}
