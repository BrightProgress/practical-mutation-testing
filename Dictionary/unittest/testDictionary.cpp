#include "gtest/gtest.h"

#include "Dictionary.hpp"

#include <string>

TEST(Dictionary, smoke) {
    Dictionary d;

    EXPECT_EQ(d.size(), 0);
    EXPECT_FALSE(d.check("abc"));
}

TEST(Dictionary, basic) {
    Dictionary d;

    d.add("abc");
    d.add("def");

    EXPECT_EQ(d.size(), 2);
    EXPECT_TRUE(d.check("abc"));
    EXPECT_TRUE(d.check("def"));
    EXPECT_FALSE(d.check("abcdef"));

    EXPECT_TRUE(d.check("ABC"));
}

TEST(Dictionary, addDuplicateWords) {
    Dictionary d;

    d.add("abc");
    d.add("def");
    d.add("def");

    EXPECT_EQ(d.size(), 2);
    EXPECT_TRUE(d.check("abc"));
    EXPECT_TRUE(d.check("def"));
    EXPECT_FALSE(d.check("abcdef"));
}

TEST(Dictionary, addSuffix) {
    Dictionary d;

    d.add("abc");
    d.add("abcdef");
    d.add("abcdefghi");

    EXPECT_EQ(d.size(),3);
    EXPECT_TRUE(d.check("abc"));
    EXPECT_TRUE(d.check("abcdef"));
    EXPECT_TRUE(d.check("abcdefghi"));
    EXPECT_FALSE(d.check("abcd"));
}

TEST(Dictionary, addWithPrefix) {
    Dictionary d;

    d.add("abcdef");
    d.add("abcghi");

    EXPECT_EQ(d.size(),2);
    EXPECT_TRUE(d.check("abcdef"));
    EXPECT_TRUE(d.check("abcghi"));
    EXPECT_FALSE(d.check("abc"));
    EXPECT_FALSE(d.check("abcd"));
}

TEST(Dictionary, addAfterPushDown) {
    Dictionary d;

    d.add("abc");
    d.add("def");
    d.add("abcdef");

    EXPECT_EQ(d.size(), 3);
    EXPECT_TRUE(d.check("abcdef"));
    EXPECT_TRUE(d.check("abc"));
    EXPECT_TRUE(d.check("def"));
}
