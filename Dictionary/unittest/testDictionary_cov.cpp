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
