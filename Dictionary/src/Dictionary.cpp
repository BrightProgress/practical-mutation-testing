#include "Dictionary.hpp"

#include <algorithm>
#include <iostream>
#include <locale>
#include <vector>
#include <set>

#include "dbc.hpp"

Dictionary::Dictionary()
    : prefix_()
    , isWord_(false)
    , isEmpty_(true)
    , isRoot_(true) {

    DBC_CLASS_INVARIANT(construction);
}

Dictionary::Dictionary(std::string const & word)
    : prefix_(word)
    , isWord_(true)
    , isEmpty_(false)
    , isRoot_(true) {

    DBC_CLASS_INVARIANT(construction);
}

Dictionary::~Dictionary() = default;


// assume that c is lowercase character
inline size_t charIndex(char c) {
    return (c - 'a');
}

inline std::unique_ptr<Dictionary>& Dictionary::suffix(size_t  idx) {
    DBC_ASSERT(suffix_access, idx <= 26);
    return suffix_[idx];
}

inline std::unique_ptr<Dictionary> const & Dictionary::suffix(size_t  idx) const {
    DBC_ASSERT(suffix_access, idx <= 26);
    return suffix_[idx];
}

std::string validateAndTransformWord_(std::string const& toAdd) {
    size_t len = toAdd.size();
    std::string word{toAdd};

    if (len == 0){
        throw  std::invalid_argument("not a word");
    }
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    for (size_t idx = 0; idx < len; ++idx) {
        if (!((word[idx] >= 'a' && word[idx] <= 'z'))) {
            throw std::invalid_argument("not a word");
        }
    }
    return word;
}

void Dictionary::add(std::string const& toAdd) {
    std::string word(validateAndTransformWord_(toAdd));
    add_(word);
}

void Dictionary::add_(std::string const& toAdd) {
    DBC_CLASS_INVARIANT();
    DBC_PRE_POST(add_word_increases_size, ([&]()->std::tuple<bool,size_t, std::set<std::string>>{std::set<std::string> list; listing_("",list); return std::make_tuple(check(toAdd), size(), list);}))
        << dbc::post << 
            [](std::tuple<bool,size_t, std::set<std::string>> pre,std::tuple<bool,size_t,std::set<std::string>> post)->bool{
                return (std::get<0>(post) && // the word is a member in the dictionary
                        (std::get<0>(pre) || std::get<1>(post) == std::get<1>(pre) + 1) && // if word is not a member in pre, then size of dictionary increments by one
                        std::all_of(std::get<2>(pre).cbegin(), std::get<2>(pre).cend(), [&](std::string const & elem){return std::get<2>(post).count(elem) > 0;})); // all words in pre are in post
            };

    size_t prefixLen = prefix_.length();
    size_t strLen = toAdd.length();

    if (prefixLen == 0) {
        size_t idx = charIndex(toAdd[0]);
        if (isEmpty_) {
            // empty root dictionary!
            prefix_ = toAdd;
            isWord_ = true;
			isEmpty_ = false;
        } else if (!suffix(idx)) {
            suffix(idx) = std::unique_ptr<Dictionary>(new Dictionary(toAdd));
            suffix(idx)->isRoot_ = false;
        } else {
            suffix(idx)->add_(toAdd);
        }
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
        if (!suffix(idx)) {
            suffix(idx) = std::unique_ptr<Dictionary>(new Dictionary(toAdd.substr(diffIdx)));
            suffix(idx)->isRoot_ = false;
        } else {
            suffix(idx)->add_(toAdd.substr(diffIdx));
        }
    } else if (diffIdx == strLen) {
        // toAdd is a proper prefix of this->prefix
        std::string newSuffix = prefix_.substr(diffIdx);
        auto sub_dictionary = std::unique_ptr<Dictionary>(new Dictionary(newSuffix));
        sub_dictionary->isRoot_ = false;
        sub_dictionary->isWord_ = isWord_;
        for (size_t i = 0; i < 26; ++i) {
            // Move the suffix array to sub_dictionary
            sub_dictionary->suffix(i) = std::move(suffix(i));
        }
        prefix_ = toAdd;
        isWord_ = true; // toAdd is a word!
        isEmpty_ = false;
        suffix(charIndex(newSuffix[0])) = std::move(sub_dictionary);
    } else {
        // toAdd and this->prefix are not equal.
        // have a potentially empty common prefix

        std::string newSuffix = prefix_.substr(diffIdx);
        std::string toAddSuffix = toAdd.substr(diffIdx);

        auto sub_dictionary = std::unique_ptr<Dictionary>(new Dictionary(newSuffix));
        sub_dictionary->isRoot_ = false;
        sub_dictionary->isWord_ = isWord_;
        for (size_t i = 0; i < 26; ++i) {
            // Move the suffix array to sub_dictionary
            sub_dictionary->suffix(i) = std::move(suffix(i));
        }
        prefix_ = newPrefix;
        isWord_ = false; // no common word..
        suffix(charIndex(newSuffix[0])) = std::move(sub_dictionary);
        isEmpty_ = false;

        // Add the appropriate suffix of toAdd
        // reuse sub_dictionary - it is guaranteed to be nullptr here!
        sub_dictionary = std::unique_ptr<Dictionary>(new Dictionary(toAddSuffix));
        sub_dictionary->isRoot_ = false;
        suffix(charIndex(toAddSuffix[0])) = std::move(sub_dictionary);
    }
}

size_t Dictionary::size() const {
    size_t mySize = 0;
    if (!isEmpty_) {
        for (size_t i = 0; i < 26; ++i) {
            if (static_cast<bool>(suffix(i))) {
                mySize += suffix(i)->size();
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
		if (static_cast<bool>(suffix(idx))) {
			// Check sub-dictionary for suffix
			return suffix(idx)->check(toCheck.substr(prefix_.size()), false);
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

bool Dictionary::classInvariant_() {
    bool res = true;

    res &= (isRoot_ || !prefix_.empty()); // !isRoot_ ==> prefix_ is non-empty (only root entry can have an empty prefix)

    res &= !isWord_ || !prefix_.empty(); // isWord_==> prefix_ is non-empty

    size_t numSubDirs = 0;
    for (size_t idx = 0; idx < 26; ++idx) {
        if (suffix(idx)) {
            ++numSubDirs;
        }
    }
    res &= (isEmpty_ || isWord_ || (numSubDirs > 1)); // if the Dictionary is not empty and prefix is NOT a word then there must be at least 2 sub-dictionaries
    for (size_t idx = 0; idx < 26; ++idx) {
        res &= !static_cast<bool>(suffix(idx)) || 
                charIndex(suffix(idx)->prefix_.at(0)) == idx; // non-empty suffixes start with character corresponding to their index.
    }

    return res;
}

void Dictionary::listing_(std::string const& prefix, std::set<std::string> & list) const {
    std::string newPrefix = prefix + prefix_;
    if (isWord_) {
        list.insert(newPrefix);
    }
    if (!isEmpty_) {
        for (size_t i = 0; i < 26; ++i) {
            if (suffix(i)) {
                suffix(i)->listing_(newPrefix, list);
            }
        }
    }
}

// random line to help mutate-cpp remove above line in a patch
