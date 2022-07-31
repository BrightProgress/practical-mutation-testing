#include "gtest/gtest.h"

#include "Dictionary.hpp"

#include <string>

// This test should cover mutations which modify iterations
// that use the character as the index. In particular it
// ensures coverage of the extreme cases of 'a' and 'z'
TEST(DictionaryTopup, wordsWithAZ) {
    Dictionary d;

    d.add("abc"); // start with a
    d.add("tola"); // ends with a
    d.add("cab"); // has a in the middle
    d.add("cas"); // has common prefix ending with a
    d.add("anzac"); // has z in the middle
    d.add("zamin"); // starts with z
    d.add("razz"); // ends with z
    d.add("raze"); // has common prefix ending with z

    EXPECT_EQ(d.size(), 8);
    EXPECT_TRUE(d.check("abc"));
    EXPECT_TRUE(d.check("tola"));
    EXPECT_TRUE(d.check("cab"));
    EXPECT_TRUE(d.check("cas"));
    EXPECT_TRUE(d.check("anzac"));
    EXPECT_TRUE(d.check("zamin"));
    EXPECT_TRUE(d.check("razz"));
    EXPECT_TRUE(d.check("raze"));
}

TEST(DictionaryTopup, testToKill305) {
    Dictionary d;

    d.add("prefix"); // short prefix
    d.add("prefixOfALongWord"); // long word with prefix
    // the above test should result in out-of-range access when
    // calculating the smallest common prefix! However, since
    // the code uses operator[] and not .at(...) this might 
    // just result in "undefined behaviour" - probably can be
    // caught by valgrind??

    EXPECT_EQ(d.size(), 2);
    EXPECT_TRUE(d.check("prefixofalongword"));
    EXPECT_TRUE(d.check("prefix"));
}

TEST(DictionaryTopup, testToKill373) {
    Dictionary d;

    d.add("prefixone");
    d.add("prefixtwo");
    d.add("prefix"); // this will catch the mutation in patch 373

    EXPECT_EQ(d.size(), 3);
    EXPECT_TRUE(d.check("prefixone"));
    EXPECT_TRUE(d.check("prefixtwo"));
    EXPECT_TRUE(d.check("prefix"));

}

TEST(DictionaryTopup, testToKill432) {
    Dictionary d;

    d.add("prefixone");
    d.add("prefixtwo");
    d.add("pre"); // this will catch the mutation in patch 373

    EXPECT_EQ(d.size(), 3);
    EXPECT_TRUE(d.check("prefixone"));
    EXPECT_TRUE(d.check("prefixtwo"));
    EXPECT_TRUE(d.check("pre"));

}

TEST(DictionaryTopup, testToKill497) {
    Dictionary d;

    d.add("preaixone");
    d.add("preaixare");
    d.add("pre"); // this will catch the mutation in patch 497

    EXPECT_EQ(d.size(), 3);
    EXPECT_TRUE(d.check("preaixone"));
    EXPECT_TRUE(d.check("preaixare"));
    EXPECT_TRUE(d.check("pre"));
}

TEST(DictionaryTopup, testToKill500) {
    Dictionary d;

    d.add("prefixone");
    d.add("prefixzre");
    d.add("pre"); // this will catch the mutation in patch 500

    EXPECT_EQ(d.size(), 3);
    EXPECT_TRUE(d.check("prefixone"));
    EXPECT_TRUE(d.check("prefixzre"));
    EXPECT_TRUE(d.check("pre"));
}

TEST(DictionaryTopup, testToKill528) {
    Dictionary d;

    d.add("prefixone");
    d.add("prefixtwo");
    d.add("prefix");
    d.add("pre");

    EXPECT_EQ(d.size(), 4);
    EXPECT_TRUE(d.check("prefixone"));
    EXPECT_TRUE(d.check("prefixtwo"));
    EXPECT_TRUE(d.check("prefix"));
    EXPECT_TRUE(d.check("pre"));
}

TEST(DictionaryTopup, testToKill610) {
    Dictionary d;

    d.add("prefixone");
    d.add("prefixtwo");
    d.add("prefman");
    d.add("pre"); // this will catch the mutation in patch 373

    EXPECT_EQ(d.size(), 4);
    EXPECT_TRUE(d.check("prefixone"));
    EXPECT_TRUE(d.check("prefixtwo"));
    EXPECT_TRUE(d.check("prefman"));
    EXPECT_TRUE(d.check("pre"));

}

TEST(DictionaryTopup, testToKill637) {
    Dictionary d;

    d.add("prefixzne");
    d.add("prefixawo");
    d.add("prefman");
    d.add("pre"); // this will catch the mutation in patch 373

    EXPECT_EQ(d.size(), 4);
    EXPECT_TRUE(d.check("prefixzne"));
    EXPECT_TRUE(d.check("prefixawo"));
    EXPECT_TRUE(d.check("prefman"));
    EXPECT_TRUE(d.check("pre"));
}

TEST(DictionaryTopup, testToKill815) {
    Dictionary d;

    d.add("prefixzne");
    d.add("prefixawo");
    d.add("prefman");
    d.add("prey");

    EXPECT_EQ(d.size(), 4);
    EXPECT_TRUE(d.check("prefixzne"));
    EXPECT_TRUE(d.check("prefixawo"));
    EXPECT_TRUE(d.check("prefman"));
    EXPECT_TRUE(d.check("prey"));
    EXPECT_FALSE(d.check("pray")); // will catch mutation 815
    EXPECT_FALSE(d.check("przy")); // will catch mutation 816
    EXPECT_FALSE(d.check("przy")); // will catch mutation 816
}