#include "gtest/gtest.h"

#include "Dictionary.hpp"

#include <string>

TEST(Dictionary_Utils, split) {
    using namespace std::string_literals;

    auto splitWords = utils::split("+-=]\\abc def,;ghi; jkl\0mnopqr s t"s);

    EXPECT_EQ(splitWords.size(), 7);
    EXPECT_EQ(splitWords[0], "abc"s);
    EXPECT_EQ(splitWords[1], "def"s);
    EXPECT_EQ(splitWords[2], "ghi"s);
    EXPECT_EQ(splitWords[3], "jkl"s);
    EXPECT_EQ(splitWords[4], "mnopqr"s);
    EXPECT_EQ(splitWords[5], "s"s);
    EXPECT_EQ(splitWords[6], "t"s);
}

TEST(Dictionary_Utils, tolower) {
    using namespace std::string_literals;

    auto splitWords = utils::split("+-=]\\aBc DEF,;ghI; jkl\0mnopqr s T"s);

    EXPECT_EQ(splitWords.size(), 7);
    EXPECT_EQ(splitWords[0], "abc"s);
    EXPECT_EQ(splitWords[1], "def"s);
    EXPECT_EQ(splitWords[2], "ghi"s);
    EXPECT_EQ(splitWords[3], "jkl"s);
    EXPECT_EQ(splitWords[4], "mnopqr"s);
    EXPECT_EQ(splitWords[5], "s"s);
    EXPECT_EQ(splitWords[6], "t"s);
}

TEST(Dictionary_Utils, empty) {
    using namespace std::string_literals;

    auto splitWords = utils::split("");
    EXPECT_EQ(splitWords.size(), 0);

    auto splitWords1 = utils::split(";-=[]'");
    EXPECT_EQ(splitWords1.size(), 0);
}

