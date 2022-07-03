#include "Dictionary.hpp"

#include <algorithm>
#include <iostream>
#include <locale>
#include <vector>

#include "dbc.hpp"

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
