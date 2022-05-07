#include "Dictionary.hpp"

#include <algorithm>
#include <iostream>
#include <locale>

Dictionary::Dictionary()
: prefix_()
, isWord_(false)
, isEmpty_(true)
{
	for (size_t i = 0; i < 26; ++i) {
		suffix_[i] = 0;
	}
}

Dictionary::~Dictionary() {

}

// assume that c is lowercase character
inline size_t charIndex(char c) {
   return (c - 'a');
}

// Assume that toAdd is all lowercase
void Dictionary::addWord(std::string const & toAdd) {
	size_t prefixLen = prefix_.length();
	size_t strLen = toAdd.length();

	if (prefixLen == 0) {
		size_t idx = charIndex(toAdd[0]);
		if (isEmpty_) {
			prefix_ = toAdd;
			isWord_ = true;
			return;
		} else if (!suffix_[idx]) {
			suffix_[idx] = new Dictionary();
		}
		suffix_[idx]->addWord(toAdd);
		return;
	}
	size_t minLen = (prefixLen < strLen) ? prefixLen : strLen;

	// Find the first location which is not common
	size_t diffIdx = 0;
	for (; diffIdx < minLen; ++diffIdx){
		if (prefix_[diffIdx] != toAdd[diffIdx]) break;
	}
	std::string newPrefix = prefix_.substr(0,diffIdx);

	if (diffIdx == prefixLen && diffIdx == strLen) {
		// prefix is same as string. This is a corner case
		// duplicate word or prefix of an existing word in dictionary
		// mark this entry as a word.
		isWord_ = true;
	} else if (diffIdx == prefixLen) {
		// this->prefix is a proper prefix of toAdd
		size_t idx = charIndex(toAdd[diffIdx]);
		if (!suffix_[idx]) {
			suffix_[idx] = new Dictionary();
			isEmpty_ = false;
		}
		suffix_[idx]->addWord(toAdd.substr(diffIdx));
	} else if (diffIdx == strLen) {
		// toAdd is a proper prefix of this->prefix
		Dictionary * sub_dictionary = new Dictionary();
		std::string newSuffix = prefix_.substr(diffIdx);
		sub_dictionary->prefix_ = newSuffix;
		sub_dictionary->isWord_ = isWord_;
		if (!isEmpty_) {
			sub_dictionary->isEmpty_ = false;
			for (size_t i = 0; i < 26; ++i) {
				// Move the suffix array to sub_dictionary
				sub_dictionary->suffix_[i] = suffix_[i];
				suffix_[i] = 0;
			}
		}
		prefix_ = newPrefix;
		isWord_ = true; // toAdd is a word!
		suffix_[charIndex(newSuffix[0])] = sub_dictionary;
		isEmpty_ = false;
	} else {
		// toAdd and this->prefix are not equal.
		// have a potentially empty common prefix
		Dictionary * sub_dictionary = new Dictionary();
		std::string newSuffix = prefix_.substr(diffIdx);
		sub_dictionary->prefix_ = newSuffix;
		sub_dictionary->isWord_ = isWord_;
		if (!isEmpty_) {
			sub_dictionary->isEmpty_ = false;
			for (size_t i = 0; i < 26; ++i) {
				// Move the suffix array to sub_dictionary
				sub_dictionary->suffix_[i] = suffix_[i];
				suffix_[i] = 0;
			}
		}
		prefix_ = newPrefix;
		isWord_ = false; // no common word..
		suffix_[charIndex(newSuffix[0])] = sub_dictionary;
		isEmpty_ = false;

		//Add the appropriate suffix of toAdd
		std::string toAddSuffix = toAdd.substr(diffIdx);
		suffix_[charIndex(toAddSuffix[0])] = new Dictionary();
		suffix_[charIndex(toAddSuffix[0])]->addWord(toAddSuffix);
	}
}

void Dictionary::listing(std::string const & prefix) const {
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

void splitAndAddToDict(Dictionary & dict, std::string line) {

	size_t startWord = 0;
	size_t endWord = 0;

	size_t strLen = line.length();

	while (startWord < strLen){
		// find start of word
		while ( startWord < strLen &&
				! ((line[startWord] >= 'a' && line[startWord] <= 'z') ||
			       (line[startWord] >= 'A' && line[startWord] <= 'Z')) ) {
			startWord ++;
		}

		if (startWord == strLen) return;

		endWord = startWord;

		// find end of word
		while ( endWord < strLen &&
				((line[endWord] >= 'a' && line[endWord] <= 'z') ||
			     (line[endWord] >= 'A' && line[endWord] <= 'Z')) ) {
			endWord ++;
		}

		size_t wordLen = endWord - startWord;
		std::string wordToAdd = line.substr(startWord,wordLen);
		std::transform(wordToAdd.begin(), wordToAdd.end(), wordToAdd.begin(), ::tolower);
		dict.addWord(wordToAdd);

		startWord = endWord;
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
	if (isWord_) mySize++;

	return mySize;
}

int main(int argc, char ** argv) {
	Dictionary dict;

	size_t numLines;
	std::cin >> numLines;
	std::string dummy;
	std::getline(std::cin, dummy);
	for (size_t i = 0; i < numLines; ++i) {
		std::string line;
		std::getline(std::cin, line);
		splitAndAddToDict(dict, line);
	}

	std::cout << dict.size() << std::endl;
	dict.listing();
}