#include "Dictionary.hpp"

#include <algorithm>
#include <iostream>
#include <locale>
#include <vector>

#include "dbc.hpp"

Dictionary::Dictionary()
    : prefix_()
    , isWord_(false)
    , isEmpty_(true) {
    for (size_t i = 0; i < 26; ++i) {
        suffix_[i] = nullptr;
    }
}

Dictionary::~Dictionary() = default;

// assume that c is lowercase character
inline size_t charIndex(char c) {
    return (c - 'a');
}

// Assume that toAdd is all lowercase and is nonempty
void Dictionary::add(std::string const& toAdd) {
    size_t prefixLen = prefix_.length();
    size_t strLen = toAdd.length();

    if (prefixLen == 0) {
        size_t idx = charIndex(toAdd[0]);
        if (isEmpty_) {
            prefix_ = toAdd;
            isWord_ = true;
			isEmpty_ = false;
            return;
        } else if (!suffix_[idx]) {
            suffix_[idx] = std::make_unique<Dictionary>();
        }
        suffix_[idx]->add(toAdd);
        return;
    }
    size_t minLen = (prefixLen < strLen) ? prefixLen : strLen;

    // Find the first location which is not common
    size_t diffIdx = 0;
    for (; diffIdx < minLen; ++diffIdx) {
        if (prefix_[diffIdx] != toAdd[diffIdx])
            break;
    }
    std::string newPrefix = prefix_.substr(0, diffIdx);

    if (diffIdx == prefixLen && diffIdx == strLen) {
        // prefix is same as string. This is a corner case
        // duplicate word or prefix of an existing word in dictionary
        // mark this entry as a word.
        isWord_ = true;
    } else if (diffIdx == prefixLen) {
        // this->prefix is a proper prefix of toAdd
        size_t idx = charIndex(toAdd[diffIdx]);
        if (!suffix_[idx]) {
            suffix_[idx] = std::make_unique<Dictionary>();
            isEmpty_ = false;
        }
        suffix_[idx]->add(toAdd.substr(diffIdx));
    } else if (diffIdx == strLen) {
        // toAdd is a proper prefix of this->prefix
        auto sub_dictionary = std::make_unique<Dictionary>();
        std::string newSuffix = prefix_.substr(diffIdx);
        sub_dictionary->prefix_ = newSuffix;
        sub_dictionary->isWord_ = isWord_;
        if (!isEmpty_) {
            sub_dictionary->isEmpty_ = false;
            for (size_t i = 0; i < 26; ++i) {
                // Move the suffix array to sub_dictionary
                sub_dictionary->suffix_[i] = std::move(suffix_[i]);
            }
        }
        prefix_ = newPrefix;
        isWord_ = true; // toAdd is a word!
        suffix_[charIndex(newSuffix[0])] = std::move(sub_dictionary);
        isEmpty_ = false;
    } else {
        // toAdd and this->prefix are not equal.
        // have a potentially empty common prefix
        auto sub_dictionary = std::make_unique<Dictionary>();
        std::string newSuffix = prefix_.substr(diffIdx);
        sub_dictionary->prefix_ = newSuffix;
        sub_dictionary->isWord_ = isWord_;
        if (!isEmpty_) {
            sub_dictionary->isEmpty_ = false;
            for (size_t i = 0; i < 26; ++i) {
                // Move the suffix array to sub_dictionary
                sub_dictionary->suffix_[i] = std::move(suffix_[i]);
            }
        }
        prefix_ = newPrefix;
        isWord_ = false; // no common word..
        suffix_[charIndex(newSuffix[0])] = std::move(sub_dictionary);
        isEmpty_ = false;

        // Add the appropriate suffix of toAdd
        std::string toAddSuffix = toAdd.substr(diffIdx);
        suffix_[charIndex(toAddSuffix[0])] = std::make_unique<Dictionary>();
        suffix_[charIndex(toAddSuffix[0])]->add(toAddSuffix);
    }
}

void Dictionary::listing(std::string const& prefix) const {
    std::string newPrefix = prefix + prefix_;
    if (isWord_) {
        std::cout << newPrefix << std::endl;
    }
    if (!isEmpty_) {
        for (size_t i = 0; i < 26; ++i) {
            if (suffix_[i]) {
                suffix_[i]->listing(newPrefix);
            }
        }
    }
}

size_t Dictionary::size() const {
    size_t mySize = 0;
    if (!isEmpty_) {
        for (size_t i = 0; i < 26; ++i) {
            if (suffix_[i]) {
                mySize += suffix_[i]->size();
            }
        }
    }
    if (isWord_)
        mySize++;

    return mySize;
}

bool Dictionary::check(std::string const& word, bool toLower) const {
    std::string toCheck{word};

	if (toLower) {
    	std::transform(toCheck.begin(), toCheck.end(), toCheck.begin(), ::tolower);
	}

    if (isEmpty_) {
        // Dictionary is empty
        return false;
    } else if (isWord_ && toCheck == prefix_) {
        // Prefix matches exactly and is a word in the dictionary
        return true;
    } else if (prefix_.size() < toCheck.size() && toCheck.compare(0, prefix_.size(), prefix_) == 0) {
		// Is not a word or if only a prefix matches
		size_t idx = (toCheck[prefix_.size()]) - 'a';
		if (static_cast<bool>(suffix_[idx])) {
			// Check sub-dictionary for suffix
			return suffix_[idx]->check(toCheck.substr(prefix_.size()), false);
		} else {
			// Empty suffix - Not a word in dictionary
			return false;
		}
    } else {
		// prefix size > word size or prefix does not match word -
		// Not a word in dictionary
		return false;
	}
}

namespace utils {

// Split the word at any non-alphabet
std::vector<std::string> split(std::string const& wordList) {
    size_t startPos = 0;
    size_t strLen = wordList.size();
    std::vector<std::string> words;

    for (size_t idx = 0; idx < strLen; ++idx) {
        if (isalpha(wordList[idx])) {
            // increment idx
            continue;
        } else if (startPos == idx) {
            // not yet start of a word
            ++startPos;
			continue;
        } else {
            words.emplace_back(wordList.substr(startPos, idx - startPos));
            // convert to lower case
            std::string& word{words.back()};
            std::transform(word.begin(), word.end(), word.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            startPos = idx + 1;
        }
    }

    if (startPos < strLen) {
        words.emplace_back(wordList.substr(startPos, strLen - startPos));
        // convert to lower case
        std::string& word{words.back()};
        std::transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c) { return std::tolower(c); });
    }

    return words;
}

void splitAndAddToDict(Dictionary& dict, std::string line) {

    size_t startWord = 0;
    size_t endWord = 0;

    size_t strLen = line.length();

    while (startWord < strLen) {
        // find start of word
        while (startWord < strLen && !((line[startWord] >= 'a' && line[startWord] <= 'z') ||
                                       (line[startWord] >= 'A' && line[startWord] <= 'Z'))) {
            startWord++;
        }

        if (startWord == strLen)
            return;

        endWord = startWord;

        // find end of word
        while (endWord < strLen && ((line[endWord] >= 'a' && line[endWord] <= 'z') ||
                                    (line[endWord] >= 'A' && line[endWord] <= 'Z'))) {
            endWord++;
        }

        size_t wordLen = endWord - startWord;
        std::string wordToAdd = line.substr(startWord, wordLen);
        std::transform(wordToAdd.begin(), wordToAdd.end(), wordToAdd.begin(), ::tolower);
        dict.add(wordToAdd);

        startWord = endWord;
    }
}
} // namespace utils
// random line to help mutate-cpp remove above line in a patch
