#include "gtest/gtest.h"

#include "Dictionary.hpp"

#include <string>

/// Requirements for Dictionary class
/// 1.  Addition: Should be able to store non-empty words encoded using ASCII characters 'a-z' and 'A-Z'.
/// 2.  Input validation: Should reject words that do not meet the above specification
/// 3.  Input validation: Throws a std::exception if input does not pass validation
/// 4.  Input performance: Complexity of adding a word is linear in the size of the input word
/// 5.  Counting: Should be able to provide a count of the number of words in the dictionary
/// 6.  Counting: Duplicate words or words that differ only in capitalization are considered equal
/// 7.  Initialization: The dictionary is initially empty
/// 8.  Memory limits: There is no programmed limit on the size of the dictionary. It is limited 
///     only by the amount of memory in the system
/// 9.  Recall: It should be able to check for membership of an input word. The input word is
///     treated as being case insensitive.
/// 10. Recall: Only words that have been added should pass the membership test.
/// 11. Recall Performance: The complexity of recall is linear in the size of the word being checked
/// 12. Empty word: The empty word is not a member of the dictionary. 
/// 13. Storage requirements: The amount of memory used for the dictionary is linear in the size
///     all the words stored in the dictionary
///
///
TEST(Dictionary_Req, Initialization) {
    // Req 7
    Dictionary d;

    EXPECT_EQ(d.size(), 0);
    EXPECT_FALSE(d.check("abc"));
}

TEST(Dictionary_Req, Addition) {
    // Req 1
    Dictionary d;

    d.add("abcd");
    d.add("DefG");
}

TEST(Dictionary_Req, Validation_1) {
    // Req 2, Req3
    Dictionary d;
    
    d.add("abcde");

    EXPECT_THROW(d.add("ab890"), std::invalid_argument);
    EXPECT_THROW(d.add("ab cde"), std::invalid_argument);
    EXPECT_THROW(d.add(""), std::invalid_argument);

    d.add("abcde");
}

TEST(Dictionary_Req, Counting_1) {
    // Req 5, Req 6
    Dictionary d;

    EXPECT_EQ(0, d.size());

    d.add("abc");
    EXPECT_EQ(1,d.size());
    d.add("abc");
    EXPECT_EQ(1, d.size());
    d.add("ABC");
    EXPECT_EQ(1, d.size());

    d.add("abcd");
    EXPECT_EQ(2, d.size());

    d.add("efgh");
    EXPECT_EQ(3, d.size());
}

TEST(Dictionary_Req, Recall_1) {
    // Req 9, 10, 12

    Dictionary d;

    // Add some words - mix case
    d.add("abc");
    d.add("abcde");
    d.add("defghi");
    d.add("AbC");
    d.add("DefGhi");

    // 9
    EXPECT_TRUE(d.check("ABC"));
    EXPECT_TRUE(d.check("aBc"));
    EXPECT_TRUE(d.check("DeFGhI"));

    //11
    EXPECT_FALSE(d.check("ab"));
    EXPECT_FALSE(d.check("abcdef"));
    EXPECT_FALSE(d.check("defghij"));

    //12
    EXPECT_FALSE(d.check(""));
}
