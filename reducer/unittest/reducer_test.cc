#include <gtest/gtest.h>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <map>
#include "../reducer.h"

using namespace pmi_toolkit;

class ReducerTest : public ::testing::Test {
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(ReducerTest, allFilesExist)
{
    const std::vector<std::string> kFiles =
        {"./unittest/dataset/reducer_test_exist0.txt",
         "./unittest/dataset/reducer_test_exist1.txt", };
    const std::string kOutputFile = "./unittest/out/reducer_exist_test.out";
    Reducer reducer(kFiles);
    ASSERT_TRUE(reducer.reduce(kOutputFile));

    struct stat buffer;
    // check path existence
    ASSERT_EQ(0, ::stat(kOutputFile.c_str(), &buffer));
}

TEST_F(ReducerTest, aFileDoesNotExist)
{
    const std::vector<std::string> kFiles =
        {"./unittest/dataset/reducer_test_exist0.txt",
         "./unittest/dataset/reducer_test_exist1.txt",
         "./not_exist.txt", };
    const std::string kOutputFile = "./unittest/out/reducer_not_exist_test.out";
    Reducer reducer(kFiles);
    ASSERT_FALSE(reducer.reduce(kOutputFile));

    struct stat buffer;
    // check path existence
    ASSERT_EQ(0, ::stat(kOutputFile.c_str(), &buffer));
}

TEST_F(ReducerTest, reduceCountIntoOne)
{
    const std::vector<std::string> kFiles =
        {"./unittest/dataset/reducer_test_data0.txt",
         "./unittest/dataset/reducer_test_data1.txt", };
    const std::string kOutputFile = "./unittest/out/reducer_test_count0.out";

    Reducer reducer(kFiles);
    ASSERT_TRUE(reducer.reduce(kOutputFile));

    // check output file
    const std::map<std::string, std::string> kCorrectPairs =
        {{"ADJ", "良い"},
         {"ANT", "悪い"},
         {"TAG", "NOUN"},
         {"TOTAL_WORDS", "35"},
         {"VOCABULARY", "-1"}, // TODO: fix this by keeping dictionaries
         {"POS_OCCURRENCES", "2"},
         {"NEG_OCCURRENCES", "1"},
         {"OC,明日", "1"},
         {"P_COOC,明日", "1"},
         {"N_COOC,明日", "0"},
         {"OC,日", "2"},
         {"P_COOC,日", "1"},
         {"N_COOC,日", "1"},
         {"OC,気分", "1"},
         {"P_COOC,気分", "1"},
         {"N_COOC,気分", "0"},
         {"OC,寝坊", "1"},
         {"P_COOC,寝坊", "0"},
         {"N_COOC,寝坊", "1"},
         {"OC,今日", "2"},
         {"P_COOC,今日", "1"},
         {"N_COOC,今日", "1"},
         {"OC,人生", "1"},
         {"P_COOC,人生", "0"},
         {"N_COOC,人生", "1"},
         {"OC,一", "1"},
         {"P_COOC,一", "0"},
         {"N_COOC,一", "1"},
         {"OC,一番", "1"},
         {"P_COOC,一番", "0"},
         {"N_COOC,一番", "1"},
         {"OC,交通事故", "1"},
         {"P_COOC,交通事故", "0"},
         {"N_COOC,交通事故", "1"}, };

    std::ifstream target_file;
    target_file.open(kOutputFile);

    std::string line;
    int line_count = 0;
    while (std::getline(target_file, line)) {
        line_count++;
        // line = e.g. 大きい,犬,SIMILE,0
        auto index = line.find_last_of(",");
        ASSERT_NE(index, std::string::npos);

        // split string into a pair of key and value
        const std::string& key = line.substr(0, index);
        const std::string& value = line.substr(index+1);
        ASSERT_STREQ(kCorrectPairs.at(key).c_str(), value.c_str());
    }
    ASSERT_EQ(kCorrectPairs.size(), line_count);
    target_file.close();
}
